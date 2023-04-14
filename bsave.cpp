/*
Original code by bupper
Modified by codeninja
*/
#include "bsave.h"


static	unsigned char byBSaveBuff[256*212];



static unsigned long MakeBSaveData(unsigned char* lpSrc, int nWidth, int nHeight, bool bCenter, bool bFull)
{
	int		i;
	int		nXDstOffset, nXSrcOffset, nWidth2;
	int		nYDstOffset, nYSrcOffset, nHeight2;

	nWidth = (nWidth + 3) & -4;
	memset(byBSaveBuff, 0, sizeof(byBSaveBuff));

	nYSrcOffset = 0;
	nHeight2 = nHeight;
	if(nHeight2 > 212){
		nHeight2 = 212;
		nYSrcOffset = bCenter ? (nHeight - 212) / 2 : 0;
	}
	nYDstOffset = 0;
	if(nHeight2 < 212){
		nYDstOffset = bCenter ? (212 - nHeight2) / 2 : 0;
	}

	nXSrcOffset = 0;
	nWidth2 = nWidth;
	if(nWidth2 > 256){
		nWidth2 = 256;
		nXSrcOffset = bCenter ? ((nWidth - 256) / 2) & -4 : 0;
	}
	nXDstOffset = 0;
	if(nWidth2 < 256){
		nXDstOffset = ((256 - nWidth2) / 2) & -4;
	}

	for(i = 0; i < nHeight2; i++){
		memcpy(
			byBSaveBuff + nXDstOffset + (nYDstOffset + i) * 256,
			lpSrc + nXSrcOffset + (nYSrcOffset + i) * nWidth,
			nWidth2
		);		
	}

	return bFull ? 256 * 212 : (nYDstOffset + nHeight2) * 256;
}


bool BSave(char* lpszFilePath, void* lpSrc, int nWidth, int nHeight, bool bCenter, bool bFull)
{
	unsigned long	dwSize;
	unsigned char	byBuff[1+2+2+2];
	int	hFile;
	unsigned long	dw;
	bool	bResult = false;

	dwSize = MakeBSaveData((unsigned char*)lpSrc, nWidth, nHeight, bCenter, bFull);
	byBuff[0] = 0xFE;
	byBuff[1] = 0;
	byBuff[2] = 0;
	byBuff[3] = (unsigned char)(dwSize & 255);
	byBuff[4] = (unsigned char)((dwSize >> 8) & 255);
	byBuff[5] = 0;
	byBuff[6] = 0;

    hFile = open(lpszFilePath, O_CREAT|O_RDWR, 0666);
	if(hFile != -1){
        dw = write(hFile, byBuff, 7);
		if(dw == 7){
            dw = write(hFile, byBSaveBuff, dwSize);
			if(dw == dwSize){
				bResult = true;
			} else fprintf(stderr, "Error (1): cannot write '%s'.\n", lpszFilePath);
		} else fprintf(stderr, "Error (2): cannot write '%s'.\n", lpszFilePath);
		close(hFile);
	} else fprintf(stderr, "Error: cannot open '%s'.\n", lpszFilePath);

	return bResult;
}

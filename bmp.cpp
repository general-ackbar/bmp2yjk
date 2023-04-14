/*
Original code by bupper
Modified by codeninja
*/


#include "bmp.h"


void* LoadBmp(char* lpszFilePath, int *pnWidth, int *pnHeight)
{
	BitmapFileHeader	bmfh;
	BitmapInfoHeader	bmih;
	unsigned long		dwSize;
	void*				lpBuff = NULL;
	bool				bResult = false;
	int				    hFile;
	unsigned long		dw;
    
    
    
    hFile = open(lpszFilePath, O_RDONLY);
        
	if(hFile != -1){
        //printf("Size: %lu\n", sizeof(bmfh));
        dw = read(hFile, &bmfh.bfType, sizeof(bmfh.bfType));
        dw += read(hFile, &bmfh.bfSize, sizeof(bmfh.bfSize));
        dw += read(hFile, &bmfh.bfReserved1, sizeof(bmfh.bfReserved1));
        dw += read(hFile, &bmfh.bfReserved2, sizeof(bmfh.bfReserved2));
        dw += read(hFile, &bmfh.bfOffBits, sizeof(bmfh.bfOffBits));
        dw += 2;
        /*
        printf("type: %hx\n", bmfh.bfType);
        printf("size: %u\n", bmfh.bfSize);
        printf("bits: %u\n", bmfh.bfOffBits);
        printf("dw: %lu\n", dw);
        */
        if(dw == 0) dw = 0xFFFFFFFF;
        
		if(dw == sizeof(bmfh)){
            dw = read(hFile, &bmih, sizeof(bmih) );
            if(dw == 0) dw = 0xFFFFFFFF;
			if(dw == sizeof(bmih)){
				if(bmih.biBitCount == 24){
					if(pnWidth != NULL) *pnWidth = bmih.biWidth;
					if(pnHeight != NULL) *pnHeight = bmih.biHeight;
					dwSize = ((bmih.biWidth * 3 + 3) & -4) * bmih.biHeight;
					lpBuff = malloc(dwSize);
					if(lpBuff != NULL){
						if(lseek(hFile, bmfh.bfOffBits, SEEK_SET) == bmfh.bfOffBits){
                            dw = read(hFile, lpBuff, dwSize);
                                                        
							if(dw == dwSize){
								bResult = true;
							} else fprintf(stderr, "cannot read '%s'(%lu).\n", lpszFilePath, dwSize);
						} else fprintf(stderr, "cannot seek '%s'(%d).\n", lpszFilePath, bmfh.bfOffBits);
						if(!bResult){
							free(lpBuff);
							lpBuff = NULL;
						}
					} else fprintf(stderr, "Error: cannot allocate memory.\n");
				} else fprintf(stderr, "Only 24bit / truecolor format supported.\n");
			} else fprintf(stderr, "Error (1): cannot read '%s'\n", lpszFilePath);
		} else fprintf(stderr, "Error (2): cannot read '%s'\n", lpszFilePath);
		close(hFile);
	} else fprintf(stderr, "Error: cannot open '%s'.\n", lpszFilePath);
	return lpBuff;
}

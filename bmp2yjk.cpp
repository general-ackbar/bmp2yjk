/*
Original code by bupper
Modified by codeninja
*/


#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "bmp.h"
#include "bsave.h"
#include "conv.h"


static bool ConvertFile(char* lpszDstFile, char* lpszSrcFile, int nAlgo, int nScreenMode, int nDither)
{
	int		nWidth, nHeight;
	void*	lpData;
	void*	lpBuff;
	bool	bResult = false;

	lpData = LoadBmp(lpszSrcFile, &nWidth, &nHeight);
	if(lpData != NULL){
		lpBuff = malloc(((nWidth + 3) & -4) * nHeight);
		if(lpBuff != NULL){
			if(ConvYJK(lpBuff, lpData, nWidth, nHeight, nAlgo, nScreenMode, nDither)){
				bResult = BSave(lpszDstFile, lpBuff, nWidth, nHeight, true, true);
			}
			free(lpBuff);
		}
		free(lpData);
	}
	return bResult;
}


static void usage()
{
	fprintf(stdout,
		"usage: bmp2yjk [source file] [dest file] [options]\n"
		"option  -screen12 : make screen12 file\n"
		"        -screen10 : make screen10 file\n"
		"        -algo	   : algorithm tu use [1..7] \n"
		"        -dither   : [0..3] use 2x2, 2x4, 4x2 or 4x4 dither pattern\n");
}

int main(int argc, char *argv[])
{
	int		i;
	int		nAlgo;
	int		nScreenMode;
	int		nDither;
    char*	szSrcFile = 0;
    char*	szDstFile = 0;
    
    //Default values
    nScreenMode = 12;
    nDither = 0;
    nAlgo = 5;


	fprintf(stdout, "BMP to YJK converter ver0.2b, created by buppu, modified by codeninja.\n");



for(i = 1; i < argc; i++){
		if(*argv[i] == '/' || *argv[i] == '-'){
			if(strcmp(argv[i]+1, "h") == 0){
				usage();
				return 0;
			} else if(strcmp(argv[i]+1,"help") == 0){
				usage();
				return 0;
			} else if(strcmp(argv[i]+1,"?") == 0){
				usage();
				return 0;
			} else if(strncmp(argv[i]+1,"screen12", 8) == 0){
				nScreenMode = 12;
				continue;
			} else if(strncmp(argv[i]+1,"screen10", 8) == 0){
				nScreenMode = 10;
				continue;
			} else if(strncmp(argv[i]+1, "dither", 6) == 0){
				nDither = atoi(argv[i]+8);
				continue;
			} else if(strncmp(argv[i]+1, "algo", 4) == 0){
				nAlgo = atoi(argv[i]+6);
				continue;
			} 
		} else {
			if(i==1){
				szSrcFile = argv[i];
				continue;
			} else if(i==2){
				szDstFile = argv[i];
				continue;
			}
		}
	}


	if(szDstFile == 0)
	{ 
		usage();
		return 0;
	}


	if(!ConvertFile(szDstFile, szSrcFile, nAlgo, nScreenMode, nDither)) return 1;

	fprintf(stdout, "Conversion done.\n");
	return 1;
}

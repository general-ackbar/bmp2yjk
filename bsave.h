/*
Original code by bupper
Modified by codeninja
*/

#ifndef	_INC_BSAVE_H_
#define	_INC_BSAVE_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


bool BSave(char* lpszFilePath, void* lpSrc, int nWidth, int nHeight, bool bCenter, bool bFull);

#endif

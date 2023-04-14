/*
Original code by bupper
Modified by codeninja
*/

#include "conv.h"


int		nDitherPattern[4][16] = {
			{
				 0,  0,  0,  0,
				 0,  0,  0,  0,
				 0,  0,  0,  0,
				 0,  0,  0,  0
			}, {
				 0, 4,  0,  4,
				 6, 2,  6,  2,
				 1, 5,  1,  5,
				 7, 3,  7,  3
			}, {
				 0, 4,  0,  4,
				 6, 2,  6,  2,
				 0, 4,  0,  4,
				 6, 2,  6,  2
			}, {
				 0, 4,  0,  4,
				 4, 0,  4,  0,
				 0, 4,  0,  4,
				 4, 0,  4,  0
			}
		};

int		nDitherPattern2[4][16] = {
			{
				 0,  0,  0,  0,
				 0,  0,  0,  0,
				 0,  0,  0,  0,
				 0,  0,  0,  0
			}, {
				 0,  8,  0,  8,
				12,  4, 12,  4,
				 2, 10,  2, 10,
				14,  6, 14,  6
			}, {
				 0,  8,  0,  8,
				12,  4, 12,  4,
				 0,  8,  0,  8,
				12,  4, 12,  4
			}, {
				 0,  8,  0,  8,
				 8,  0,  8,  0,
				 0,  8,  0,  8,
				 8,  0,  8,  0
			}
		};


static bool _convYJK(unsigned char* lpDst, unsigned char* lpSrc, int nWidth, int nHeight, int nAlgo, int nScreenMode, int nDither)
{
	int		u, v, i;
	int		r[4],g[4],b[4],y[4];
	int		i0,i1,i2,i3;
	int		yy, j, k;
	unsigned char*	lp;
	int		*pnDither;
	int		m;

	if(nScreenMode == 12){
		m = 0x1F;
		pnDither = (int*)&nDitherPattern[nDither];
	} else {
		m = 0x1E;
		pnDither = (int*)&nDitherPattern2[nDither];
	}

	lpSrc += ((nWidth * 3 + 3) & -4) * (nHeight - 1);

	for(v = 0; v < nHeight; v++){
		lp = lpSrc;
		lpSrc -= (nWidth * 3 + 3) & -4;
		for(u = 0; u < nWidth; u+=4){
			/* conv */
			yy = j = k = 0;
			for(i = 0; i < 4; i++){
				if(u + i < nWidth){
					b[i] = *lp++;
					g[i] = *lp++;
					r[i] = *lp++;
				} else {
					b[i] = g[i] = r[i] = 0;
				}
				y[i] = b[i] / 2 + r[i] / 4 + g[i] / 8;
				yy += y[i];// + pnDither[(v&3)*4+i];
				j  += r[i];
				k  += g[i];
			}

			j  = (j - yy) / 4;
			k  = (k - yy) / 4;

			for(i = 0; i < 4; i++){
				if(nAlgo == 1){
					// �uB = Y * 5 / 4 - J / 2 - K / 4�v���瓾���� Y ���v�Z
					y[i] = (b[i] * 4 + j * 2 + k ) / 5;
				} else if(nAlgo == 2){
					// YJK->RGB �ϊ���Ɉ�ԋ߂��F�ɂȂ� Y �����[�v�ŒT��
					int jj = j / 8;
					int kk = k / 8;
					int min = 0x7FFFFFFF;
					for(int yy = 0; yy < 32; yy++){
						int	rr = yy + jj;
						if(rr < 0) rr = 0;
						if(rr > 31) rr = 31;
						int gg = yy + kk;
						if(gg < 0) gg = 0;
						if(gg > 31) gg = 31;
						int bb = yy * 5 / 4 - jj / 2 - kk / 4;
						if(bb < 0) bb = 0;
						if(bb > 31) bb = 31;
						rr=abs(rr-r[i]/8);
						gg=abs(gg-g[i]/8);
						bb=abs(bb-b[i]/8);
						int sub = rr*rr+gg*gg+bb*bb;
						if(sub < min){
							min = sub;
							y[i] = (yy << 3) | (yy >> 2);
						}
					}
				} else if(nAlgo >= 3){
					//�uR = Y + J,G = Y + K,B = Y * 5 / 4 - J / 2 - K / 4�v�̊e�����狁�܂� Y �����������l���v�Z
					switch(nAlgo){
						case 3:		// ����
							i0 = 1;
							i1 = 1;
							i2 = 1;
							i3 = 3;
							break;

						case 4:		// 8:4:2
							i0 = 8;
							i1 = 4;
							i2 = 2;
							i3 = i0+i1+i2;
							break;

						case 5:
							i3 = (g[i]*153 + r[i]*76 + b[i]*27) / 256;
							i0 = i3 ? r[i]*256/i3 : 256;
							i1 = i3 ? g[i]*256/i3 : 256;
							i2 = i3 ? b[i]*256/i3 : 256;
							i3 = i0+i1+i2;
							break;

						case 6:
							i3 = (g[i] + r[i] + b[i]) / 3;
							i0 = i3 ? r[i]*256/i3 : 256;
							i1 = i3 ? g[i]*256/i3 : 256;
							i2 = i3 ? b[i]*256/i3 : 256;
							i3 = i0+i1+i2;
							break;

						case 7:
							i0 = r[i];
							i1 = g[i];
							i2 = b[i];
							i3 = i0+i1+i2;
							break;
					}
					y[i] = i3 ? ((b[i] * 4 + j * 2 + k)*i2 / 5 + (r[i] - j)*i0 + (g[i] - k)*i1) / i3 : 255;
				}
				y[i] += pnDither[i+(v&3)*4];
				y[i] /= 8;
				if(y[i] < 0) y[i] = 0;
				if(y[i] > 31) y[i] = 31;
			}

			j = j >> 3;
			k = k >> 3;

			/* write */
			*lpDst++ = (unsigned char)(((y[0] & m) << 3) | ( k       & 7));
			*lpDst++ = (unsigned char)(((y[1] & m) << 3) | ((k >> 3) & 7));
			*lpDst++ = (unsigned char)(((y[2] & m) << 3) | ( j       & 7));
			*lpDst++ = (unsigned char)(((y[3] & m) << 3) | ((j >> 3) & 7));
		}
	}
	return true;
}


bool ConvYJK(void* lpDst, void* lpSrc, int nWidth, int nHeight, int nAlgo, int nScreenMode, int nDither)
{
	return _convYJK((unsigned char*)lpDst, (unsigned char*)lpSrc, nWidth, nHeight, nAlgo, nScreenMode, nDither);
}

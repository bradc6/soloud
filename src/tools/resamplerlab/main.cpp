/*
SoLoud audio engine - tool to develop resamplers with
Copyright (c) 2013-2014 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "stb_image_write.h"

#ifndef TAU
#define TAU 6.283185307179586476925286766559f
#endif

unsigned char TFX_AsciiFontdata[12*256] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,// ' '
  0, 12, 30, 30, 30, 12, 12,  0, 12, 12,  0,  0,// '!'
  0,102,102,102, 36,  0,  0,  0,  0,  0,  0,  0,// '"'
  0, 54, 54,127, 54, 54, 54,127, 54, 54,  0,  0,// '#'
 12, 12, 62,  3,  3, 30, 48, 48, 31, 12, 12,  0,// '$'
  0,  0,  0, 35, 51, 24, 12,  6, 51, 49,  0,  0,// '%'
  0, 14, 27, 27, 14, 95,123, 51, 59,110,  0,  0,// '&'
  0, 12, 12, 12,  6,  0,  0,  0,  0,  0,  0,  0,// '''
  0, 48, 24, 12,  6,  6,  6, 12, 24, 48,  0,  0,// '('
  0,  6, 12, 24, 48, 48, 48, 24, 12,  6,  0,  0,// ')'
  0,  0,  0,102, 60,255, 60,102,  0,  0,  0,  0,// '*'
  0,  0,  0, 24, 24,126, 24, 24,  0,  0,  0,  0,// '+'
  0,  0,  0,  0,  0,  0,  0,  0, 28, 28,  6,  0,// ','
  0,  0,  0,  0,  0,127,  0,  0,  0,  0,  0,  0,// '-'
  0,  0,  0,  0,  0,  0,  0,  0, 28, 28,  0,  0,// '.'
  0,  0, 64, 96, 48, 24, 12,  6,  3,  1,  0,  0,// '/'
  0, 62, 99,115,123,107,111,103, 99, 62,  0,  0,// '0'
  0,  8, 12, 15, 12, 12, 12, 12, 12, 63,  0,  0,// '1'
  0, 30, 51, 51, 48, 24, 12,  6, 51, 63,  0,  0,// '2'
  0, 30, 51, 48, 48, 28, 48, 48, 51, 30,  0,  0,// '3'
  0, 48, 56, 60, 54, 51,127, 48, 48,120,  0,  0,// '4'
  0, 63,  3,  3,  3, 31, 48, 48, 51, 30,  0,  0,// '5'
  0, 28,  6,  3,  3, 31, 51, 51, 51, 30,  0,  0,// '6'
  0,127, 99, 99, 96, 48, 24, 12, 12, 12,  0,  0,// '7'
  0, 30, 51, 51, 55, 30, 59, 51, 51, 30,  0,  0,// '8'
  0, 30, 51, 51, 51, 62, 24, 24, 12, 14,  0,  0,// '9'
  0,  0,  0, 28, 28,  0,  0, 28, 28,  0,  0,  0,// ':'
  0,  0,  0, 28, 28,  0,  0, 28, 28, 24, 12,  0,// ';'
  0, 48, 24, 12,  6,  3,  6, 12, 24, 48,  0,  0,// '<'
  0,  0,  0,  0,126,  0,126,  0,  0,  0,  0,  0,// '='
  0,  6, 12, 24, 48, 96, 48, 24, 12,  6,  0,  0,// '>'
  0, 30, 51, 48, 24, 12, 12,  0, 12, 12,  0,  0,// '?'
  0, 62, 99, 99,123,123,123,  3,  3, 62,  0,  0,// '@'
  0, 12, 30, 51, 51, 51, 63, 51, 51, 51,  0,  0,// 'A'
  0, 63,102,102,102, 62,102,102,102, 63,  0,  0,// 'B'
  0, 60,102, 99,  3,  3,  3, 99,102, 60,  0,  0,// 'C'
  0, 31, 54,102,102,102,102,102, 54, 31,  0,  0,// 'D'
  0,127, 70,  6, 38, 62, 38,  6, 70,127,  0,  0,// 'E'
  0,127,102, 70, 38, 62, 38,  6,  6, 15,  0,  0,// 'F'
  0, 60,102, 99,  3,  3,115, 99,102,124,  0,  0,// 'G'
  0, 51, 51, 51, 51, 63, 51, 51, 51, 51,  0,  0,// 'H'
  0, 30, 12, 12, 12, 12, 12, 12, 12, 30,  0,  0,// 'I'
  0,120, 48, 48, 48, 48, 51, 51, 51, 30,  0,  0,// 'J'
  0,103,102, 54, 54, 30, 54, 54,102,103,  0,  0,// 'K'
  0, 15,  6,  6,  6,  6, 70,102,102,127,  0,  0,// 'L'
  0, 99,119,127,127,107, 99, 99, 99, 99,  0,  0,// 'M'
  0, 99, 99,103,111,127,123,115, 99, 99,  0,  0,// 'N'
  0, 28, 54, 99, 99, 99, 99, 99, 54, 28,  0,  0,// 'O'
  0, 63,102,102,102, 62,  6,  6,  6, 15,  0,  0,// 'P'
  0, 28, 54, 99, 99, 99,115,123, 62, 48,120,  0,// 'Q'
  0, 63,102,102,102, 62, 54,102,102,103,  0,  0,// 'R'
  0, 30, 51, 51,  3, 14, 24, 51, 51, 30,  0,  0,// 'S'
  0, 63, 45, 12, 12, 12, 12, 12, 12, 30,  0,  0,// 'T'
  0, 51, 51, 51, 51, 51, 51, 51, 51, 30,  0,  0,// 'U'
  0, 51, 51, 51, 51, 51, 51, 51, 30, 12,  0,  0,// 'V'
  0, 99, 99, 99, 99,107,107, 54, 54, 54,  0,  0,// 'W'
  0, 51, 51, 51, 30, 12, 30, 51, 51, 51,  0,  0,// 'X'
  0, 51, 51, 51, 51, 30, 12, 12, 12, 30,  0,  0,// 'Y'
  0,127,115, 25, 24, 12,  6, 70, 99,127,  0,  0,// 'Z'
  0, 60, 12, 12, 12, 12, 12, 12, 12, 60,  0,  0,// '['
  0,  0,  1,  3,  6, 12, 24, 48, 96, 64,  0,  0,// '\'
  0, 60, 48, 48, 48, 48, 48, 48, 48, 60,  0,  0,// ']'
  8, 28, 54, 99,  0,  0,  0,  0,  0,  0,  0,  0,// '^'
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  0,// '_'
 12, 12, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,// '`'
  0,  0,  0,  0, 30, 48, 62, 51, 51,110,  0,  0,// 'a'
  0,  7,  6,  6, 62,102,102,102,102, 59,  0,  0,// 'b'
  0,  0,  0,  0, 30, 51,  3,  3, 51, 30,  0,  0,// 'c'
  0, 56, 48, 48, 62, 51, 51, 51, 51,110,  0,  0,// 'd'
  0,  0,  0,  0, 30, 51, 63,  3, 51, 30,  0,  0,// 'e'
  0, 28, 54,  6,  6, 31,  6,  6,  6, 15,  0,  0,// 'f'
  0,  0,  0,  0,110, 51, 51, 51, 62, 48, 51, 30,// 'g'
  0,  7,  6,  6, 54,110,102,102,102,103,  0,  0,// 'h'
  0, 24, 24,  0, 30, 24, 24, 24, 24,126,  0,  0,// 'i'
  0, 48, 48,  0, 60, 48, 48, 48, 48, 51, 51, 30,// 'j'
  0,  7,  6,  6,102, 54, 30, 54,102,103,  0,  0,// 'k'
  0, 30, 24, 24, 24, 24, 24, 24, 24,126,  0,  0,// 'l'
  0,  0,  0,  0, 63,107,107,107,107, 99,  0,  0,// 'm'
  0,  0,  0,  0, 31, 51, 51, 51, 51, 51,  0,  0,// 'n'
  0,  0,  0,  0, 30, 51, 51, 51, 51, 30,  0,  0,// 'o'
  0,  0,  0,  0, 59,102,102,102,102, 62,  6, 15,// 'p'
  0,  0,  0,  0,110, 51, 51, 51, 51, 62, 48,120,// 'q'
  0,  0,  0,  0, 55,118,110,  6,  6, 15,  0,  0,// 'r'
  0,  0,  0,  0, 30, 51,  6, 24, 51, 30,  0,  0,// 's'
  0,  0,  4,  6, 63,  6,  6,  6, 54, 28,  0,  0,// 't'
  0,  0,  0,  0, 51, 51, 51, 51, 51,110,  0,  0,// 'u'
  0,  0,  0,  0, 51, 51, 51, 51, 30, 12,  0,  0,// 'v'
  0,  0,  0,  0, 99, 99,107,107, 54, 54,  0,  0,// 'w'
  0,  0,  0,  0, 99, 54, 28, 28, 54, 99,  0,  0,// 'x'
  0,  0,  0,  0,102,102,102,102, 60, 48, 24, 15,// 'y'
  0,  0,  0,  0, 63, 49, 24,  6, 35, 63,  0,  0,// 'z'
  0, 56, 12, 12,  6,  3,  6, 12, 12, 56,  0,  0,// '{'
  0, 24, 24, 24, 24,  0, 24, 24, 24, 24,  0,  0,// '|'
  0,  7, 12, 12, 24, 48, 24, 12, 12,  7,  0,  0,// '}'
  0,206, 91,115,  0,  0,  0,  0,  0,  0,  0,  0,// '~'
};

// The pixel data is 8x12 bits, so each glyph is 12 bytes.
int ispixel(char ch, int x, int y)
{
  return (TFX_AsciiFontdata[(ch - 32) * 12 + y] & (1 << x)) != 0;
}

void drawchar(int aChar, int aX, int aY, int *aBitmap, int aBitmapWidth, int aColor)
{
	int i, j;
	for (i = 0; i < 12; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (ispixel(aChar, j, i))
			{
				aBitmap[(aY+i)*aBitmapWidth+aX+j] = aColor;
			}
		}
	}
}

void drawstring(const char *aString, int aX, int aY, int *aBitmap, int aBitmapWidth, int aColor)
{
	while (*aString)
	{
		drawchar(*aString, aX, aY, aBitmap, aBitmapWidth, aColor);
		aX += 8;
		if (aX+8 > aBitmapWidth)
			return;
		aString++;
	}
}

#define SAMPLE_GRANULARITY 512

void resample_pointsample(float *aSrc,
                          float *aSrc1,
                          float *aDst,
                          int aSrcOffset,
                          int aDstSampleCount,
                          float aSrcSamplerate,
                          float aDstSamplerate,
                          int aStepFixed)
{
  int i;
  int pos = aSrcOffset;

  for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
  {
    int p = pos >> 16;
    aDst[i] = aSrc[p];
  }
}

void resample_linear(float *aSrc,
                     float *aSrc1,
                     float *aDst,
                     int aSrcOffset,
                     int aDstSampleCount,
                     float aSrcSamplerate,
                     float aDstSamplerate,
                     int aStepFixed)
{
  int i;
  int pos = aSrcOffset;

  for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
  {
    int p = pos >> 16;
    int f = pos & 0xffff;
#ifdef _DEBUG

    if (p >= SAMPLE_GRANULARITY || p < 0)
    {
      // This should never actually happen
      p = SAMPLE_GRANULARITY - 1;
    }

#endif
    float s1 = aSrc1[SAMPLE_GRANULARITY - 1];

    float s2 = aSrc[p];

    if (p != 0)
    {
      s1 = aSrc[p-1];
    }

    aDst[i] = s1 + (s2 - s1) * f * (1 / 65536.0f);
  }
}

void plot_diff(const char *aFilename, int aSampleCount, int aHeight, float *aSrc1, float *aSrc2, int aColor1, int aColor2, int aBgColor, int aGridColor)
{
	int *bitmap = new int[aSampleCount * aHeight];
	int i;
	for (i = 0; i < aSampleCount * aHeight; i++)
	{
		bitmap[i] = aBgColor;
	}

	for (i = 0; i < aSampleCount; i++)
	{
		bitmap[(aHeight / 2) * aSampleCount + i] = aGridColor;

		if (i & 1)
		{
			bitmap[((aHeight * 1) / 4) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 3) / 4) * aSampleCount + i] = aGridColor;
		}

		if ((i & 3) == 0)
		{
			bitmap[((aHeight * 1) / 8) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 3) / 8) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 5) / 8) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 7) / 8) * aSampleCount + i] = aGridColor;
		}

		if ((i & 7) == 0)
		{
			bitmap[((aHeight * 1) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 3) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 5) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 7) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 9) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 11) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 13) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 15) / 16) * aSampleCount + i] = aGridColor;
		}
	}

	for (i = 0; i < aHeight; i++)
	{
		bitmap[i * aSampleCount + aSampleCount / 2] = aGridColor;

		if (i & 1)
		{
			bitmap[i * aSampleCount + (aSampleCount * 1) / 4] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 3) / 4] = aGridColor;
		}

		if ((i & 3) == 0)
		{
			bitmap[i * aSampleCount + (aSampleCount * 1) / 8] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 3) / 8] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 5) / 8] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 7) / 8] = aGridColor;
		}

		if ((i & 7) == 0)
		{
			bitmap[i * aSampleCount + (aSampleCount * 1) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 3) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 5) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 7) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 9) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 11) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 13) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 15) / 16] = aGridColor;
		}
	}

	for (i = 0; i < aSampleCount-1; i++)
	{
		if (aSrc1[i] > -2 && aSrc1[i] < 2 && aSrc1[i+1] > -2 && aSrc1[i+1] < 2)
		{
			float v1 = 0.5f - (aSrc1[i] + 1) / 4 + 0.25f;
			float v2 = 0.5f - (aSrc1[i+1] + 1) / 4 + 0.25f;
			v1 *= aHeight;
			v2 *= aHeight;
			if (v1 > v2)
			{
				float t = v1;
				v1 = v2;
				v2 = t;
			}
			float j;
			for (j = v1; j <= v2; j++)
			{
				bitmap[(int)floor(j) * aSampleCount + i] = aColor1;
			}
		}

		if (aSrc2[i] > -2 && aSrc2[i] < 2 && aSrc2[i+1] > -2 && aSrc2[i+1] < 2)
		{
			float v1 = 0.5f - (aSrc2[i] + 1) / 4 + 0.25f;
			float v2 = 0.5f - (aSrc2[i+1] + 1) / 4 + 0.25f;
			v1 *= aHeight;
			v2 *= aHeight;
			if (v1 > v2)
			{
				float t = v1;
				v1 = v2;
				v2 = t;
			}
			float j;
			for (j = v1; j <= v2; j++)
			{
				bitmap[(int)floor(j) * aSampleCount + i] = aColor2;
			}
		}
	}

	drawstring("SoLoud Resampler Lab - http://soloud-audio.com", 0, 0, bitmap, aSampleCount, 0xff000000);
	drawstring(aFilename, 0, 12, bitmap, aSampleCount, 0xff000000);
	char tempstr[1024];

	/*
	float maxdiff = 0;
	float diffsum = 0;
	for (i = 0; i < aSampleCount/2; i++)
	{
		float diff = fabs(aSrc1[i+aSampleCount/4]-aSrc2[i+aSampleCount/4]);
		if (diff > maxdiff) maxdiff = diff;
		diffsum += diff;
	}
	sprintf(tempstr, "Avg diff:%3.7f", diffsum / aSampleCount);
	drawstring(tempstr, 0, aHeight-12*2, bitmap, aSampleCount, 0xff000000);
	sprintf(tempstr, "Max diff:%3.7f", maxdiff);
	drawstring(tempstr, 0, aHeight-12*1, bitmap, aSampleCount, 0xff000000);
	*/

	float maxdiff_d = 0;
	float diffsum_d = 0;
	for (i = 0; i < aSampleCount/2; i++)
	{
		float d1 = aSrc1[i+aSampleCount/4 + 1] - aSrc1[i+aSampleCount/4];
		float d2 = aSrc2[i+aSampleCount/4 + 1] - aSrc2[i+aSampleCount/4];
		float diff = fabs(d1 - d2);
		if (diff > maxdiff_d) maxdiff_d = diff;
		diffsum_d += diff;
	}
	
	sprintf(tempstr, "Avg d diff:%3.7f", diffsum_d / aSampleCount);
	drawstring(tempstr, aSampleCount/2, aHeight-12*2, bitmap, aSampleCount, 0xff000000);
	sprintf(tempstr, "Max d diff:%3.7f", maxdiff_d);
	drawstring(tempstr, aSampleCount/2, aHeight-12*1, bitmap, aSampleCount, 0xff000000);


	stbi_write_png(aFilename, aSampleCount, aHeight, 4, bitmap, aSampleCount * 4);
	delete[] bitmap;
}

float saw(float v)
{
	float t = v / TAU;
	t = t - floor(t);
	return (float)(t-0.5)*2;
}

float square(float v)
{
	float t = v / TAU;
	t = t - floor(t);
	if (t > 0.5) return 1;
	return -1;
}

void upsampletest(int aResampler, int aFunction, float aMultiplier, FILE *aIndexf)
{
	float *a, *b, *temp;
	int i;
	int src_samples = (int)floor(512/aMultiplier)+1;

	a = new float[512];
	b = new float[512];
	temp = new float[src_samples];
	
	char *func = "";
	char *samp = "";

	switch (aFunction)
	{
	//case 0:
	default:
		func = "sin";
		for (i = 0; i < 512; i++)
		{
			a[i] = sin(i/256.0f * TAU);
		}
		for (i = 0; i < src_samples; i++)
		{
			temp[i] = sin(i/256.0f * TAU * aMultiplier);
		}
		break;
	case 1:
		func = "saw";
		for (i = 0; i < 512; i++)
		{
			a[i] = saw(i/256.0f * TAU);
		}
		for (i = 0; i < src_samples; i++)
		{
			temp[i] = saw(i/256.0f * TAU * aMultiplier);
		}
		break;
	case 2:
		func = "sqr";
		for (i = 0; i < 512; i++)
		{
			a[i] = square(i/256.0f * TAU);
		}
		for (i = 0; i < src_samples; i++)
		{
			temp[i] = square(i/256.0f * TAU * aMultiplier);
		}
		break;
	}

	switch (aResampler)
	{
	//case 0:
	default:
		samp = "point";
		resample_pointsample(temp,temp,b,0,512,44100/aMultiplier,44100,(int)floor(65536/aMultiplier));
		break;
	case 1:
		samp = "linear";
		resample_linear(temp,temp,b,0,512,44100/aMultiplier,44100,(int)floor(65536/aMultiplier));
		break;
	}

	char tempstr[1024];
	if (aMultiplier >= 1)
	{
		sprintf(tempstr, "%s_%s_%dx.png", samp, func, (int)aMultiplier);
	}
	else
	{
		sprintf(tempstr, "%s_%s_0_%02dx.png", samp, func, (int)aMultiplier*100);
	}

	plot_diff(tempstr, 512, 256, a, b, 0xff0000ff, 0xffff0000, 0xffffffff, 0xffcccccc);

	fprintf(aIndexf, "<img src=\"%s\">&nbsp;", tempstr);

	delete[] a;
	delete[] b;
	delete[] temp;
}

int main(int parc, char ** pars)
{
	FILE * indexf = fopen("index.html", "w");
	fprintf(indexf,
		"<html>\n<body>\n");
	int func, samp, k;
	for (func = 0; func < 3; func++)
	{
		for (k = 0; k < 5; k++)
		{
			for (samp = 0; samp < 2; samp++)
			{
				upsampletest(samp, func, (float)(k * k * 3 + 2), indexf);
			}
			fprintf(indexf, "<br><br>\n");
		}
	}
	fprintf(indexf,"</body>\n</html>\n");
	fclose(indexf);
}
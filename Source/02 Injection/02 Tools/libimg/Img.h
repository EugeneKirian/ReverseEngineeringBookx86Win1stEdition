/*
Copyright (c) 2024 Eugene Kirian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <windows.h>

#define IMG_MAGIC                   0x474D4949 /* IIMG */
#define IMG_VERSION                 1

#define IMG_OK                      0
#define IMG_ERROR                   1
#define IMG_INVALID_DATA            2
#define IMG_OUTOFMEMORY             3
#define IMG_NO_FILE                 4
#define IMG_INVALID_FILE            5
#define IMG_UNABLE_TO_READ_FILE     6
#define IMG_UNABLE_TO_WRITE_FILE    7

#define IMG_FORMAT_RGB565           565

typedef struct ImgHead
{
    DWORD Magic;
    DWORD Version;
    DWORD Size;
} IMGHEAD, * LPIMGHEAD;

typedef struct ImgInfo
{
    DWORD Format;
    DWORD Width;
    DWORD Height;
    DWORD Stride;
} IMGINFO, LPIMGINFO;

typedef struct Img
{
    IMGHEAD Header;
    IMGINFO Info;
} IMG, * LPIMG;

int CreateImage(LPBITMAPFILEHEADER bmp, LPIMG* img);
int ReleaseImage(LPIMG img);
int CreateBitmap(LPIMG img, LPBITMAPFILEHEADER* bmp);
int ReleaseBitmap(LPBITMAPFILEHEADER bmp);
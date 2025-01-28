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

#include "Img.h"

int CreateImage(DWORD width, DWORD height, DWORD format, LPIMG* img)
{
    if (width == 0 || height == 0
        || format != IMG_FORMAT_RGB565 || img == NULL) { return IMG_ERROR; }

    CONST DWORD stride = (((width * (16 >> 3)) + 31) & ~31); // 16 bits per pixel
    CONST DWORD pixels = stride * height;

    CONST DWORD size = pixels + sizeof(IMGHEAD) + sizeof(IMGINFO);

    LPIMG image = (LPIMG)malloc(size);
    if (image == NULL) { return IMG_OUTOFMEMORY; }

    ZeroMemory(image, size);

    image->Header.Magic = IMG_MAGIC;
    image->Header.Version = IMG_VERSION;
    image->Header.Size = size;

    image->Info.Format = format;
    image->Info.Width = width;
    image->Info.Height = height;
    image->Info.Stride = stride;

    *img = image;

    return IMG_OK;
}

VOID Convert24BitBitmap(LPBITMAPFILEHEADER bmp, LPIMG img)
{
    LPBITMAPINFO info = (LPBITMAPINFO)((LPBYTE)bmp + sizeof(BITMAPFILEHEADER));

    // Assume this is a normal bitmap, meaning a bottom-up bitmap
    // Therefore, move all the way to the first scan line (last in the file)
    CONST DWORD stride = (((info->bmiHeader.biWidth * info->bmiHeader.biBitCount) + 31) & ~31) >> 3;
    RGBTRIPLE* pixels = (RGBTRIPLE*)((LPBYTE)bmp + bmp->bfOffBits
        + stride * (info->bmiHeader.biHeight - 1));

    RGBTRIPLE* src = pixels;
    LPWORD dst = (LPWORD)((LPBYTE)img + sizeof(IMGHEAD) + sizeof(IMGINFO));
    for (LONG h = info->bmiHeader.biHeight - 1; h >= 0; h--)
    {
        for (LONG w = 0; w < info->bmiHeader.biWidth; w++)
        {
            CONST RGBTRIPLE input = src[w];
            
            dst[w] = (WORD)(((input.rgbtRed >> 3) << 11)
                | ((input.rgbtGreen >> 2) << 5) | (input.rgbtBlue >> 3));
        }

        src = (RGBTRIPLE*)((LPBYTE)src - stride);
        dst = (LPWORD)((LPBYTE)dst + img->Info.Stride);
    }
}


VOID Convert32BitBitmap(LPBITMAPFILEHEADER bmp, LPIMG img)
{
    LPBITMAPINFO info = (LPBITMAPINFO)((LPBYTE)bmp + sizeof(BITMAPFILEHEADER));

    // Assume this is a normal bitmap, meaning a bottom-up bitmap
    // Therefore, move all the way to the first scan line (last in the file)
    CONST DWORD stride = (((info->bmiHeader.biWidth * info->bmiHeader.biBitCount) + 31) & ~31) >> 3;
    LPDWORD pixels = (LPDWORD)((LPBYTE)bmp + bmp->bfOffBits
        + stride * (info->bmiHeader.biHeight - 1));

    LPDWORD src = pixels;
    LPWORD dst = (LPWORD)((LPBYTE)img + sizeof(IMGHEAD) + sizeof(IMGINFO));
    for (LONG h = info->bmiHeader.biHeight - 1; h >= 0; h--)
    {
        for (LONG w = 0; w < info->bmiHeader.biWidth; w++)
        {
            CONST DWORD input = src[w];
            
            CONST WORD r = (WORD)(((input & 0x00FF0000) >> 16) >> 3);
            CONST WORD g = (WORD)(((input & 0x0000FF00) >> 8) >> 2);
            CONST WORD b = (WORD)(((input & 0x000000FF) >> 0) >> 3);

            dst[w] = (WORD)((r << 11) | (g << 5) | (b));
        }

        src = (LPDWORD)((LPBYTE)src - stride);
        dst = (LPWORD)((LPBYTE)dst + img->Info.Stride);
    }
}

int CreateImage(LPBITMAPFILEHEADER bmp, LPIMG* img)
{
    if (bmp == NULL || img == NULL) { return IMG_ERROR; }
    if (bmp->bfType != 0x4D42) { return IMG_INVALID_DATA; }

    LPBITMAPINFO info = (LPBITMAPINFO)((LPBYTE)bmp + sizeof(BITMAPFILEHEADER));
    
    if (info->bmiHeader.biWidth == 0
        || info->bmiHeader.biHeight == 0) { return IMG_INVALID_DATA; }

    if (info->bmiHeader.biBitCount != 24
        || info->bmiHeader.biCompression != BI_RGB)
    {
        if (info->bmiHeader.biBitCount == 32
            && info->bmiHeader.biCompression == BI_BITFIELDS)
        {
            LPDWORD masks = (LPDWORD)info->bmiColors;

            CONST DWORD red = masks[0];
            CONST DWORD green = masks[1];
            CONST DWORD blue = masks[2];

            if (red != 0x00FF0000 || green != 0x0000FF00 || blue != 0x000000FF) { return IMG_INVALID_DATA; }
        }
        else { return IMG_INVALID_DATA; }
    }

    int result = IMG_OK;
    LPIMG image = NULL;
    if ((result = CreateImage(info->bmiHeader.biWidth,
        info->bmiHeader.biHeight, IMG_FORMAT_RGB565, &image)) != IMG_OK)
    {
        return result;
    }
    
    if (info->bmiHeader.biBitCount == 24) { Convert24BitBitmap(bmp, image); }
    else if (info->bmiHeader.biBitCount == 32) { Convert32BitBitmap(bmp, image); }
    else
    {
        ReleaseImage(image);
        return IMG_INVALID_DATA;
    }

    *img = image;

    return IMG_OK;
}

int ReleaseImage(LPIMG img)
{
    if (img == NULL) { return IMG_ERROR; }

    free(img);

    return IMG_OK;
}

int CreateBitmap(LPIMG img, LPBITMAPFILEHEADER* bmp)
{
    if (img == NULL || bmp == NULL) { return IMG_ERROR; }

    CONST DWORD stride = (((img->Info.Width * 32) + 31) & ~31) >> 3;
    CONST DWORD offset = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 3 * sizeof(DWORD);
    CONST DWORD size = offset + stride * img->Info.Height;

    LPBITMAPFILEHEADER image = (LPBITMAPFILEHEADER)malloc(size);

    if (image == NULL) { return IMG_OUTOFMEMORY; }

    ZeroMemory(image, size);

    image->bfType = 0x4D42;
    image->bfSize = size;
    image->bfOffBits = offset;

    LPBITMAPINFO info = (LPBITMAPINFO)((LPBYTE)image + sizeof(BITMAPFILEHEADER));

    info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info->bmiHeader.biWidth = img->Info.Width;
    info->bmiHeader.biHeight = img->Info.Height;
    info->bmiHeader.biPlanes = 1;
    info->bmiHeader.biBitCount = 32;
    info->bmiHeader.biCompression = BI_BITFIELDS;
    info->bmiHeader.biSizeImage = stride * img->Info.Height;
    info->bmiHeader.biXPelsPerMeter = 2834;
    info->bmiHeader.biYPelsPerMeter = 2834;
    info->bmiHeader.biClrUsed = 0;
    info->bmiHeader.biClrImportant = 0;

    {
        LPDWORD masks = (LPDWORD)info->bmiColors;

        masks[0] = 0x00FF0000;
        masks[1] = 0x0000FF00;
        masks[2] = 0x000000FF;
    }

    // Convert pixels
    LPWORD src = (LPWORD)((LPBYTE)img + sizeof(IMGHEAD) + sizeof(IMGINFO));
    LPDWORD dst = (LPDWORD)((LPBYTE)image + offset + stride * (img->Info.Height - 1));
    for (LONG h = (LONG)img->Info.Height - 1; h >= 0; h--)
    {
        for (LONG w = 0; w < (LONG)img->Info.Width; w++)
        {
            CONST WORD input = src[w];
            
            CONST DWORD r = ((DWORD)((input & 0xF800) >> 11) << 3);
            CONST DWORD g = ((DWORD)((input & 0x07E0) >> 5) << 2);
            CONST DWORD b = ((DWORD)((input & 0x001F) >> 0) << 3);

            dst[w] = (DWORD)((r << 16) | (g << 8) | (b));
        }

        src = (LPWORD)((LPBYTE)src + img->Info.Stride);
        dst = (LPDWORD)((LPBYTE)dst - stride);
    }

    *bmp = image;

    return IMG_OK;
}

int ReleaseBitmap(LPBITMAPFILEHEADER bmp)
{
    if (bmp == NULL) { return IMG_ERROR; }

    free(bmp);

    return IMG_OK;
}
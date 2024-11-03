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

#include <Img.h>
#include <stdio.h>

const char* get_error_message(int err)
{
    switch (err)
    {
    case IMG_OK: return "IMG_OK";
    case IMG_ERROR: return "IMG_ERROR";
    case IMG_INVALID_DATA: return "IMG_INVALID_DATA";
    case IMG_OUTOFMEMORY: return "IMG_OUTOFMEMORY";
    case IMG_NO_FILE: return "IMG_NO_FILE";
    case IMG_INVALID_FILE: return "IMG_INVALID_FILE";
    case IMG_UNABLE_TO_READ_FILE: return "IMG_UNABLE_TO_READ_FILE";
    case IMG_UNABLE_TO_WRITE_FILE: return "IMG_UNABLE_TO_WRITE_FILE";
    }

    return "UNKNOWN";
}

int read_image(const char* path, LPIMG* img)
{
    if (path == NULL) { return IMG_NO_FILE; }
    if (img == NULL) { return IMG_ERROR; }

    HANDLE handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE) { return IMG_NO_FILE; }

    CONST DWORD size = GetFileSize(handle, NULL);

    if (size < (sizeof(IMG)))
    {
        CloseHandle(handle);
        return IMG_INVALID_FILE;
    }

    LPIMG image = (LPIMG)malloc(size);

    if (image == NULL)
    {
        CloseHandle(handle);
        return IMG_OUTOFMEMORY;
    }

    ZeroMemory(image, size);

    {
        DWORD bytes = 0;
        if (!ReadFile(handle, image, size, &bytes, NULL) || size != bytes)
        {
            free(image);
            CloseHandle(handle);
            return IMG_UNABLE_TO_READ_FILE;
        }
    }

    CloseHandle(handle);

    *img = image;

    return IMG_OK;
}

int save_bmp(LPBITMAPFILEHEADER bmp, const char* path)
{
    if (bmp == NULL || path == NULL) { return IMG_ERROR; }

    HANDLE handle = CreateFileA(path, GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE) { return IMG_UNABLE_TO_WRITE_FILE; }

    DWORD bytes = 0;
    if (!WriteFile(handle, bmp, bmp->bfSize, &bytes, NULL)
        || bytes != bmp->bfSize)
    {
        CloseHandle(handle);
        DeleteFileA(path);
        return IMG_UNABLE_TO_WRITE_FILE;
    }

    CloseHandle(handle);

    return IMG_OK;
}

int convert_file(const char* input, const char* output)
{
    int result = IMG_OK;
    LPIMG image = NULL;
    if ((result = read_image(input, &image)) != IMG_OK)
    {
        printf("Error while reading file %s: %s (%d).\r\n",
            input, get_error_message(result), result);

        return result;
    }

    LPBITMAPFILEHEADER bmp = NULL;
    if ((result = CreateBitmap(image, &bmp)) != IMG_OK)
    {
        ReleaseImage(image);

        printf("Error while converting file %s: %s (%d).\r\n",
            input, get_error_message(result), result);

        return result;
    }

    ReleaseImage(image);

    if ((result = save_bmp(bmp, output)) != IMG_OK)
    {
        free(bmp);

        printf("Error while saving file %s: %s (%d).\r\n",
            output, get_error_message(result), result);

        return result;
    }

    free(bmp);

    return IMG_OK;
}

int main(int argc, char* argv[])
{
    return convert_file(argv[1], argv[2]);
}
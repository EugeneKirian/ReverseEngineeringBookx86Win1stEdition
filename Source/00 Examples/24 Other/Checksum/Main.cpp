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

#include <zlib.h>
#include <stdio.h>

#include <windows.h>

unsigned long calculate_crc(const void* data, const unsigned long length)
{
    unsigned long  crc = crc32(0L, Z_NULL, 0);

    crc = crc32(crc, (const unsigned char*)data, length);

    return crc;
}

void calculate_checksum_for_file(const char* path)
{
    HANDLE handle = CreateFileA(path, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Unable to open %s\n", path);

        return;
    }

    DWORD size = GetFileSize(handle, NULL);

    if (size == INVALID_FILE_SIZE)
    {
        printf("Unable to get file size of %s\n", path);

        CloseHandle(handle);

        return;
    }

    if (size == 0)
    {
        printf("File %s is zero bytes long. Skipping...\n", path);

        CloseHandle(handle);

        return;
    }

    void* data = malloc(size);

    if (data == NULL)
    {
        printf("Unable to allocate memory for %d bytes for %s\n", size, path);

        CloseHandle(handle);

        return;
    }

    DWORD bytes = 0;
    if (!ReadFile(handle, data, size, &bytes, NULL) || bytes != size)
    {
        printf("Unable to read file %s\n", size, path);

        CloseHandle(handle);

        return;
    }

    CloseHandle(handle);

    const unsigned long crc = calculate_crc(data, size);

    printf("Checksum for %s is %x\n", path, crc);

    free(data);
}

int main(int argc, char** argv)
{
    char path[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, path);

    printf("Scanning %s\n", path);

    strcat_s(path, MAX_PATH, "\\*.exe");

    HANDLE handle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA ffd;
    ZeroMemory(&ffd, sizeof(ffd));

    handle = FindFirstFileA(path, &ffd);

    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Unable to start file discovery for %s\n", path);

        return -1;
    }

    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            calculate_checksum_for_file(ffd.cFileName);
        }
    } while (FindNextFileA(handle, &ffd) != 0);

    FindClose(handle);

    return 0;
}
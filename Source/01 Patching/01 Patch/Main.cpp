/*
Copyright (c) 2025 Eugene Kirian

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

#define _CRT_SECURE_NO_WARNINGS

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <imagehlp.h>

#pragma comment(lib, "imagehlp.lib")

#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4996)

typedef unsigned char byte;

#define CUBE_FILE_SIZE  11776

#define CUBE_ORIGINAL_CHECKSUM  0xb1db
#define CUBE_PATCHED_CHECKSUM   0xb8db

#define PATTERN_LENGTH      20

const byte original_pattern[PATTERN_LENGTH] = { 0x68, 0x00, 0x00, 0x00, 0x80, 0x68, 0x00, 0x00, 0x00, 0x80, 0x68, 0x00, 0x00, 0xc8, 0x00, 0x68, 0x50, 0x31, 0x40, 0x00 };
const byte correct_pattern[PATTERN_LENGTH]  = { 0x68, 0x00, 0x00, 0x00, 0x80, 0x68, 0x00, 0x00, 0x00, 0x80, 0x68, 0x00, 0x00, 0xcf, 0x00, 0x68, 0x50, 0x31, 0x40, 0x00 };

int patch_file(byte* ptr, const DWORD size)
{
    for (unsigned x = 0; x < size - PATTERN_LENGTH; x++)
    {
        if (memcmp(&ptr[x], original_pattern, PATTERN_LENGTH) == 0)
        {
            for (unsigned xx = 0; xx < PATTERN_LENGTH; xx++)
            {
                ptr[x + xx] = correct_pattern[xx];
            }            
            
            return TRUE;
        }
    }

    return FALSE;
}

int process_file(HANDLE file, const DWORD size, const byte checksum)
{
    HANDLE map = CreateFileMappingA(file, NULL, PAGE_READWRITE, 0, 0, NULL);

    if (map == NULL)
    {
        int err = GetLastError();
        printf("Unable to map file to memory.\n");
        return EXIT_FAILURE;
    }

    void* view = MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (view == NULL)
    {
        printf("Unable to create file mapping view.\n");
        CloseHandle(map);
        return EXIT_FAILURE;
    }

    DWORD actual = 0, correct = 0;
    PIMAGE_NT_HEADERS header = CheckSumMappedFile(view, size, &actual, &correct);

    if (header == NULL)
    {
        printf("Unable to obtain executable header and checksum.\n");
        UnmapViewOfFile(view);
        CloseHandle(map);
        return EXIT_FAILURE;
    }

    if(header->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
    {
        // Verify checksum, if needed (not really).
        if (checksum)
        {
            const DWORD value = ((IMAGE_NT_HEADERS32*)header)->OptionalHeader.CheckSum;

            if (value != CUBE_ORIGINAL_CHECKSUM)
            {
                if (value == CUBE_PATCHED_CHECKSUM)
                {
                    printf("File is already patched.\n");
                    UnmapViewOfFile(view);
                    CloseHandle(map);
                    return EXIT_FAILURE;
                }
                else
                {
                    printf("File is not a Cube.exe!\n");
                    UnmapViewOfFile(view);
                    CloseHandle(map);
                    return EXIT_FAILURE;
                }
            }
        }

        // Update the code/data.
        if(!patch_file((byte*)view, size))
        {
            printf("Unable to patch file, data pattern not found!\n");
            UnmapViewOfFile(view);
            CloseHandle(map);
            return EXIT_FAILURE;
        }

        // Update the checksum.
        ((IMAGE_NT_HEADERS32*)header)->OptionalHeader.CheckSum = correct;

        // Save the file.
        FlushViewOfFile(view, 0);
        FlushFileBuffers(file);

        UnmapViewOfFile(view);
        CloseHandle(map);

        return EXIT_SUCCESS;
    }
    else
    {
        printf("File is not a valid Portable Executable file.\n");
        UnmapViewOfFile(view);
        CloseHandle(map);
    }
    
    return EXIT_FAILURE;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: patch <file>\n");
        return EXIT_FAILURE;
    }

    HANDLE file = CreateFileA(argv[1], GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE)
    {
        printf("Unable to open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    const DWORD size = GetFileSize(file, NULL);

    if (size != CUBE_FILE_SIZE) {
        printf("Invalid file size %d, expected %d", size, CUBE_FILE_SIZE);
        CloseHandle(file);
        return EXIT_FAILURE;
    }

    const int result = process_file(file, size, FALSE);

    CloseHandle(file);

    return result;
}
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

#include "File.h"

#include <typeinfo.h>
#include <stdio.h>

int stack_class_example(const char* path)
{
    RealFile file;

    if (!file.Open(path))
    {
        return INVALID_FILE_SIZE;
    }

    return file.Size();
}

void stack_class_examples()
{
    printf("Size of %s is %d\n", typeid(RealFile).name(), sizeof(RealFile));

    static char* files[] =
    {
        "C:\\Windows\\explorer.exe",
        "C:\\Windows\\system32\\notepad.exe",
        "C:\\Windows\\system32\\calc.exe"
    };

    for (int i = 0; i < _countof(files); i++)
    {
        printf("File size of %s is %d\n", files[i],
            stack_class_example(files[i]));
    }

    printf("\n--------\n");
}

RealFile* allocated_class_example(const char* path)
{
    RealFile* file = new RealFile();

    file->Open(path);

    return file;
}

void allocated_class_examples()
{
    printf("Size of %s is %d\n", typeid(RealFile*).name(), sizeof(RealFile*));

    static char* files[] =
    {
        "C:\\Windows\\explorer.exe",
        "C:\\Windows\\system32\\notepad.exe",
        "C:\\Windows\\system32\\calc.exe"
    };

    for (int i = 0; i < _countof(files); i++)
    {
        RealFile* file = allocated_class_example(files[i]);

        printf("File size of %s is %d\n", files[i], file->Size());

        delete file;
    }

    printf("\n--------\n");
}

#define MAX_STATIC_FILES_COUNT 24
RealFile static_files[MAX_STATIC_FILES_COUNT];

void static_class_examples()
{
    char path[MAX_PATH];
    
    if (GetCurrentDirectoryA(MAX_PATH, path) == 0)
    {
        printf("Unable to acquire current directory.\n");

        return;
    }

    char search[MAX_PATH];
    WIN32_FIND_DATAA fd;

    strcpy_s(search, MAX_PATH, path);
    strcat_s(search, "\\*");

    HANDLE handle = FindFirstFileA(search, &fd);

    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Unable to acquire current directory files.\n");

        return;
    }

    int count = 0;

    do
    {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            if (static_files[count].Name()[0] != NULL)
            {
                static_files[count].Close();
            }

            if (static_files[count].Open(fd.cFileName))
            {
                count++;
            }
        }
    } while (count < MAX_STATIC_FILES_COUNT && FindNextFileA(handle, &fd));

    FindClose(handle);

    printf("First %d files in %s:\n", count, path);

    for (int i = 0; i < count; i++)
    {
        printf("%s\t\t%d\n", static_files[i].Name(), static_files[i].Size());
    }

    printf("\n");
}

int main(int argc, char** argv)
{
    stack_class_examples();
    allocated_class_examples();
    static_class_examples();

    return 0;
}
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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include <strsafe.h>

#define MAX_STRUCTURE_COUNT 64

struct file_system_entity
{
    HANDLE handle;
    char path[MAX_PATH];
    char is_directory;
    FILETIME create_date_time;
    FILETIME last_access_date_time;
};

void nested_structure_copy_example()
{
    file_system_entity entity = { 0 };

    entity.handle = INVALID_HANDLE_VALUE;
    entity.is_directory = TRUE;
    GetSystemDirectoryA(entity.path, MAX_PATH);

    file_system_entity copy = entity;

    printf("Structure Copy\nDirectory: %s, Handle: %d, Flag: %s.\n",
        copy.path, copy.handle, copy.is_directory ? "TRUE" : "FALSE");
}

file_system_entity entities[MAX_STRUCTURE_COUNT];

void get_system_drive_path(char* result)
{
    if (result == NULL) { return; }

    char system[MAX_PATH];
    ZeroMemory(system, MAX_PATH * sizeof(char));

    GetSystemDirectoryA(system, MAX_PATH);

    _splitpath(system, result, NULL, NULL, NULL);
}

void get_system_drive_search_pattern(char* drive)
{
    if (drive == NULL) { return; }

    StringCchCatA(drive, MAX_PATH, "\\*");
}

void extended_file_system_structure_example()
{
    printf("Struct file_system_entity size: %d\n", sizeof(file_system_entity));

    char drive[MAX_PATH];
    ZeroMemory(drive, MAX_PATH * sizeof(char));

    get_system_drive_path(drive);

    if (drive[0] == NULL)
    {
        printf("Unable to get system drive name.\n");

        return;
    }

    get_system_drive_search_pattern(drive);

    HANDLE handle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA ffd;

    handle = FindFirstFileA(drive, &ffd);

    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Unable to start file discovery for %s\n", drive);

        return;
    }

    int iteration = 0;

    do
    {
        StringCchCopyA(entities[iteration].path, MAX_PATH, ffd.cFileName);
        
        entities[iteration].is_directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        entities[iteration].create_date_time = ffd.ftCreationTime;
        entities[iteration].last_access_date_time = ffd.ftLastAccessTime;

        iteration++;
    } while (iteration < MAX_STRUCTURE_COUNT && FindNextFileA(handle, &ffd) != 0);

    FindClose(handle);

    for (int i = 0; i < iteration; i++)
    {
        SYSTEMTIME time;
        FileTimeToSystemTime(&entities[i].create_date_time, &time);

        printf("%s\tCreated: %d-%02d-%02d %02d:%02d:%02d\t\t%s\n",
            entities[i].is_directory ? "<DIR>" : "<FILE>",
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
            entities[i].path);
    }
}

int main(int argc, char** argv)
{
    nested_structure_copy_example();
    extended_file_system_structure_example();

    return 0;
}
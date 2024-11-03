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

struct struct_default_alignement
{
    char c;
    long l;
    unsigned char b[2];
    unsigned char z[3];
    double zz;
};

#pragma pack(push, 1)
struct packed_struct_1_alignement
{
    char c;
    long l;
    unsigned char b[2];
    unsigned char z[3];
    double zz;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct packed_struct_2_alignement
{
    char c;
    long l;
    unsigned char b[2];
    unsigned char z[3];
    double zz;
};
#pragma pack(pop)

#pragma pack(push, 4)
struct packed_struct_4_alignement
{
    char c;
    long l;
    unsigned char b[2];
    unsigned char z[3];
    double zz;
};
#pragma pack(pop)

#pragma pack(push, 8)
struct packed_struct_8_alignement
{
    char c;
    long l;
    unsigned char b[2];
    unsigned char z[3];
    double zz;
};
#pragma pack(pop)

#pragma pack(push, 16)
struct packed_struct_16_alignement
{
    char c;
    long l;
    unsigned char b[2];
    unsigned char z[3];
    double zz;
};
#pragma pack(pop)

void structure_sizes_example()
{
    printf("Struct struct_default_alignement size: %d, offsets of l: %d, b: %d, zz: %d\n",
        sizeof(struct struct_default_alignement), offsetof(struct_default_alignement, l),
        offsetof(struct_default_alignement, b), offsetof(struct_default_alignement, zz));

    printf("Struct packed_struct_1_alignement size: %d, offsets of l: %d, b: %d, zz: %d\n",
        sizeof(packed_struct_1_alignement), offsetof(packed_struct_1_alignement, l),
        offsetof(packed_struct_1_alignement, b), offsetof(packed_struct_1_alignement, zz));

    printf("Struct packed_struct_2_alignement size: %d, offsets of l: %d, b: %d, zz: %d\n",
        sizeof(packed_struct_2_alignement), offsetof(packed_struct_2_alignement, l),
        offsetof(packed_struct_2_alignement, b), offsetof(packed_struct_2_alignement, zz));

    printf("Struct packed_struct_4_alignement size: %d, offsets of l: %d, b: %d, zz: %d\n",
        sizeof(packed_struct_4_alignement), offsetof(packed_struct_4_alignement, l),
        offsetof(packed_struct_4_alignement, b), offsetof(packed_struct_4_alignement, zz));

    printf("Struct packed_struct_8_alignement size: %d, offsets of l: %d, b: %d, zz: %d\n",
        sizeof(packed_struct_8_alignement), offsetof(packed_struct_8_alignement, l),
        offsetof(packed_struct_8_alignement, b), offsetof(packed_struct_8_alignement, zz));

    printf("Struct packed_struct_16_alignement size: %d, offsets of l: %d, b: %d, zz: %d\n",
        sizeof(packed_struct_16_alignement), offsetof(packed_struct_16_alignement, l),
        offsetof(packed_struct_16_alignement, b), offsetof(packed_struct_16_alignement, zz));
}

void simple_structure_example()
{
    SYSTEMTIME time;
    ZeroMemory(&time, sizeof(SYSTEMTIME));

    GetLocalTime(&time);

    printf("Current Time: %d-%02d-%02d %02d:%02d:%02d\n",
        time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
}

void simple_structure_copy_example()
{
    SYSTEMTIME time;
    ZeroMemory(&time, sizeof(SYSTEMTIME));

    GetLocalTime(&time);

    SYSTEMTIME copy = time;

    printf("Copy Time: %d-%02d-%02d %02d:%02d:%02d\n",
        copy.wYear, copy.wMonth, copy.wDay, copy.wHour, copy.wMinute, copy.wSecond);
}

void structure_array_example()
{
    char drives[MAX_PATH];

    const int count = GetLogicalDriveStringsA(MAX_PATH, drives);

    if (count == 0)
    {
        printf("Unable to get available disk volumes.\n");

        return;
    }

    int index = 0;
    int length = 0;

    char drive[MAX_PATH];
    ZeroMemory(drive, MAX_PATH * sizeof(char));

    while (TRUE)
    {
        drive[length] = drives[index];

        length = drives[index] == NULL ? 0 : (length + 1);

        if (length == 0)
        {
            DWORD total_clusters = 0, free_clusters = 0;
            DWORD sectors_in_cluster = 0, bytes_in_sector = 0;

            GetDiskFreeSpaceA(drive, &sectors_in_cluster, &bytes_in_sector,
                &free_clusters, &total_clusters);

            const float free_space_percentage = total_clusters == 0
                ? 100.0f
                : (100.0f * (float)free_clusters / (float)total_clusters);
            
            printf("Drive: %s Free Space: %f%%\n", drive, free_space_percentage);
        }

        if (drives[index] == NULL && drives[index + 1] == NULL) { break; }

        index++;
    }
}

#define MAX_STRUCTURE_COUNT 32

struct file_system_entity
{
    HANDLE handle;
    char path[MAX_PATH];
    unsigned char is_directory;
};

int file_structures_count = 0;
file_system_entity entities[MAX_STRUCTURE_COUNT];

void get_system_drive_path(char* result)
{
    if (result == NULL) { return; }

    char system[MAX_PATH] = { 0 };
    GetSystemDirectoryA(system, MAX_PATH);

    _splitpath(system, result, NULL, NULL, NULL);
}

void get_system_drive_search_pattern(char* drive)
{
    if (drive == NULL) { return; }

    StringCchCatA(drive, MAX_PATH, "\\*");
}

void file_system_structure_example()
{
    printf("Struct file_system_entity size: %d\n", sizeof(file_system_entity));

    char drive[MAX_PATH] = { 0 };

    get_system_drive_path(drive);

    if (drive[0] == NULL)
    {
        printf("Unable to get system drive name.\n");

        return;
    }

    get_system_drive_search_pattern(drive);

    HANDLE handle = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA ffd;
    ZeroMemory(&ffd, sizeof(ffd));

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

        iteration++;
    } while (iteration < MAX_STRUCTURE_COUNT && FindNextFileA(handle, &ffd) != 0);

    FindClose(handle);

    file_structures_count = iteration;

    for (int i = 0; i < iteration; i++)
    {
        printf("%s\t\t%s\n", entities[i].is_directory ? "<DIR>" : "<FILE>", entities[i].path);
    }
}

struct addition_structure
{
    int integer;
    float single;
};

void modify_structure_through_pointer(addition_structure* input)
{
    input->integer++;
    input->single++;
}

void structure_pointer_example()
{
    addition_structure value;

    value.integer = 0;
    value.single = 0.0f;

    printf("Original structure values %d %f\n", value.integer, value.single);

    for (int i = 0; i < 10; i++)
    {
        modify_structure_through_pointer(&value);

        printf("Modified structure values %d %f\n", value.integer, value.single);
    }
}

void structure_pointer_math_example()
{
    printf("Struct Pointer Math\n");

    const file_system_entity* pointer = entities;

    for (int i = 0; i < file_structures_count; i++)
    {
        printf("%s\n", pointer->path);

        pointer++;
    }

    printf("\n");
}

void modify_structure_through_value(addition_structure input)
{
    input.integer++;
    input.single++;
}

void structure_value_example()
{
    addition_structure value;

    value.integer = 0;
    value.single = 0.0f;

    printf("Original structure values %d %f\n", value.integer, value.single);

    for (int i = 0; i < 10; i++)
    {
        modify_structure_through_value(value);

        printf("Modified structure values %d %f\n", value.integer, value.single);
    }
}

int main(int argc, char** argv)
{
    structure_sizes_example();
    simple_structure_example();
    simple_structure_copy_example();
    structure_array_example();
    file_system_structure_example();
    structure_pointer_example();
    structure_pointer_math_example();
    structure_value_example();
    
    return 0;
}
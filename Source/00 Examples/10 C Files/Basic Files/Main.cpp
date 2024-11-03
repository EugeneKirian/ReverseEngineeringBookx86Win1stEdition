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

#include <stdio.h>
#include <string.h>

#include <windows.h>

void read_file_example()
{
    char path[MAX_PATH] = { 0 };

    GetSystemDirectoryA(path, MAX_PATH);

    if (strlen(path) == 0)
    {
        printf("Unable to acquire system directory path.\n");

        return;
    }

    const size_t last_character_index = strlen(path) - 1;

    if (path[last_character_index] != '\\')
    {
        path[last_character_index + 1] = '\\';
        path[last_character_index + 2] = NULL;
    }

    strcat(path, "notepad.exe");

    FILE* notepad = fopen(path, "rb");

    if (notepad == NULL)
    {
        printf("Unable to open file %s\n", path);

        return;
    }

    fseek(notepad, 0L, SEEK_END);

    const int size = ftell(notepad);

    fseek(notepad, 0L, SEEK_SET);

    printf("File size of %s is %d bytes.\n", path, size);

    const size_t read = min(5, size);

    fprintf(stdout, "Reading first %d bytes of the file...\n", read);

    for (size_t i = 0; i < read; i++)
    {
        const char item = fgetc(notepad);

        fprintf(stdout, "%c (0x%x) ", item, (int)item);
    }

    printf("\n");

    fclose(notepad);
}

void write_file_example()
{
    char path[MAX_PATH];

    if (GetTempPathA(MAX_PATH, path) == 0)
    {
        printf("Unable to acquire temp directory path.\n");

        return;
    }

    const size_t last_character_index = strlen(path) - 1;

    if (path[last_character_index] != '\\')
    {
        path[last_character_index + 1] = '\\';
        path[last_character_index + 2] = NULL;
    }

    strcat(path, "basic_files.txt");

    FILE* fp = fopen(path, "wb+");

    if (fp == NULL)
    {
        printf("Unable to open or create file %s\n", path);

        return;
    }
    
    printf("Writing data into file %s\n", path);

    if (fprintf(fp, "Test 123 456 56.0 value\n") == 0)
    {
        printf("Unable to write data into %s\n", path);
    }

    int values[5] = { 1, 2, 3, 4, 5 };
    
    if (fwrite(values, sizeof(int), 5, fp) == 0)
    {
        printf("Unable to write data into %s\n", path);
    }

    fflush(fp);

    fclose(fp);
}

void parse_file_example()
{
    char path[MAX_PATH];

    if (GetTempPathA(MAX_PATH, path) == 0)
    {
        printf("Unable to acquire temp directory path.\n");

        return;
    }

    const size_t last_character_index = strlen(path) - 1;

    if (path[last_character_index] != '\\')
    {
        path[last_character_index + 1] = '\\';
        path[last_character_index + 2] = NULL;
    }

    strcat(path, "basic_files.txt");

    FILE* fp = fopen(path, "rb+");

    if (fp == NULL)
    {
        printf("Unable to open file %s\n", path);

        return;
    }

    char string1[128];
    char string2[128];
    int integer1, integer2;
    float single;

    if (fscanf(fp, "%s %d %d %f %s",
        string1, &integer1, &integer2, &single, string2) == 5)
    {
        printf("Parsed values are %d %d %f %s %s\n", integer1, integer2, single, string1, string2);
    }
    else
    {
        printf("Unable to parse values from the file.\n");
    }

    fclose(fp);
}

int main(int argc, char** argv)
{
    read_file_example();
    write_file_example();
    parse_file_example();

    return 0;
}
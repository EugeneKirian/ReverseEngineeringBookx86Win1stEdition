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

void zeroing_memory_memset_array_example()
{
    int integers[97];

    memset(&integers[2], 0, sizeof(integers) - 2 * sizeof(int));

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", integers[i]);
    }

    printf("\n");
}

void zeroing_memory_memset_struct_example()
{
    BITMAPV4HEADER header;

    printf("Struct Size: %d\n", sizeof(BITMAPV4HEADER));

    printf("Width: %d, Height: %d\n", header.bV4Width, header.bV4Height);

    memset(&header, 0, sizeof(BITMAPV4HEADER));

    printf("Width: %d, Height: %d\n", header.bV4Width, header.bV4Height);
}

void zeroing_memory_zeromemory_array_example()
{
    int integers[97];

    ZeroMemory(&integers[2], sizeof(integers) - 2 * sizeof(int));

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", integers[i]);
    }

    printf("\n");
}

void zeroing_memory_zeromemory_struct_example()
{
    BITMAPV4HEADER header;

    printf("Struct Size: %d\n", sizeof(BITMAPV4HEADER));

    printf("Width: %d, Height: %d\n", header.bV4Width, header.bV4Height);

    ZeroMemory(&header, sizeof(BITMAPV4HEADER));

    printf("Width: %d, Height: %d\n", header.bV4Width, header.bV4Height);
}

void zeroing_memory_examples()
{
    zeroing_memory_memset_array_example();
    zeroing_memory_memset_struct_example();
    zeroing_memory_zeromemory_array_example();
    zeroing_memory_zeromemory_struct_example();

    printf("\n---------\n");
}

void set_memory_array_example()
{
    int integers[9];

    memset(&integers[2], 0xF, 2 * sizeof(int));
    memset(&integers[5], 0xFF, 3 * sizeof(int));

    for (int i = 0; i < 9; i++)
    {
        printf("%d ", integers[i]);
    }

    printf("\n");
}

void set_memory_struct_example()
{
    BITMAPV4HEADER header;

    ZeroMemory(&header, sizeof(header));

    const CIEXYZTRIPLE* triple = &header.bV4Endpoints;

    memset((void*)triple, 0x1F, sizeof(CIEXYZTRIPLE));

    printf("%d, %d, %d\n", triple->ciexyzBlue.ciexyzX, triple->ciexyzBlue.ciexyzY, triple->ciexyzBlue.ciexyzZ);
}

void set_memory_values_examples()
{
    set_memory_array_example();
    set_memory_struct_example();

    printf("\n---------\n");
}

void copy_memory_memcpy_array_example()
{
    int integers[7] = { 1, 2 };

    memcpy(&integers[2], integers, 2 * sizeof(int));

    for (int i = 0; i < 7; i++)
    {
        printf("%d ", integers[i]);
    }

    printf("\n");
}

void copy_memory_memcpy_struct_example()
{
    SYSTEMTIME time, copy;

    ZeroMemory(&copy, sizeof(SYSTEMTIME));

    GetLocalTime(&time);

    memcpy(&copy, &time, sizeof(SYSTEMTIME));

    printf("Copy Time: %d-%02d-%02d %02d:%02d:%02d\n",
        copy.wYear, copy.wMonth, copy.wDay, copy.wHour, copy.wMinute, copy.wSecond);
}

void copy_memory_safe_memcpy_array_example()
{
    int integers[5] = { 5, 7 };

    memcpy_s(&integers[2], 3 * sizeof(int), integers, 2 * sizeof(int));

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", integers[i]);
    }

    printf("\n");
}

void copy_memory_safe_memcpy_struct_example()
{
    SYSTEMTIME time, copy;

    ZeroMemory(&copy, sizeof(SYSTEMTIME));

    GetLocalTime(&time);

    memcpy_s(&copy, 4 * sizeof(WORD), &time, 4 * sizeof(WORD));

    printf("Copy Time: %d-%02d-%02d %02d:%02d:%02d\n",
        copy.wYear, copy.wMonth, copy.wDay, copy.wHour, copy.wMinute, copy.wSecond);
}

void copy_memory_copymemory_array_example()
{
    int integers[6] = { 9, 10, 11 };

    CopyMemory(&integers[2], integers, 2 * sizeof(int));

    for (int i = 0; i < 6; i++)
    {
        printf("%d ", integers[i]);
    }

    printf("\n");
}

void copy_memory_copymemory_struct_example()
{
    SYSTEMTIME time, copy;

    ZeroMemory(&copy, sizeof(SYSTEMTIME));

    GetLocalTime(&time);

    CopyMemory(&copy, &time, sizeof(SYSTEMTIME));

    printf("Copy Time: %d-%02d-%02d %02d:%02d:%02d\n",
        copy.wYear, copy.wMonth, copy.wDay, copy.wHour, copy.wMinute, copy.wSecond);
}

void copy_memory_examples()
{
    copy_memory_memcpy_array_example();
    copy_memory_memcpy_struct_example();
    copy_memory_safe_memcpy_array_example();
    copy_memory_safe_memcpy_struct_example();
    copy_memory_copymemory_array_example();
    copy_memory_copymemory_struct_example();

    printf("\n---------\n");
}

const char memory_move_example[] = "Memory Move Example";
const char long_memory_move_example[] = "Long Memory Move Example";

void move_memory_memmove_examples()
{
    char input[25];

    memmove(input, memory_move_example, strlen(memory_move_example) + 1);

    printf("Original: %s. ", input);

    const char* last_word = &memory_move_example[12];

    memmove(&input[7], last_word, strlen(last_word) + 1);

    printf("Moved: %s \n", input);
}

void move_memory_safe_memmove_examples()
{
    char input[36];

    memmove(input, memory_move_example, strlen(memory_move_example) + 1);

    printf("Original: %s. ", input);

    const char* last_word = &memory_move_example[12];

    memmove_s(&input[7], sizeof(input) - 7, last_word, strlen(last_word) + 1);

    printf("Moved: %s \n", input);
}

void move_memory_memmove_overlap_examples()
{
    char input[42];

    memmove(input, long_memory_move_example, strlen(long_memory_move_example) + 1);

    printf("Original: %s. ", input);

    const char* last_word = &long_memory_move_example[17];

    memmove(&input[12], last_word, strlen(last_word) + 1);

    printf("Moved: %s \n", input);
}

void move_memory_safe_memmove_overlap_examples()
{
    char input[42];

    memmove(input, long_memory_move_example, strlen(long_memory_move_example) + 1);

    printf("Original: %s. ", input);

    const char* last_word = &long_memory_move_example[17];

    memmove_s(&input[12], sizeof(input) - 12, last_word, strlen(last_word) + 1);

    printf("Moved: %s \n", input);
}

void move_memory_examples()
{
    move_memory_memmove_examples();
    move_memory_safe_memmove_examples();
    move_memory_memmove_overlap_examples();
    move_memory_safe_memmove_overlap_examples();

    printf("\n---------\n");
}

void compare_memory_memcp_array_example()
{
    static const char* inputs[] =
    {
        "string", "value", "123", "example"
    };

    static const char* compare[] =
    {
        "string", "VALUE", "Example", "1234"
    };

    for (int i = 0; i < _countof(inputs); i++)
    {
        for (int j = 0; j < _countof(compare); ++j)
        {
            printf("Comparing %s with %s using memcmp results in: %d\n",
                inputs[i], compare[j], memcmp(inputs[i], compare[j], strlen(inputs[i]) + 1));
        }
    }
}

void compare_memory_memcp_struct_example()
{
    SYSTEMTIME time, copy;

    ZeroMemory(&copy, sizeof(SYSTEMTIME));

    GetLocalTime(&time);

    CopyMemory(&copy, &time, sizeof(SYSTEMTIME));

    printf("Original Structure %s Copy Structure\n", memcmp(&time, &copy, sizeof(time)) == 0 ? "==" : "!=");

    copy.wYear = 0;
    
    printf("Original Structure %s Modified Structure\n", memcmp(&time, &copy, sizeof(time)) == 0 ? "==" : "!=");
}

void compare_memory_examples()
{
    compare_memory_memcp_array_example();
    compare_memory_memcp_struct_example();
}

int main(int argc, char** argv)
{
    zeroing_memory_examples();
    set_memory_values_examples();
    copy_memory_examples();
    move_memory_examples();
    compare_memory_examples();

    return 0;
}
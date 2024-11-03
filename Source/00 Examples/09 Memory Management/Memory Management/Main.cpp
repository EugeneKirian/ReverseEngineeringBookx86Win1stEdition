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
#include <stdlib.h>

#include <windows.h>

void allocate_free_array_example()
{
    int* pointer = (int*)malloc(19 * sizeof(int));

    if (pointer == NULL)
    {
        printf("Unable to allocate memory.\n");

        return;
    }

    printf("Memory allocated at %p\n", pointer);

    for (int i = 0; i < 19; i++)
    {
        printf("%d ", pointer[i]);
    }

    printf("\n");

    free(pointer);
}

void allocate_free_structure_example()
{
    BITMAPV4HEADER* pointer = (BITMAPV4HEADER*)malloc(sizeof(BITMAPV4HEADER));

    if (pointer != NULL)
    {
        printf("Memory allocated at %p\n", pointer);

        printf("Width: %d, Height: %d\n", pointer->bV4Width, pointer->bV4Height);

        free(pointer);
    }
}

void allocate_free_structures_example()
{
    BITMAPV4HEADER* pointer = (BITMAPV4HEADER*)malloc(12 * sizeof(BITMAPV4HEADER));

    if (pointer != NULL)
    {
        printf("Memory allocated at %p\n", pointer);

        BITMAPV4HEADER* item = pointer;

        for (int i = 0; i < 12; i++)
        {
            printf("Width: %d, Height: %d\n", item->bV4Width, item->bV4Height);

            item++;
        }

        free(pointer);
    }
}

void allocate_zeroes_free_example()
{
    int* pointer = (int*)calloc(19, sizeof(int));

    if (pointer == NULL)
    {
        printf("Unable to allocate memory.\n");

        return;
    }

    printf("Memory allocated at %p\n", pointer);

    for (int i = 0; i < 19; i++)
    {
        printf("%d ", pointer[i]);
    }

    printf("\n");

    free(pointer);
}

void reallocate_free_example()
{
    int* pointer = (int*)calloc(4, sizeof(int));

    if (pointer == NULL)
    {
        printf("Unable to allocate memory.\n");

        return;
    }

    printf("Memory allocated at %p\n", pointer);

    for (int i = 0; i < 4; i++)
    {
        printf("%d ", pointer[i]);
    }

    printf("\n");

    int* new_pointer = (int*)realloc(pointer, 8 * sizeof(int));

    if (new_pointer == NULL)
    {
        printf("Unable to reallocate memory.\n");

        free(pointer);

        return;
    }

    printf("Memory reallocated at %p\n", new_pointer);

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", new_pointer[i]);
    }

    printf("\n");

    free(new_pointer);
}

void aligned_allocate_free_example()
{
    int* pointer = (int*)_aligned_malloc(19 * sizeof(int), 16);

    if (pointer == NULL)
    {
        printf("Unable to allocate aligned memory.\n");

        return;
    }

    printf("Aligned memory allocated at %p\n", pointer);

    for (int i = 0; i < 19; i++)
    {
        printf("%d ", pointer[i]);
    }

    printf("\n");

    _aligned_free(pointer);
}

int main(int argc, char** argv)
{
    allocate_free_array_example();
    allocate_free_structure_example();
    allocate_free_structures_example();

    allocate_zeroes_free_example();
    reallocate_free_example();

    aligned_allocate_free_example();

    return 0;
}
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

#include <windows.h>

#define MAX_VALUES_COUNT 15

void simple_stack_array_allocation_example()
{
    int values[MAX_VALUES_COUNT];

    for (int i = 0; i < MAX_VALUES_COUNT; i++)
    {
        printf("%d ", values[i]);
    }

    printf("\n");
}

void proper_stack_array_allocation_example()
{
    char path[MAX_PATH] = { 0 };

    GetSystemDirectoryA(path, MAX_PATH);

    char message[MAX_PATH + 50];

    sprintf(message, "System Directory: %s\n", path);
    printf(message);
}

#define MAX_INPUT_COUNT         12
#define MAX_OUTPUT_MULTIPLIER   17
#define MAX_OUTPUT_COUNT        (MAX_INPUT_COUNT * MAX_OUTPUT_MULTIPLIER)

int inputs[MAX_INPUT_COUNT] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23 };
int outputs[MAX_OUTPUT_COUNT];

void global_array_allocation_example()
{
    for (int i = 0; i < MAX_INPUT_COUNT; i++)
    {
        for (int j = 0; j < MAX_OUTPUT_MULTIPLIER; j++)
        {
            outputs[i * MAX_OUTPUT_MULTIPLIER + j] = inputs[i] * (j + 1);
        }
    }

    for (int i = 0; i < MAX_OUTPUT_COUNT; i++)
    {
        printf("%d ", outputs[i]);
    }

    printf("\n");
}

#define MAX_POINTER_MATH_EXAMPLE_COUNT 5

const char characters[MAX_POINTER_MATH_EXAMPLE_COUNT] = { 1, 2, 3, 4, 5 };
const short shorts[MAX_POINTER_MATH_EXAMPLE_COUNT] = { 10, 11, 12, 13, 14 };
const int integers[MAX_POINTER_MATH_EXAMPLE_COUNT] = { 100, 101, 102, 103, 104 };
const double doubles[MAX_POINTER_MATH_EXAMPLE_COUNT] = { 1000.0, 1001.1, 1002.2, 1003.3, 1004.4 };

void char_pointer_math_example()
{
    const char* pointer = characters;

    for (int i = 0; i < MAX_POINTER_MATH_EXAMPLE_COUNT; i++)
    {
        printf("%d ", (int)*pointer);

        pointer++;
    }

    printf("\n");
}

void short_pointer_math_example()
{
    const short* pointer = shorts;

    for (int i = 0; i < MAX_POINTER_MATH_EXAMPLE_COUNT; i++)
    {
        printf("%d ", (int)*pointer);

        pointer++;
    }

    printf("\n");
}

void integer_pointer_math_example()
{
    const int* pointer = integers;

    for (int i = 0; i < MAX_POINTER_MATH_EXAMPLE_COUNT; i++)
    {
        printf("%d ", *pointer);

        pointer++;
    }

    printf("\n");
}

void double_pointer_math_example()
{
    const double* pointer = doubles;

    for (int i = 0; i < MAX_POINTER_MATH_EXAMPLE_COUNT; i++)
    {
        printf("%lf ", *pointer);

        pointer++;
    }

    printf("\n");
}

void pointer_math_examples()
{
    char_pointer_math_example();
    short_pointer_math_example();
    integer_pointer_math_example();
    double_pointer_math_example();
}

int main(int argc, char** argv)
{
    simple_stack_array_allocation_example();

    printf("\n----------\n");

    proper_stack_array_allocation_example();

    printf("\n----------\n");

    global_array_allocation_example();

    printf("\n----------\n");

    pointer_math_examples();

    return 0;
}
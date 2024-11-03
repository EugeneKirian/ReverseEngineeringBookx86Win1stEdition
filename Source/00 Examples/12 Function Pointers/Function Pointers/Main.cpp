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

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

typedef void (__cdecl * basic_function_definition)();

void basic_function()
{
    printf("%s is called\n", __FUNCTION__);
}

basic_function_definition acquire_basic_callback()
{
    return basic_function;
}

void basic_function_example()
{
    basic_function_definition func = acquire_basic_callback();

    if (func != NULL)
    {
        func();
    }
}

typedef int (__stdcall * parameterized_function_definition)(int value);

int __stdcall parameterized_even_function(int value)
{
    return printf("%s is called for %d\n", __FUNCTION__, value);
}

int __stdcall parameterized_odd_function(int value)
{
    return printf("%s is called for %d\n", __FUNCTION__, value);
}

parameterized_function_definition acquire_parameterized_function(int input)
{
    return input % 2 == 0
        ? parameterized_even_function
        : parameterized_odd_function;
}

void parameterized_function_example()
{
    for (int i = 0; i < 10; i++)
    {
        parameterized_function_definition func = acquire_parameterized_function(i);

        if (func != NULL)
        {
            func(i);
        }
    }
}

int compare_integers_function(const void* a, const void* b)
{
    const int arg1 = *(const int*)a;
    const int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;

    return 0;
}

void qsort_example()
{
    int intsegers[] = { -2, 0, 1444, 2024, 99, 0, -743, 2, INT_MIN, 4, INT_MAX, -10000 };
    
    const int count = sizeof(intsegers) / sizeof(*intsegers);

    printf("Original values: ");

    for (int i = 0; i < count; i++)
    {
         printf("%d ", intsegers[i]);
    }

    printf("\n");
 
    qsort(intsegers, count, sizeof(int), compare_integers_function);

    printf("Sorted values: ");

    for (int i = 0; i < count; i++)
    {
        printf("%d ", intsegers[i]);
    }
 
    printf("\n");
}

LRESULT CALLBACK wndproc_function(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    switch (uMsg) 
    { 
        case WM_CREATE:
            {
                printf("WM_CREATE\n");

                return 0;
            }
        case WM_PAINT: 
            {
                printf("WM_PAINT\n");

                return 0;
            }
        case WM_SIZE: 
            {
                printf("WM_SIZE\n");

                return 0;
            }
        case WM_DESTROY: 
            {
                printf("WM_DESTROY\n");

                return 0;
            }            
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void winproc_example()
{
    WNDCLASSA wc;
    ZeroMemory(&wc, sizeof(WNDCLASSA));

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)wndproc_function;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "MainWindowClass";

    if (!RegisterClassA(&wc))
    {
       printf("Unable to register window class.\n");
    }

    UnregisterClassA("MainWindowClass", GetModuleHandle(NULL));
}

int main(int argc, char** argv)
{
    basic_function_example();
    parameterized_function_example();
    qsort_example();
    winproc_example();

    return 0;
}
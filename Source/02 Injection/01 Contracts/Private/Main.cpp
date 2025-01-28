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

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#define BASE_ADDRESS                0x10000000
#define FUN_10001420_ADDRESS        0x10001420
#define FUN_10001780_ADDRESS        0x10001780

typedef unsigned (__cdecl * FUN_10001420PTR)(char* path, void* obj);
typedef unsigned (__cdecl * FUN_10001780PTR)(void* obj);

void execute_library_functions(HMODULE module, char* path)
{
    FUN_10001420PTR init = (FUN_10001420PTR)
        ((SIZE_T)module + (FUN_10001420_ADDRESS - BASE_ADDRESS));

    FUN_10001780PTR release = (FUN_10001780PTR)
        ((SIZE_T)module + (FUN_10001780_ADDRESS - BASE_ADDRESS));

    void* ptr = NULL;

    printf("Calling init private function %d\n", init(path, &ptr));
    printf("Calling release private function %d\n", release(ptr));
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage: public.exe <dll file> <data file>\n");
        return EXIT_FAILURE;
    }

    HMODULE module = LoadLibraryA(argv[1]);

    if (module == NULL)
    {
        printf("Unable to load %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    execute_library_functions(module, argv[2]);

    FreeLibrary(module);

    printf("Done!\n");

    return EXIT_SUCCESS;
}
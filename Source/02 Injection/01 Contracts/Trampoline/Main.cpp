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

HMODULE dll;

int fun_10001440(const char* path, void** pointer)
{
    int result = 0;

    __asm
    {
        xor ebx, ebx;
        add ebx, dll;
        add ebx, 0x00001440;

        push pointer;   // The second argument.

        mov eax, path;  // The first argument.

        call ebx;       // Function call.
        add esp, 0x4;   // Clean-up the stack.

        mov result, eax;
    }

    return result;
}

int fun_10001760(void* pointer)
{
    int result = 0;

    __asm
    {
        xor eax, eax;
        add eax, dll;
        add eax, 0x00001760;

        mov esi, pointer;   // Function argument.

        call eax;           // Function call.

        mov result, eax;
    }

    return result;
}

void execute_library_functions(char* path)
{
    void* ptr = NULL;

    printf("Calling init private function %d\n", fun_10001440(path, &ptr));
    printf("Calling release private function %d\n", fun_10001760(ptr));
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("Usage: trampoline.exe <dll file> <data file>\n");
        return EXIT_FAILURE;
    }

    dll = LoadLibraryA(argv[1]);

    if (dll == NULL)
    {
        printf("Unable to load %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    execute_library_functions(argv[2]);

    FreeLibrary(dll);

    printf("Done!\n");

    return EXIT_SUCCESS;
}
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

typedef int (__cdecl * PFNNUMBER)(void);
typedef int (__cdecl * PFNPRINT)(const char* str);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: import.exe <exe file>\n");
        return EXIT_FAILURE;
    }

    HMODULE module = LoadLibraryA(argv[1]);

    if (module == NULL)
    {
        printf("Unable to load %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    PFNNUMBER num = (PFNNUMBER)GetProcAddress(module, "number");

    printf("Value: %d\n", num());

    
    PFNPRINT print = (PFNPRINT)GetProcAddress(module, "print");

    print("Print Test...");

    FreeLibrary(module);

    return 0;
}
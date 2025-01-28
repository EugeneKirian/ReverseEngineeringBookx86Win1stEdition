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

typedef unsigned (__cdecl *CREATEASSETSFUNCTION)(char*, void**);
typedef unsigned (__cdecl *GETASSETSCONTENTFUNCTION)(void*, char*, void*);
typedef unsigned (__cdecl *GETASSETSCONTENTCONTENTFUNCTION)(void*, void*, void*);
typedef unsigned (__cdecl *RELEASEASSETSFUNCTION)(void*);

int execute_library_functions(HMODULE module, char* path)
{
    CREATEASSETSFUNCTION create =
        (CREATEASSETSFUNCTION)GetProcAddress(module, "CreateAssets");

    if (create == NULL)
    {
        return EXIT_FAILURE;
    }

    RELEASEASSETSFUNCTION release =
        (RELEASEASSETSFUNCTION)GetProcAddress(module, "ReleaseAssets");

    if (release == NULL)
    {
        return EXIT_FAILURE;
    }

    void* asset;
    int result = create(path, &asset);

    if (result != 0)
    {
        return EXIT_FAILURE;
    }

    release(asset);

    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: public.exe <dll file>\n");
        return EXIT_FAILURE;
    }

    HMODULE module = LoadLibraryA(argv[1]);

    if (module == NULL)
    {
        printf("Unable to load %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    CHAR dir[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, dir) == 0)
    {
        printf("Unable to get current working directory.");
        return EXIT_FAILURE;
    }

    const int result = execute_library_functions(module, dir);

    FreeLibrary(module);

    printf(result == EXIT_SUCCESS ? "Success!" : "Failure!");

    return result;
}
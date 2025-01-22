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

#include <malloc.h>

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

void stack_example_basic()
{
    char path[1024 * 10];

    GetWindowsDirectoryA(path, MAX_PATH);

    printf("%s", path);
}

void stack_example_function()
{
    char* path = (char*)_alloca(MAX_PATH * 2);

    if (path == NULL)
    {
        printf("Unable to allocate memory on stack.\n");

        return;
    }

    GetWindowsDirectoryA(path, MAX_PATH);

    printf("%s", path);
}

int main(int argc, char** argv)
{
    stack_example_basic();
    stack_example_function();

    return 0;
}
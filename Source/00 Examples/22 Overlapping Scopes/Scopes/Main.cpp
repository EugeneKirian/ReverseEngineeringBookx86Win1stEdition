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

int main(int argc, char** argv)
{
    {
        int values[7];
        float fvalues[19];

        for (int i = 0; i < 7; i++)
        {
            values[i] = i * 17 + 2 * i;
            fvalues[i] = (float)(i * i * i);
        }

        for (int i = 0; i < 7; i++)
        {
            printf("%d %f", values[i], fvalues[i]);
        }

        printf("\n");
    }

    {
        char path[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, path);

        printf("%s\n", path);
    }

    {
        float values[19];
        
        for (int i = 0; i < 7; i++)
        {
            values[i] = (float)(i * i * i);
        }

        for (int i = 0; i < 7; i++)
        {
            printf("%f ", values[i]);
        }

        printf("\n");
    }

    return 0;
}
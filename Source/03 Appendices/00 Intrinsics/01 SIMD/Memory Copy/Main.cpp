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

#include <intrin.h>

#include <stdio.h>
#include <string.h>

#include <windows.h>

bool mmx_available = false;

bool is_mmx_available()
{
    int info[4] = {-1};
    __cpuid(info, 0);

    if (info[0] == 0) { return false; }

    __cpuid(info, 1);

    return (info[3] & 0x80000) ? true : false;
}

void* mmx_memcpy(void* dst, void* src, const size_t size)
{
    size_t len = size;
    void* result = dst;

    __asm
    {
        mov ecx, len
        mov esi, src
        mov edi, dst
        code:
        cmp ecx, 8
        jb done

        movq mm0, [esi]
        movq [edi], mm0

        add esi, 8
        add edi, 8
        sub ecx, 8

        jmp code

        done:
        mov len, ecx
        mov src, esi
        mov dst, edi
        emms
    }

    for (size_t i = 0; i < len; i++)
    {
        (reinterpret_cast<char*>(dst))[i] = (reinterpret_cast<char*>(src))[i];
    }

    return result;
}

void* optimized_memcpy(void* dst, void* src, const size_t size)
{
    return mmx_available ? mmx_memcpy(dst, src, size) : memcpy(dst, src, size);
}

int main(int argc, char** argv)
{
    mmx_available = is_mmx_available();

    char original[MAX_PATH];
    GetSystemDirectoryA(original, MAX_PATH);

    char copy[MAX_PATH];
    optimized_memcpy(copy, original, strlen(original) + 1);

    printf("Copied: %s\n", copy);

	return 0;
}
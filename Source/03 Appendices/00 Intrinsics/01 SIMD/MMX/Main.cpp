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

bool is_mmx_available()
{
    int info[4] = {-1};
    __cpuid(info, 0);

    if (info[0] == 0) { return false; }

    __cpuid(info, 1);

    return (info[3] & 0x80000) ? true : false;
}

void string_to_upper_case(char* value)
{
    if (value == NULL)
    {
        return;
    }

    while (*value != NULL)
    {
        char l = *value;

        if (l >= 'a' && l <= 'z')
        {
            *value = l - 32;
        }

        value++;
    }
}

void string_to_upper_case_mmx(char* value)
{
    char* ptr = value;
    size_t length = strlen(value) + 1;
    
    if (length >= 8)
    {
        const __m64 min = _mm_set1_pi8('a' - 1);
        const __m64 max = _mm_set1_pi8('z' + 1);
        const __m64 mod = _mm_set1_pi8(0x20);

        do
        {
            int* a = reinterpret_cast<int*>(ptr);
            int* b = reinterpret_cast<int*>(ptr + 4);

            __m64 in = _mm_set_pi32(*b, *a);

            __m64 comp_min = _m_pcmpgtb(in, min);
            __m64 comp_max = _m_pcmpgtb(max, in);
            
            __m64 mix = _m_pand(comp_min, comp_max);
            __m64 change = _m_pand(mix, mod);

            __m64 out = _m_psubb(in, change);

            *a = out.m64_i32[0];
            *b = out.m64_i32[1];

            ptr = ptr + 8;
            length = length - 8;
        } while (length >= 8);

        _m_empty();
    }

    if (length > 0)
    {
        string_to_upper_case(ptr);
    }
}

void string_to_upper_case_example(char* value)
{
    if (value != NULL)
    {
        string_to_upper_case_mmx(value);
    }

    printf("%s\n", value);
}

int main(int argc, char** argv)
{
    if (is_mmx_available())
    {
        char string[255];
        strcpy_s(string, 255, "this is a test mixed character string ($@# 12589) for mmx processing!");
        string_to_upper_case_example(string);
    }
    else
    {
        printf("MMX is not supported.\n");
    }

	return 0;
}
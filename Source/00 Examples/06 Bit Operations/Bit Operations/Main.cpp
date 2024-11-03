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

int basic_flag_is_set_example(unsigned int input, unsigned int flag)
{
    return input & flag;
}

int clean_flag_is_set_example(unsigned int input, unsigned int flag)
{
    return (input & flag) != 0 ? TRUE : FALSE;
}

int flags_count_example(unsigned int input)
{
    int result = 0;

    while (input != 0)
    {
        if (input & 1)
        {
            result++;
        }

        input = input >> 1;
    }

    return result;
}

#define MAX_FLAG_INPUT_COUNT    16
#define FLAG_VALUE              (PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER)

void flags_examples()
{
    const static unsigned int inputs[MAX_FLAG_INPUT_COUNT] =
    {
        PFD_DOUBLEBUFFER,
        PFD_STEREO,
        PFD_DRAW_TO_WINDOW,
        PFD_DRAW_TO_BITMAP,
        PFD_SUPPORT_GDI,
        PFD_SUPPORT_OPENGL,
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_GENERIC_FORMAT,
        PFD_NEED_PALETTE | PFD_NEED_SYSTEM_PALETTE,
        PFD_SWAP_EXCHANGE,
        PFD_SWAP_COPY,
        PFD_GENERIC_ACCELERATED,
        PFD_SUPPORT_DIRECTDRAW,
        PFD_SUPPORT_DIRECTDRAW | PFD_SUPPORT_OPENGL,
        PFD_SUPPORT_DIRECTDRAW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_SUPPORT_DIRECTDRAW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER,
    };

    for (int i = 0; i < MAX_FLAG_INPUT_COUNT; i++)
    {
        const int contains = basic_flag_is_set_example(inputs[i], FLAG_VALUE)
            || clean_flag_is_set_example(inputs[i], FLAG_VALUE);

        printf("(0x%08x & 0x%x) == %s. Number of bits set: %d.\n",
            inputs[i], FLAG_VALUE, contains ? "TRUE" : "FALSE", flags_count_example(inputs[i]));
    }
}

unsigned int left_unsigned_bit_shift_example(unsigned int input, unsigned int shift)
{
    return input << shift;
}

unsigned int right_unsigned_bit_shift_example(unsigned int input, unsigned int shift)
{
    return input >> shift;
}

void unsigned_bit_shift_example()
{
    printf("\nUnsigned Bit Shift\n\n");

    for (int i = 0; i < 10; i++)
    {
        printf("(%d * %d) %s (%d << %d)\n",
            i, 2, i * 2 == left_unsigned_bit_shift_example(i, 1) ? "==" : "!=", i, 1);
        printf("(%d * %d) %s (%d << %d)\n",
            i, 4, i * 4 == left_unsigned_bit_shift_example(i, 2) ? "==" : "!=", i, 2);
        printf("(%d * %d) %s (%d << %d)\n",
            i, 8, i * 8 == left_unsigned_bit_shift_example(i, 3) ? "==" : "!=", i, 3);
    }

    for (int i = 100; i >= 10; i = i - 10)
    {
        printf("(%d / %d) %s (%d >> %d)\n",
            i, 2, i / 2 == right_unsigned_bit_shift_example(i, 1) ? "==" : "!=", i, 1);
        printf("(%d / %d) %s (%d >> %d)\n",
            i, 4, i / 4 == right_unsigned_bit_shift_example(i, 2) ? "==" : "!=", i, 2);
        printf("(%d / %d) %s (%d >> %d)\n",
            i, 8, i / 8 == right_unsigned_bit_shift_example(i, 3) ? "==" : "!=", i, 3);
    }
}

int left_signed_bit_shift_example(int input, unsigned int shift)
{
    return input << shift;
}

int right_signed_bit_shift_example(int input, unsigned int shift)
{
    return input >> shift;
}

void signed_bit_shift_example()
{
    printf("\nSigned Bit Shift\n\n");

    for (int i = -100; i < 101; i = i + 10)
    {
        printf("(%d * %d) %s (%d << %d)\texpected: %d got: %d\n",
            i, 2, i * 2 == left_signed_bit_shift_example(i, 1) ? "==" : "!=", i, 1, i * 2, left_signed_bit_shift_example(i, 1));
        printf("(%d * %d) %s (%d << %d)\texpected: %d got: %d\n",
            i, 4, i * 4 == left_signed_bit_shift_example(i, 2) ? "==" : "!=", i, 2, i * 4, left_signed_bit_shift_example(i, 2));
        printf("(%d * %d) %s (%d << %d)\texpected: %d got: %d\n",
            i, 8, i * 8 == left_signed_bit_shift_example(i, 3) ? "==" : "!=", i, 3, i * 8, left_signed_bit_shift_example(i, 3));
    }

    for (int i = -100; i < 101; i = i + 10)
    {
        printf("(%d / %d) %s (%d >> %d)\texpected: %d got: %d\n",
            i, 2, i / 2 == right_signed_bit_shift_example(i, 1) ? "==" : "!=", i, 1, i / 2, right_signed_bit_shift_example(i, 1));
        printf("(%d / %d) %s (%d >> %d)\texpected: %d got: %d\n",
            i, 4, i / 4 == right_signed_bit_shift_example(i, 2) ? "==" : "!=", i, 2, i / 4, right_signed_bit_shift_example(i, 2));
        printf("(%d / %d) %s (%d >> %d)\texpected: %d got: %d\n",
            i, 8, i / 8 == right_signed_bit_shift_example(i, 3) ? "==" : "!=", i, 3, i / 8, right_signed_bit_shift_example(i, 3));
    }
}

#define BOOL_TYPE_A 12346
#define BOOL_TYPE_B 12201

int boolean_if_else_optimization_example(unsigned char input)
{
    return input ? BOOL_TYPE_A : BOOL_TYPE_B;
}

void boolean_compare_examples()
{
    printf("\nBoolean Compare\n\n");

    for (unsigned char i = 0; i < 10; i++)
    {
        printf("%d -> %d\n", i % 2, boolean_if_else_optimization_example(i % 2));
    }
}

int main(int argc, char** argv)
{
    flags_examples();
    unsigned_bit_shift_example();
    signed_bit_shift_example();
    boolean_compare_examples();

    return 0;
}
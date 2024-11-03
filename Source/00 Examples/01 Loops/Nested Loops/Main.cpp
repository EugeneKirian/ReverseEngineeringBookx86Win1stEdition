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

#include <stddef.h>

int do_while_do_while_nested_loops_example(int outer, int inner, int* output)
{
    int icount = 0;
    int ocount = 0;

    int result = 0;

    do
    {
        do
        {
            result += icount;

            icount++;
        } while (icount < inner);

        ocount++;
    } while (ocount < outer);

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int do_while_for_nested_loops_example(int outer, int inner, int* output)
{
    int ocount = 0;
    int result = 0;

    do
    {
        for (int i = 0; i < inner; i++)
        {
            result += i * 2;
        }

        ocount++;
    } while (ocount < outer);

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int do_while_while_nested_loops_example(int outer, int inner, int* output)
{
    int icount = 0;
    int ocount = 0;

    int result = 0;

    do
    {
        while (icount < inner)
        {
            result += icount;

            icount++;
        }

        ocount++;
    } while (ocount < outer);

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int for_do_while_nested_loops_example(int outer, int inner, int* output)
{
    int result = 0;
    int icount = 0;

    for (int i = 0; i < outer; i++)
    {
        do
        {
            result = result + 3 * i;
            icount++;
        } while (icount < inner);
    }

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int for_for_nested_loops_example(int outer, int inner, int* output)
{
    int result = 0;

    for (int i = 0; i < outer; i++)
    {
        for (int j = 0; j < inner; j++)
        {
            result = result + 3 * i;
        }
    }

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int for_while_nested_loops_example(int outer, int inner, int* output)
{
    int result = 0;
    int icount = 0;

    for (int i = 0; i < outer; i++)
    {
        while (icount < inner)
        {
            result = result + 3 * i;
            icount++;
        }
    }

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int while_do_while_nested_loops_example(int outer, int inner, int* output)
{
    int icount = 0;
    int ocount = 0;

    int result = 0;

    do
    {
        do
        {
            result += icount;

            icount++;
        } while (icount < inner);

        ocount++;
    } while (ocount < outer);

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int while_for_nested_loops_example(int outer, int inner, int* output)
{
    int ocount = 0;
    int result = 0;

    while (ocount < outer)
    {
        for (int i = 0; i < inner; i++)
        {
            result += i * 2 + 1;
        }

        ocount++;
    }

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int while_while_nested_loops_example(int outer, int inner, int* output)
{
    int icount = 0;
    int ocount = 0;

    int result = 0;

    while (ocount < outer)
    {
        while (icount < inner)
        {
            result += icount;

            icount++;
        }

        ocount++;
    }

    if (output != NULL)
    {
        *output = result;
    }

    return result;
}

int main(int argc, char** argv)
{
    int result = 0;

    do_while_do_while_nested_loops_example(10, 5, &result);
    do_while_for_nested_loops_example(12, 4, &result);
    do_while_while_nested_loops_example(17, 7, NULL);

    for_do_while_nested_loops_example(1, 2, &result);
    for_for_nested_loops_example(7, 4, NULL);
    for_while_nested_loops_example(11, 9, NULL);

    while_do_while_nested_loops_example(13, 9, NULL);
    while_for_nested_loops_example(9, 3, &result);
    while_while_nested_loops_example(10, 9, NULL);

    return 0;
}
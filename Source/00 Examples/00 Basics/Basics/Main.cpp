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

#include <stdarg.h>

unsigned char   byte_variable   = 19;
char            char_variable   = 'A';
short           short_variable  = 19001;
int             int_variable    = 100001;
long long       long_variable   = 100000000000LL;
float           single_variable = 1.0f;
double          double_variable = 1.0;

int             initialized_int_array[13] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
int             uninitialized_int_array[6];

float           initialized_single_array[4] = { 1.0f, 127.0f, 255.0f, -1.0f };
double          initialized_double_array[3] = { 1.0, 255.0, 127.0 };

void __cdecl simple_cdecl_function_example()
{
    byte_variable++;
}

void __fastcall simple_fastcall_function_example()
{
    long_variable++;
}

void __stdcall simple_stdcall_function_example()
{
    char_variable--;
}

void __cdecl parameterized_cdecl_function_example(short delta)
{
    short_variable = short_variable + delta;
}

void __fastcall parameterized_fastcall_function_example(short delta)
{
    long_variable = long_variable + delta;
}

void __stdcall parameterized_stdcall_function_example(short delta)
{
    long_variable = long_variable - delta;
}

void __cdecl complex_parameterized_cdecl_function_example(short index, int i, float f, double d)
{
    initialized_int_array[index] = initialized_int_array[index] + short_variable;
    uninitialized_int_array[index] = int_variable + i;
    
    initialized_single_array[index] = f;
    initialized_double_array[index] = d;
}

void __fastcall complex_parameterized_fastcall_function_example(short index, int i, float f, double d)
{
    initialized_int_array[index] = initialized_int_array[index] - short_variable;
    uninitialized_int_array[index] = int_variable - i;
    
    initialized_single_array[index] = f + 1.0f;
    initialized_double_array[index] = d + 1.0;
}

void __stdcall complex_parameterized_stdcall_function_example(short index, int i, float f, double d)
{
    initialized_int_array[index] = initialized_int_array[index] - short_variable;
    uninitialized_int_array[index] = int_variable - i;
    
    initialized_single_array[index] = single_variable + f;
    initialized_double_array[index] = double_variable + d;
}

int __cdecl get_integer_value_cdecl_function_example(int index)
{
    return initialized_int_array[index];
}

int __fastcall get_integer_value_fastcall_function_example(int index)
{
    return initialized_int_array[index + 1];
}

int __stdcall get_integer_value_stdcall_function_example(int index)
{
    return uninitialized_int_array[index + 2];
}

float __cdecl get_single_value_cdecl_function_example(int index)
{
    return initialized_single_array[index];
}

float __fastcall get_single_value_fastcall_function_example(int index)
{
    return initialized_single_array[index + 1];
}

float __stdcall get_single_value_stdcall_function_example(int index)
{
    return initialized_single_array[index + 2];
}

double __cdecl get_double_value_cdecl_function_example(int index)
{
    return initialized_double_array[index];
}

double __fastcall get_double_value_fastcall_function_example(int index)
{
    return initialized_double_array[index];
}

double __stdcall get_double_value_stdcall_function_example(int index)
{
    return initialized_double_array[index];
}

int __cdecl varargs_cdecl_function_example(int count, ...)
{
    int result = 0;

    va_list items;
    va_start(items, count);

    for (int i = 0; i < count; i++)
    {
        result += va_arg(items, int);
    }

    return result;
}

int _stdcall varargs_stdcall_function_example(int count, ...)
{
    int result = 0;

    va_list items;
    va_start(items, count);

    for (int i = 0; i < count; i++)
    {
        result += va_arg(items, int);
    }

    return result;
}

int main(int argc, char** argv)
{
    simple_cdecl_function_example();
    simple_fastcall_function_example();
    simple_stdcall_function_example();

    parameterized_cdecl_function_example(7);
    parameterized_fastcall_function_example(11);
    parameterized_stdcall_function_example(9);

    complex_parameterized_cdecl_function_example(2, 13, 7.0f, 19.0);
    complex_parameterized_fastcall_function_example(19, 1, 0.5f, 2.0);
    complex_parameterized_stdcall_function_example(1, 9, -1.0f, 1.0/255.0);

    int int_values[4];
    int_values[0] = get_integer_value_cdecl_function_example(1);
    int_values[1] = get_integer_value_fastcall_function_example(10);
    int_values[2] = get_integer_value_stdcall_function_example(2);

    float float_values[12];
    float_values[0] = get_single_value_cdecl_function_example(0);
    float_values[9] = get_single_value_fastcall_function_example(3);
    float_values[10] = get_single_value_stdcall_function_example(4);

    double double_values[17];
    double_values[11] = get_double_value_cdecl_function_example(19007 + int_values[0]);
    double_values[10] = get_double_value_fastcall_function_example(14 - int_values[1]);
    double_values[12] = get_double_value_stdcall_function_example(int_values[2]);

    int_values[2] = varargs_cdecl_function_example(5, 0, 1, 2, 3, 4);
    int_values[3] = varargs_stdcall_function_example(5, 0, 2, 4, 6, 8);

    return 0;
}
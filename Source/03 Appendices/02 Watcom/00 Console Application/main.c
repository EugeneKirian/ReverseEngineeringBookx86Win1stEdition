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

void one_parameter_function_call_example(int a)
{
    printf("%d\n", a);
}

void two_parameters_function_call_example(int a, short b)
{
    printf("%d %d\n", a, b);
}

float one_parameter_function_call_example_return(float c)
{
    printf("%f\n", c);

    return c * 2.0f;
}

void three_parameters_function_call_example(int a, float c, const char* d)
{
    printf("%d %f %s\n", a, c, d);
}

void watcom_function_call_convention_example(int a, short b, float c, const char* d)
{
    one_parameter_function_call_example(a);
    two_parameters_function_call_example(a, b);

    one_parameter_function_call_example_return(
        c + one_parameter_function_call_example_return(c));

    three_parameters_function_call_example(a, c, d);
}

int main(int argc, char** argv)
{
    watcom_function_call_convention_example(111, 111, 1.0f,
        "This is a string value...");

    return 0;
}
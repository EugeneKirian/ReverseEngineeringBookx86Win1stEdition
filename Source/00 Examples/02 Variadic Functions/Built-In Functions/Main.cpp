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
#include <stdio.h>

int simple_printf_example(const char* format)
{
    return printf(format);
}

int complex_printf_example(const char* format, const char* name)
{
    return printf(format, name);
}

void printf_examples()
{
    simple_printf_example("Hello, World!\n");
    complex_printf_example("Hello, %s!\n", "World");

    printf("String: %s, Integer: %d, Hex: 0x%x, Float: %f, Double: %lf\r\n", "<str>", 1, 10000, 1.0f, 255.0);
}

void secure_printf_example()
{
    printf_s("Secure String: %s, Integer: %d, Hex: 0x%x, Float: %f, Double: %lf\r\n", "<str>", 1, 10000, 1.0f, 255.0);
}

int sprintf_example()
{
    char buffer[100];
    
    sprintf(buffer, "Print: %s, value: %d!\r\n", "<field>", 123);

    return printf(buffer);
}

int sprintf_s_example()
{
    char buffer[100];
    
    sprintf_s(buffer, sizeof(buffer), "Safe Print: %s, value: %d!\r\n", "<field>", 123);

    return printf(buffer);
}

int scanf_example()
{
    printf("Enter a floating point number and press <ENTER>:\n");

    const unsigned max = ~0;
    float result = *((float*)&max);

    const int read = scanf("%f", &result);

    if (read == 0)
    {
        printf("Invalid input!\n");
    }
    else
    {
        printf("Found value: %f\n", result);
    }

    return read;
}

int sscanf_example(const char* payload)
{
    float float_value = 0;
    char buffer[100];
    int integer_value = 0;

    return sscanf(payload, "%f %s %d", &float_value, buffer, &integer_value);
}

int main(int argc, char** argv)
{
    printf_examples();
    secure_printf_example();

    sprintf_example();
    sprintf_s_example();

    scanf_example();
    sscanf_example("19.76 example 123");

    return 0;
}
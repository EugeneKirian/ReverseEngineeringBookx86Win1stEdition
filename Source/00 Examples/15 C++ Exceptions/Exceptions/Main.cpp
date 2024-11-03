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

#include "Calc.h"

#include <exception>
#include <stdexcept>

#include <stdio.h>

#include <windows.h>

int* basic_allocation_exception_example()
{
    try
    {
        return new int[9];
    }
    catch (...)
    {
        printf("Unable to allocate memory.\n");
    }

    return NULL;
}

void basic_exception_example()
{
    int* value = basic_allocation_exception_example();

    printf(value == NULL
        ? "Memory was NOT allocated.\n"
        : "Memory was allocated.\n");

    if (value != NULL)
    {
        delete[] value;
    }
}

double check_double_value(double d)
{
    if (d == 42)
    {
        throw 42;
    }

    return d > 1E6 ? throw std::overflow_error("The value is too large!") : d;
}
 
void multi_catch_example()
{
    double values[] = { 1.0, 22.0, 42.0, 100000.0, 1000001.0 };

    for (int i = 0; i < _countof(values); i++)
    {
        try
        {
            printf("Processing %lf...\n", check_double_value(values[i]));
        }
        catch (const std::overflow_error& e)
        {
            printf("Caught exception: %s\n", e.what());
        }
        catch (const int v)
        {
            printf("Caught integer exception %d.\n", v);
        }
        catch(...)
        {
            printf("Caught unknown exception.\n");
        }
    }
}

void auto_exceptions()
{
    Calc* c1 = new Calc();

    printf("Value #1 is: %lf\n", c1->Get());

    c1->Add(23.0);

    printf("Value #1 is: %lf\n", c1->Get());

    Calc* c2 = new Calc(91.0);

    printf("Value #2 is: %lf\n", c2->Get());

    c2->Add(23.0);

    printf("Value #2 is: %lf\n", c2->Get());

    delete c2;
    delete c1;
}

int main(int argc, char** argv)
{
    basic_exception_example();
    multi_catch_example();
    auto_exceptions();

    return 0;
}
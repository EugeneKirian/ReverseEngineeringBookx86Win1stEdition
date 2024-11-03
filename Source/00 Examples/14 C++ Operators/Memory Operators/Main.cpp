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

struct simple_struct
{
    int a;
    int b;
    float z;
};

class simple_class
{
protected:
    int m_value;
public:
    simple_class();
    virtual int get_value();
    virtual void set_value(int value);
    virtual ~simple_class();
};

simple_class::simple_class()
{
    this->m_value = 17;
}

int simple_class::get_value()
{
    return this->m_value;
}

void simple_class::set_value(int value)
{
    this->m_value = value;
}

simple_class::~simple_class()
{
    this->m_value = 0;
}

void basic_array_new_delete_example()
{
    int* items = new int[17];

    for (int i = 0; i < 5; i++)
    {
        items[i] = i;
    }

    for (int i = 0; i < 17; i++)
    {
        printf("%d = %d\n", i, items[i]);
    }

    delete[] items;
}

void one_struct_new_delete_example()
{
    simple_struct* value = new simple_struct;

    value->a = 1;
    value->b = 2;

    printf("%d %d %f\n", value->a, value->b, value->z);

    delete value;
}

void struct_array_new_delete_example()
{
    simple_struct* values = new simple_struct[5];

    values[0].a = 1;
    values[0].b = 2;

    values[4].a = 11;
    values[4].b = 21;

    for (int i = 0; i < 5; i++)
    {
        printf("%d %d %f\n", values[i].a, values[i].b, values[i].z);
    }

    delete[] values;
}

void one_class_new_delete_example()
{
    simple_class* value = new simple_class;

    value->set_value(1);

    printf("%d\n", value->get_value());

    delete value;
}

void class_array_new_delete_example()
{
    simple_class* values = new simple_class[7];

    for (int i = 0; i < 3; i++)
    {
        values[i].set_value(i);
    }

    for (int i = 0; i < 7; i++)
    {
        printf("%d\n", values[i].get_value());
    }

    delete[] values;
}

int main(int argc, char** argv)
{
    basic_array_new_delete_example();

    printf("\n---------\n");

    one_struct_new_delete_example();

    printf("\n---------\n");

    struct_array_new_delete_example();

    printf("\n---------\n");

    one_class_new_delete_example();

    printf("\n---------\n");

    class_array_new_delete_example();

    return 0;
}
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

typedef enum INVARIANT_TYPE
{
    INVARIANT_TYPE_SHORT    = 0,
    INVARIANT_TYPE_INTEGER  = 1,
    INVARIANT_TYPE_FLOAT    = 2,
    INVARIANT_TYPE_DOUBLE   = 3
} INVARIANT_TYPE;

union small_invariant_value
{
    short mini;
    int integer;
    float single;
};

struct small_invariant
{
    INVARIANT_TYPE type;
    small_invariant_value value;
};

char invariant_string_value[24];

const char* format_small_invariant(small_invariant* input)
{
    switch (input->type)
    {
    case INVARIANT_TYPE_SHORT:
        {
            sprintf(invariant_string_value, "%hd", input->value.mini);

            return invariant_string_value;
        }
    case INVARIANT_TYPE_INTEGER:
        {
            sprintf(invariant_string_value, "%d", input->value.integer);

            return invariant_string_value;
        }
    case INVARIANT_TYPE_FLOAT:
        {
            sprintf(invariant_string_value, "%f", input->value.single);

            return invariant_string_value;
        }
    }

    return "";
}

void small_union_example()
{
    printf("Small invariant structure size: %d. Union size: %d.\n",
        sizeof(small_invariant), sizeof(small_invariant_value));

    small_invariant value;

    value.type = INVARIANT_TYPE_INTEGER;
    value.value.integer = 17;

    printf("Invariant integer value is %s\n", format_small_invariant(&value));

    value.type = INVARIANT_TYPE_FLOAT;
    value.value.single = 17.0;

    printf("Invariant float value is %s\n", format_small_invariant(&value));
}

union large_invariant_value
{
    short mini;
    int integer;
    float single;
    double extended;
};

struct large_invariant
{
    INVARIANT_TYPE type;
    large_invariant_value value;
};

const char* format_large_invariant(large_invariant* input)
{
    switch (input->type)
    {
    case INVARIANT_TYPE_SHORT:
        {
            sprintf(invariant_string_value, "%hd", input->value.mini);

            return invariant_string_value;
        }
    case INVARIANT_TYPE_INTEGER:
        {
            sprintf(invariant_string_value, "%d", input->value.integer);

            return invariant_string_value;
        }
    case INVARIANT_TYPE_FLOAT:
        {
            sprintf(invariant_string_value, "%f", input->value.single);

            return invariant_string_value;
        }
    case INVARIANT_TYPE_DOUBLE:
        {
            sprintf(invariant_string_value, "%lf", input->value.extended);

            return invariant_string_value;
        }
    }

    return "";
}

void large_union_example()
{
    printf("Large invariant structure size: %d. Union size: %d.\n",
        sizeof(large_invariant), sizeof(large_invariant_value));

    large_invariant value;

    value.type = INVARIANT_TYPE_INTEGER;
    value.value.integer = 17;

    printf("Invariant integer value is %s\n", format_large_invariant(&value));

    value.type = INVARIANT_TYPE_FLOAT;
    value.value.single = 17.0;

    printf("Invariant float value is %s\n", format_large_invariant(&value));

    value.type = INVARIANT_TYPE_DOUBLE;
    value.value.single = 17.0;

    printf("Invariant double value is %s\n", format_large_invariant(&value));
}

void simple_union_examples()
{
    small_union_example();
    large_union_example();
}

struct nested_struct
{
   union
   {
       struct { WORD q, w; };
       struct { DWORD o, p; } z;
   };
   int n;
   float m;
} nested_struct_example;

void nested_union_example()
{
    printf("Size of a structure with nested union and structures: %d\n", sizeof(nested_struct_example));

    nested_struct_example.z.o = 0x0F0F0F0F; // 252645135

    printf("Value of q = %hd, w = %hd, o = %d\n",
        nested_struct_example.q, nested_struct_example.w, nested_struct_example.z.o);
}

int main(int argc, char** argv)
{   
    simple_union_examples();
    nested_union_example();

    return 0;
}
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

#include <vector>

#include <stdio.h>

void std_vector_example()
{
    const int arr[] = { 1, 2, 3, 5, 7, 9, 11, 13 };
    const int n = sizeof(arr) / sizeof(arr[0]);

    std::vector<int> v(arr, arr + n);

    for (size_t i = 0; i < v.size(); i++)
    {
        printf("%d and %d\n", v.at(i), v[i]);
    }

    printf("Size: %d, Capacity: %d\n", v.size(), v.capacity());

    v.resize(17);

    printf("Size: %d, Capacity: %d\n", v.size(), v.capacity());
}

std::vector<int> std_vector_operation_example(const int count)
{
    return std::vector<int>(count);
}

void std_vector_operation_examples()
{
    std::vector<int> v = std_vector_operation_example(7);

    printf("Size: %d, Capacity: %d\n", v.size(), v.capacity());

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(5);

    printf("Size: %d, Capacity: %d\n", v.size(), v.capacity());
}

std::vector<int>* std_vector_allocate_example(const int count)
{
    return new std::vector<int>(count);
}

void std_vector_allocate_examples()
{
    std::vector<int>* v = std_vector_allocate_example(7);

    printf("Size: %d, Capacity: %d\n", v->size(), v->capacity());

    v->push_back(1);
    v->push_back(2);
    v->push_back(3);
    v->push_back(5);

    printf("Size: %d, Capacity: %d\n", v->size(), v->capacity());

    delete v;
}

std::vector<float> static_vector;

void std_vector_static_examples()
{
    printf("Size: %d, Capacity: %d\n", static_vector.size(), static_vector.capacity());

    static_vector.push_back(1.0f);
    static_vector.push_back(2.0f);
    static_vector.push_back(3.0f);
    static_vector.push_back(5.0f);

    printf("Size: %d, Capacity: %d\n", static_vector.size(), static_vector.capacity());
}

int main(int argc, char** argv)
{
    std_vector_example();
    std_vector_operation_examples();
    std_vector_allocate_examples();
    std_vector_static_examples();

    return 0;
}
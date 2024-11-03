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

#include <string>

#include <stdio.h>

#include <windows.h>

void std_string_example()
{
    std::string str1 = "C++ String";

    printf("The string is: %s, length: %d\n", str1.c_str(), str1.size());

    std::string str2("Another C++ String");

    printf("The string is: %s, length: %d\n", str2.c_str(), str2.size());
}

std::string std_string_operations_example(const char* path)
{
    std::string str(path);

    if (str.at(str.length() - 1) != '\\')
    {
        str.push_back('\\');
    }

    return str;
}

void append_std_string_value_example(std::string input, const char* value)
{
    input.append(value);
}

void append_std_string_reference_example(std::string& input, const char* value)
{
    input.append(value);
}

void std_string_operations_examples()
{
    char path[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, path);

    std::string str = std_string_operations_example(path);

    printf("%s\n", str.c_str());

    append_std_string_value_example(str, "*.*");

    printf("%s\n", str.c_str());

    append_std_string_reference_example(str, "*.*");

    printf("%s\n", str.c_str());
}

std::string* std_string_empty_allocation_example()
{
    return new std::string;
}

std::string* std_string_classic_allocation_example(const char* value)
{
    return new std::string(value);
}

std::string* std_string_string_allocation_example(std::string value)
{
    return new std::string(value);
}

void std_string_allocation_examples()
{
    std::string* empty = std_string_empty_allocation_example();
    std::string* c = std_string_classic_allocation_example("C String");
    std::string* str = std_string_string_allocation_example(std::string("C++ String"));

    printf("Strings: %s %s %s\n", empty->c_str(), c->c_str(), str->c_str());

    delete empty;
    delete c;
    delete str;
}

std::string static_string;

void std_string_static_examples()
{
    printf("Static string: %s, Capacity: %d\n", static_string.c_str(), static_string.capacity());

    if (static_string.empty())
    {
        static_string.append("Static String Value");
    }

    printf("Static string: %s, Size: %d, Capacity: %d\n",
        static_string.c_str(), static_string.length(), static_string.capacity());
}

int main(int argc, char** argv)
{
    std_string_example();
    std_string_operations_examples();
    std_string_allocation_examples();
    std_string_static_examples();

    return 0;
}
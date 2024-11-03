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

#include <errno.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

void string_classification_examples()
{
    printf("Character Classification\n");

    for (char i = '7'; i < 'E'; i++)
    {
        printf("isalpha(%c): %s\t\tisdigit(%c): %s\n",
            i, isalpha(i) ? "TRUE" : "FALSE",
            i, isdigit(i) ? "TRUE" : "FALSE");
    }
    
    printf("\n-------------\n");
}

void atoi_examples()
{
    printf("%i\n",  atoi(" -123example"));
    printf("%i\n",  atoi("25.75"));
    printf("%i\n",  atoi("2147483648"));
    printf("%i\n",  atoi(" +321value"));
    printf("%i\n",  atoi("0"));
    printf("%i\n",  atoi("0x3F"));
    printf("%i\n",  atoi("\t0065"));
    printf("%i\n",  atoi("test"));
    printf("%i\n",  atoi("123 136"));
    printf("%ld\n", atol("long"));
    printf("%ld\n", atol("2147483649"));
}

void basic_strol_parsing_examples()
{
    printf("Binary  \"1011\"   is  %ld\n", strtol("1011", NULL, 2));
    printf("Octal   \"12\"     is  %ld\n", strtol("12",   NULL, 8));
    printf("Hex     \"A\"      is  %ld\n", strtol("A",    NULL, 16));
    printf("Base-32 \"test\"   is  %ld\n", strtol("test", NULL, 32));
    printf("Auto    \"016\"    is  %ld\n", strtol("016",  NULL, 0));
    printf("Auto    \"0xAA\"   is  %ld\n", strtol("0xAA",  NULL, 0));
    printf("Auto    \"ex\"     is  %ld\n", strtol("ex", NULL, 0));
}

void extended_strol_parsing_example()
{
    const char *pointer = "70   93 \t -77 250000000250000000002500000555 99 strtol";

    printf("Parsing: %s\n", pointer);
 
    while (TRUE)
    {
        errno = 0;
        char *end;
        const long value = strtol(pointer, &end, 10);

        if (pointer == end) { break; }
 
        const bool range_error = errno == ERANGE;

        printf("Extracted: %.*s, got: %ld.", (char*)((size_t)end - (size_t)pointer), pointer, value);

        pointer = end;
 
        if (range_error)
        {
            printf(" Value Range Error!");
        }
 
        putchar('\n');
    }
 
    printf("Unable to parse: %s\n", pointer);
}

void strtol_examples()
{
    basic_strol_parsing_examples();
    
    printf("\n-------------\n");

    extended_strol_parsing_example();
}

void atof_examples()
{
    printf("%lf\n", atof("\t0065"));
    printf("%lf\n", atof("test"));
    printf("%lf\n", atof("123 136"));
    printf("%lf\n", atof("long"));
    printf("%lf\n", atof("2147483649"));
}

void string_conversion_examples()
{
    printf("String Conversion\n");

    atoi_examples();
    
    printf("\n-------------\n");

    strtol_examples();
    
    printf("\n-------------\n");

    atof_examples();
}

void string_length_examples()
{
    const char* input = "String Length";

    printf("Length of %s is %d, pointer size is %d.\n",
        input, strlen(input), sizeof(input));

    const static char limited_input[] = "Safe Limited String Length";

    printf("Limited Length of %s is %d, array size is %d.\n",
        input, strnlen(limited_input, 25), sizeof(limited_input));

    const char* safe_input = "Safe String Length";

    printf("Safe Length of %s is %d, pointer size is %d.\n",
        safe_input, strnlen_s(safe_input, 25), sizeof(safe_input));
}

void string_compare_examples()
{
    static const char* inputs[] =
    {
        "string", "value", "123", "example"
    };

    static const char* compare[] =
    {
        "string", "VALUE", "Example", "1234"
    };

    for (int i = 0; i < _countof(inputs); i++)
    {
        for (int j = 0; j < _countof(compare); ++j)
        {
            printf("Comparing %s with %s using strcmp results in: %d\n",
                inputs[i], compare[j], strcmp(inputs[i], compare[j]));
            printf("Comparing %s with %s using strncmp(x, y, 3) results in: %d\n",
                inputs[i], compare[j], strncmp(inputs[i], compare[j], 3));
            printf("Comparing %s with %s using _stricmp results in: %d\n\n",
                inputs[i], compare[j], _stricmp(inputs[i], compare[j]));
        }
    }
}

void string_find_character_start_example()
{
    char path[MAX_PATH] = { 0 };

    GetSystemDirectoryA(path, MAX_PATH);

    printf("System Directory: %s\n", path);

    const char* result = path;
    const char* last = path;

    while ((result = strchr(result, '\\')) != NULL)
    {
        printf("Found \\ starting at %s\n", result);

        ++result;

        last = result;
    }

    if (last[0] != NULL)
    {
        printf("Extra: %s\n", last);
    }
}

void string_find_character_end_example()
{
    char path[MAX_PATH] = { 0 };

    GetSystemDirectoryA(path, MAX_PATH);

    printf("System Directory: %s\n", path);

    char* result = NULL;

    while ((result = strrchr(path, '\\')) != NULL)
    {
        printf("Found \\ starting at %s\n", result);

        *result = NULL;
    }

    if (path != NULL && path[0] != NULL)
    {
        printf("Extra: %s\n", path);
    }
}

void string_find_substring_example()
{
    static const char* inputs[] =
    {
        "one", "two", " ", "five", "seven", "seventy"
    };

    for (int i = 0; i < _countof(inputs); i++)
    {
        printf("Searching for %s results in: %s\n",
            inputs[i], strstr("one hundred seventy five", inputs[i]));
    }
}

void string_find_any_character_example()
{
    static const char* inputs[] =
    {
        "/\\\"\'", "1234", " ", "@#$%^", "_`~"
    };

    for (int i = 0; i < _countof(inputs); i++)
    {
        printf("Searching for any of %s results in: %s\n",
            inputs[i], strpbrk("abcdeABCDE123456@#$%^&()", inputs[i]));
    }
}

void string_examination_examples()
{
    printf("\n-------------\n");

    string_length_examples();

    printf("\n-------------\n");

    string_compare_examples();

    printf("\n-------------\n");

    string_find_character_start_example();
    string_find_character_end_example();

    printf("\n-------------\n");

    string_find_substring_example();

    printf("\n-------------\n");

    string_find_any_character_example();
}

#define MAX_STRING_MANIPULATION_EXAMPLE_LENGTH 1024

char buffer[MAX_STRING_MANIPULATION_EXAMPLE_LENGTH];

void string_copy_examples()
{
    const static char source[] = "String Copy Example";

    const size_t string_length = strlen(source);

    strcpy(buffer, source);

    printf("Source: %s\nCopy: %s\n", source, buffer);

    ZeroMemory(buffer, (string_length + 1) * sizeof(char));

    strcpy_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, source);

    printf("Source: %s\nCopy: %s\n", source, buffer);
}

void string_copy_with_limit_examples()
{
    const char source[] = "String Copy With Limits Example String";

    const size_t string_length = strlen(source);

    strncpy(buffer, source, 31);

    printf("Source: %s\nLimited Copy: %s\n", source, buffer);

    ZeroMemory(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH * sizeof(char));

    strncpy_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, source, string_length - 7);

    printf("Source: %s\nLimited Copy: %s\n", source, buffer);
}

void string_concatenate_examples()
{
    const static char source[] = "String Concatenate Example";

    strcpy(buffer, source);
    strcat(buffer, " + ");
    strcat(buffer, source);

    printf("Source: %s\nConcatenated: %s\n", source, buffer);

    ZeroMemory(buffer, (strlen(buffer) + 1) * sizeof(char));

    strcpy_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, source);
    strcat_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, " + ");
    strcat_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, source);

    printf("Source: %s\nConcatenated: %s\n", source, buffer);
}

void string_concatenate_with_limit_examples()
{
    const char source[] = "String Concatenate Example";

    strcpy(buffer, source);
    strncat(buffer, " + ", 13);
    strncat(buffer, source, 15);

    printf("Source: %s\nConcatenated: %s\n", source, buffer);

    ZeroMemory(buffer, (strlen(buffer) + 1) * sizeof(char));

    strncpy_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, source, 13);
    strncat_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, " + ", 15);
    strncat_s(buffer, MAX_STRING_MANIPULATION_EXAMPLE_LENGTH, source, 17);

    printf("Source: %s\nConcatenated: %s\n", source, buffer);
}

void string_manipulation_examples()
{
    printf_s("\n-------------\n");

    string_copy_examples();
    string_copy_with_limit_examples();
    string_concatenate_examples();
    string_concatenate_with_limit_examples();
}

int main(int argc, char** argv)
{
    string_classification_examples();
    string_conversion_examples();
    string_examination_examples();
    string_manipulation_examples();

    return 0;
}
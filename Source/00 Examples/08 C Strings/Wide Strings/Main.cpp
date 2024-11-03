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

#include <tchar.h>

#include <windows.h>

void wide_string_length_example()
{
    static const wchar_t wide_string[] = _T("Wide String Example");

    wprintf(_T("Wide string: %s, length (characters): %d, length (bytes): %d\n"),
        wide_string, wcslen(wide_string), sizeof(wide_string));
}

void wide_string_find_character_start_example()
{
    wchar_t path[MAX_PATH] = { 0 };

    GetSystemDirectoryW(path, MAX_PATH);

    wprintf(_T("System Directory: %s\n"), path);

    const wchar_t* result = path;
    const wchar_t* last = path;

    while ((result = wcschr(result, _T('\\'))) != NULL)
    {
        wprintf(_T("Found \\ starting at %s\n"), result);

        ++result;

        last = result;
    }

    if (last[0] != NULL)
    {
        wprintf(_T("Extra: %s\n"), last);
    }
}

int main(int argc, char** argv)
{
    wide_string_length_example();
    wide_string_find_character_start_example();

    return 0;
}
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

#include <Arc.h>
#include <stdio.h>

const char* get_error_name(int err)
{
    switch(err)
    {
    case ARC_FILE_OK: return "ARC_FILE_OK";
    case ARC_FILE_NO_FILES_FOUND: return "ARC_FILE_NO_FILES_FOUND";
    case ARC_FILE_UNABLE_TO_CREATE_FILE: return "ARC_FILE_UNABLE_TO_CREATE_FILE";
    case ARC_FILE_ALREADY_EXISTS: return "ARC_FILE_ALREADY_EXISTS";
    case ARC_FILE_UNABLE_TO_WRITE_FILE: return "ARC_FILE_UNABLE_TO_WRITE_FILE";
    case ARC_FILE_UNABLE_TO_READ_FILE: return "ARC_FILE_UNABLE_TO_READ_FILE";
    case ARC_FILE_UNABLE_TO_ARCHIVE_FILE: return "ARC_FILE_UNABLE_TO_ARCHIVE_FILE";
    }

    return "UNKNOWN";
}

void __cdecl error_handler(int err, const char* path)
{
    printf("Error %s (%d) %s\n", get_error_name(err), err, path);
}

int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 4)
    {
        printf("Usage:\r\n%s <file> <path> <comment>\r\n\t"
            "<file> -- file to be created\r\n\t"
            "<path> -- path to a directory to be archived\r\n\t"
            "<comment> -- optional comment for the archive", argv[0]);
        return -1;
    }

    return CreateArcFile(argv[1], argv[2], argc != 4 ? argv[3] : NULL, error_handler);
}
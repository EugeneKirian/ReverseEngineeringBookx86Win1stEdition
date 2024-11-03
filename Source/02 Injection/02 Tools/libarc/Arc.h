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

#pragma once

#include <windows.h>

#define ARC_FILE_MAGIC              0x48435241 /* ARCH */
#define ARC_FILE_VERSION            1
#define ARC_FILE_COMMENT_LENGTH     256
#define ARC_FILE_NAME_LENGTH        MAX_PATH

struct ArcHeader
{
    unsigned int    Magic;
    unsigned int    Version;
};

struct ArcContent
{
    unsigned int    Count;
    char            Comment[ARC_FILE_COMMENT_LENGTH];
};

struct ArcItem
{
    unsigned int    CRC;
    unsigned int    Offset;
    unsigned int    CompressedSize;
    unsigned int    UncompressedSize;
    char            Name[ARC_FILE_NAME_LENGTH];
};

struct ArcFile
{
    HANDLE      Handle;
    ArcContent  Descriptor;
    ArcItem*    Items;
};

#define ARC_FILE_OK                      0
#define ARC_FILE_NO_FILES_FOUND          1
#define ARC_FILE_UNABLE_TO_CREATE_FILE   2
#define ARC_FILE_ALREADY_EXISTS          3
#define ARC_FILE_UNABLE_TO_WRITE_FILE    4
#define ARC_FILE_UNABLE_TO_READ_FILE     5
#define ARC_FILE_UNABLE_TO_ARCHIVE_FILE  6
#define ARC_FILE_NO_FILE                 7
#define ARC_FILE_INVALID_CONTENT         8
#define ARC_FILE_UNABLE_TO_EXTRACT_FILE  9

int CreateArcFile(const char* file, const char* path,
                  const char* comment, void (* __cdecl err)(int, const char*));
int OpenArcFile(const char* file, ArcFile** archive);
int AcquireArcFileContent(const ArcFile* archive, const ArcItem* item, void* data);
int CloseArcFile(ArcFile* archive);
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
    case ARC_FILE_NO_FILE: return "ARC_FILE_NO_FILE";
    case ARC_FILE_INVALID_CONTENT: return "ARC_FILE_INVALID_CONTENT";
    case ARC_FILE_UNABLE_TO_EXTRACT_FILE: return "ARC_FILE_UNABLE_TO_EXTRACT_FILE";
    }

    return "UNKNOWN";
}

int extract_archive_item(ArcFile* archive, ArcItem* item, const char* path)
{
    char output[MAX_PATH];
    strcpy_s(output, MAX_PATH, path);

    {
        const unsigned int len = (unsigned int)strlen(output);

        for (unsigned int i = 0; i < len; i++)
        {
            if (output[i] == '/') { output[i] = '\\'; }
        }

        const char* last = strrchr(output, '\\');
        if (last != NULL && last[1] != NULL)
        {
            strcat_s(output, MAX_PATH, "\\");
        }   

        strcat_s(output, MAX_PATH, item->Name);
    }

    void* data = malloc(item->UncompressedSize);
    ZeroMemory(data, item->UncompressedSize);

    int result = AcquireArcFileContent(archive, item, data);
    
    if (result != ARC_FILE_OK)
    {
        free(data);
        return result;
    }

    HANDLE handle = CreateFileA(output, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE)
    {
        free(data);
        return ARC_FILE_UNABLE_TO_WRITE_FILE;
    }

    DWORD bytes = 0;
    if (!WriteFile(handle, data, item->UncompressedSize, &bytes, NULL)
        || bytes != item->UncompressedSize)
    {
        free(data);
        CloseHandle(handle);
        DeleteFileA(output);
        return ARC_FILE_UNABLE_TO_WRITE_FILE;
    }
    
    free(data);
    CloseHandle(handle);

    return ARC_FILE_OK;
}

int extract_archive(const char* file, const char* path)
{
    ArcFile* archive = NULL;
    int result = OpenArcFile(file, &archive);
    
    if (result != ARC_FILE_OK)
    {
        printf("Error: %s (%d)\r\n", get_error_name(result), result);
        return -1;
    }

    if (strlen(archive->Descriptor.Comment) != 0)
    {
        printf("Comment: %s\r\n", archive->Descriptor.Comment);
    }

    for (unsigned int i = 0; i < archive->Descriptor.Count; i++)
    {
        printf("Extracting %s ... ", archive->Items[i].Name);

        result = extract_archive_item(archive, &archive->Items[i], path);

        if (result == ARC_FILE_OK) { printf("OK\r\n"); }
        else { printf("Error: %s (%d)\r\n", get_error_name(result), result);}
    }

    return CloseArcFile(archive);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage:\r\n%s <file> <path>\r\n\t"
            "<file> -- archive file\r\n\t"
            "<path> -- path to a directory where extracted files are created\r\n", argv[0]);
        return -1;
    }

    return extract_archive(argv[1], argv[2]);
}
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

#include <Obj.h>

#include <stdio.h>

const char* get_error_name(const int err)
{
    switch (err)
    {
    case OBJ_OK: { return "OBJ_OK"; }
    case OBJERR_GENERIC: { return "OBJERR_GENERIC"; }
    case OBJERR_INVALIDPARAMS: { return "OBJERR_INVALIDPARAMS";}
    case OBJERR_OUTOFMEMORY: { return "OBJERR_OUTOFMEMORY"; }
    case OBJERR_INVALIDFORMAT: { return "OBJERR_INVALIDFORMAT"; }
    }

    return "UNKNOWN";
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage:\r\n%s <input> <output>\r\n\t"
            "<input> -- input .obj file\r\n\t"
            "<output> -- output .o file\r\n", argv[0]);
        return -1;
    }

    HANDLE hIn = CreateFileA(argv[1], GENERIC_READ,
        FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hIn == INVALID_HANDLE_VALUE)
    {
        printf("Unable to open file %s\r\n", argv[1]);
        return -1;
    }

    CONST DWORD dwSize = GetFileSize(hIn, NULL);

    if (dwSize == 0)
    {
        printf("Unable to read file %s\r\n", argv[1]);
        return -1;
    }

    LPVOID obj = malloc(dwSize + 1);

    if (obj == NULL)
    {
        printf("Unable to allocate %u bytes.\r\n", dwSize + 1);
        return -1;
    }

    ZeroMemory(obj, dwSize + 1);

    DWORD dwBytes = 0;
    if (!ReadFile(hIn, obj, dwSize, &dwBytes, NULL) || dwSize != dwBytes)
    {
        printf("Unable to read file %s\r\n", argv[1]);
        return -1;
    }

    CloseHandle(hIn);

    int result = OBJ_OK;
    LPOBJOBJECT object = NULL;
    if ((result = CreateObjObject((LPCHAR)obj, dwSize + 1, &object)) != OBJ_OK)
    {
        printf("Error: %s (%d)\r\n", get_error_name(result), result);
        return -1;
    }

    free(obj);

    HANDLE hOut = CreateFileA(argv[2], GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);

    if (hOut == INVALID_HANDLE_VALUE)
    {
        printf("Unable to create file %s\r\n", argv[2]);
        ReleaseObjObject(object);
        return -1;
    }

    {
        DWORD bytes = 0;
        if (!WriteFile(hOut, &object->Count, sizeof(DWORD), &bytes, NULL)
            || bytes != sizeof(DWORD))
        {
            printf("Unable to write file %s\r\n", argv[2]);
            ReleaseObjObject(object);
            CloseHandle(hOut);
            DeleteFileA(argv[2]);
            return -1;
        }
    }

    {
        DWORD bytes = 0;
        CONST DWORD length = sizeof(OBJTRIANGLE) * object->Count;
        if (!WriteFile(hOut, object->Triangles, length, &bytes, NULL) || bytes != length)
        {
            printf("Unable to write file %s\r\n", argv[2]);
            ReleaseObjObject(object);
            CloseHandle(hOut);
            DeleteFileA(argv[2]);
            return -1;
        }
    }

    ReleaseObjObject(object);
    CloseHandle(hOut);

    return 0;
}
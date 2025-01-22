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

#include "File.h"

BaseFile::BaseFile()
{
    this->m_handle = INVALID_HANDLE_VALUE;
}

BaseFile::~BaseFile()
{
    if (this->m_handle != INVALID_HANDLE_VALUE)
    {
        this->Close();
    }
}

bool BaseFile::Open(const char* path)
{
    if (this->m_handle != INVALID_HANDLE_VALUE) { return false; }

    const HANDLE handle = CreateFileA(path,
        GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle != INVALID_HANDLE_VALUE)
    {
        this->m_handle = handle;

        return true;
    }

    return false;
}

bool BaseFile::Close()
{
    if (this->m_handle != INVALID_HANDLE_VALUE)
    {
        if (CloseHandle(this->m_handle))
        {
            this->m_handle = INVALID_HANDLE_VALUE;

            return true;
        }
    }

    return false;
}

unsigned int RealFile::Read(void* ptr, const unsigned int length)
{
    DWORD bytes = 0;

    if (ReadFile(this->m_handle, ptr, length, &bytes, NULL))
    {
        return bytes;
    }

    return 0;
}

unsigned int RealFile::Write(void* ptr, const unsigned int length)
{
    DWORD bytes = 0;

    if (WriteFile(this->m_handle, ptr, length, &bytes, NULL))
    {
        return bytes;
    }

    return 0;
}

int RealFile::Size()
{
    if (this->m_handle != INVALID_HANDLE_VALUE)
    {
        return INVALID_FILE_SIZE;
    }

    DWORD size = 0;

    if(GetFileSize(this->m_handle, &size) != INVALID_FILE_SIZE)
    {
        return size;
    }

    return INVALID_FILE_SIZE;
}
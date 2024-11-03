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

#include <windows.h>

class BaseFile
{
protected:
    HANDLE m_handle;
    virtual bool Open(const char* path, const bool create);
public:
    BaseFile();
    virtual ~BaseFile();
    virtual bool Create(const char* path);
    virtual bool Open(const char* path);
    virtual bool Close();
    virtual unsigned int Read(void* ptr, const unsigned int length) = 0;
    virtual unsigned int Write(void* ptr, const unsigned int length) = 0;
};

class RealFile: private BaseFile
{
protected:
    char m_name[MAX_PATH];
    virtual bool Open(const char* path, const bool create) override;
public:
    using BaseFile::Create;
    using BaseFile::Open;
public:
    RealFile();
    virtual ~RealFile();
    virtual bool Close() override;
    virtual const char* Name();
    virtual int Size();
    virtual unsigned int Read(void* ptr, const unsigned int length) override;
    virtual unsigned int Write(void* ptr, const unsigned int length) override;
};
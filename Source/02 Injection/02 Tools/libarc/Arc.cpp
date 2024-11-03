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

#include "Arc.h"

#include <string.h>
#include <zlib.h>

#include <vector>

#define CHUNK               (16 * 1024)
#define BACKUP_EXTENSION    ".bak"

std::vector<char*>* AcquireFiles(const char* path)
{
    HANDLE handle;
    WIN32_FIND_DATAA ffd;

    char pattern[MAX_PATH];
    strcpy_s(pattern, MAX_PATH, path);

    std::vector<char*>* result = new std::vector<char*>();
    
    // Construct the pattern for file search
    {
        char* last = strrchr(pattern, '\\');

        if (last == NULL) { return result; }

        if (last[1] == NULL) { strcat_s(pattern, MAX_PATH, "*"); }
        else { strcat_s(pattern, MAX_PATH, "\\*"); }
    }
    
    handle = FindFirstFileA(pattern, &ffd);

    if (handle == INVALID_HANDLE_VALUE) { return result; }

    do 
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            char* name = new char[MAX_PATH];
            strcpy_s(name, MAX_PATH, path);
            
            char* last = strrchr(name, '\\');

            if (last != NULL && last[1] != NULL)
            {
                strcat_s(name, MAX_PATH, "\\");

                strcat_s(name, MAX_PATH, ffd.cFileName);
                result->push_back(name);
            }   
        }
      
    } while (FindNextFileA(handle, &ffd));

    FindClose(handle);

    return result;
}

void FreeFiles(std::vector<char*>* files)
{
    if (files != NULL)
    {
        const unsigned int count = (unsigned int)files->size();

        for (unsigned int i = 0; i < count; i++)
        {
            delete[] files->at(i);
        }

        delete files;
    }
}

BOOL CompressFile(HANDLE handle, const char* file)
{
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    FILE* source = fopen(file, "rb");

    if (source == NULL) { return FALSE; }

    z_stream strm;
    ZeroMemory(&strm, sizeof(z_stream));

    int result = Z_OK;

    if ((result = deflateInit(&strm, Z_DEFAULT_COMPRESSION)) != Z_OK)
    {
        fclose(source);
        return FALSE;
    }

    // Compress
    int flush = 0;
    do
    {
        strm.avail_in = (uInt)fread(in, 1, CHUNK, source);

        if (ferror(source))
        {
            fclose(source);
            return deflateEnd(&strm) == Z_OK;
        }

        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        // Execute deflate()
        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;
           
            if ((result = deflate(&strm, flush)) == Z_STREAM_ERROR)
            {
                fclose(source);
                (void)deflateEnd(&strm);
                return FALSE;
            }

            DWORD bytes = 0;
            const unsigned int have = CHUNK - strm.avail_out;
            if (!WriteFile(handle, out, have, &bytes, NULL) || have != bytes)
            {
                fclose(source);
                return deflateEnd(&strm) == Z_OK;
            }
        } while (strm.avail_out == 0);
        
        if (strm.avail_in != 0)
        {
            fclose(source);
            return FALSE;
        }
    } while (flush != Z_FINISH);

    fclose(source);

    if (result != Z_STREAM_END) { return FALSE; }

    return deflateEnd(&strm) == Z_OK;
}

int ArchiveFile(HANDLE handle, const char* path, ArcItem* item,
                void (* __cdecl err)(int, const char*))
{
    {
        HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (h == INVALID_HANDLE_VALUE)
        {
            if (err != NULL) { err(ARC_FILE_UNABLE_TO_READ_FILE, path); }

            return ARC_FILE_UNABLE_TO_READ_FILE;
        }

        item->UncompressedSize = GetFileSize(h, NULL);

        CloseHandle(h);
    }

    item->Offset = SetFilePointer(handle, 0, NULL, FILE_CURRENT);

    if (!CompressFile(handle, path))
    {
        if (err != NULL) { err(ARC_FILE_UNABLE_TO_ARCHIVE_FILE, path); }

        return ARC_FILE_UNABLE_TO_WRITE_FILE;
    }

    item->CompressedSize = SetFilePointer(handle, 0, NULL, FILE_CURRENT) - item->Offset;

    {
        const char* last = strrchr(path, '\\');
        if (last != NULL)
        {
            strcpy_s(item->Name, ARC_FILE_NAME_LENGTH, &last[1]);
        }
    }

    return ARC_FILE_OK;
}

int WriteArcFile(HANDLE handle, const char* comment,
                  std::vector<char*>* files, void (* __cdecl err)(int, const char*))
{
    unsigned int offset = 0;

    // Header
    {
        ArcHeader header = { ARC_FILE_MAGIC, ARC_FILE_VERSION };

        DWORD bytes = 0;
        if (!WriteFile(handle, &header, sizeof(ArcHeader), &bytes, NULL)
            || bytes != sizeof(ArcHeader))
        {
            if (err != NULL) { err(ARC_FILE_UNABLE_TO_CREATE_FILE, NULL); }

            return ARC_FILE_UNABLE_TO_WRITE_FILE;
        }

        offset += sizeof(ArcHeader);
    }

    // Content
    {
        ArcContent content = { (unsigned int)files->size() };

        if (comment != NULL)
        {
            strcpy_s(content.Comment, ARC_FILE_NAME_LENGTH, comment);
        }

        DWORD bytes = 0;
        if (!WriteFile(handle, &content, sizeof(ArcContent), &bytes, NULL)
            || bytes != sizeof(ArcContent))
        {
            if (err != NULL) { err(ARC_FILE_UNABLE_TO_CREATE_FILE, NULL); }

            return ARC_FILE_UNABLE_TO_WRITE_FILE;
        }

        offset += sizeof(ArcContent);
    }

    // Items
    {
        const unsigned int count = (unsigned int)files->size();

        ArcItem* items = new ArcItem[count];
        ZeroMemory(items, count * sizeof(ArcItem));

        // Write empty items details
        {
            DWORD bytes = 0;
            if (!WriteFile(handle, items, sizeof(ArcItem) * count, &bytes, NULL)
                || bytes != (sizeof(ArcItem) * count))
            {
                if (err != NULL) { err(ARC_FILE_UNABLE_TO_CREATE_FILE, NULL); }

                free(items);
                return ARC_FILE_UNABLE_TO_WRITE_FILE;
            }
        }

        // Archive the file content
        for (unsigned int i = 0; i < count; i++)
        {
            const int result = ArchiveFile(handle, files->at(i), &items[i], err);
            
            if (result != ARC_FILE_OK)
            {
                free(items);
                return result;
            }
        }

        // Set pointer to the beginning of the items section
        if (SetFilePointer(handle, offset, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
        {
            // Update items details section
            DWORD bytes = 0;
            if (!WriteFile(handle, items, sizeof(ArcItem) * count, &bytes, NULL)
                || bytes != (sizeof(ArcItem) * count))
            {
                if (err != NULL) { err(ARC_FILE_UNABLE_TO_CREATE_FILE, NULL); }

                return ARC_FILE_UNABLE_TO_WRITE_FILE;
            }
        }
        else
        {
            if (err != NULL) { err(ARC_FILE_UNABLE_TO_CREATE_FILE, NULL); }

            return ARC_FILE_UNABLE_TO_WRITE_FILE;
        }
    }

    return ARC_FILE_OK;
}

int CreateArcFile(const char* file, const char* path,
                  const char* comment, void (* __cdecl err)(int, const char*))
{
    char bak[MAX_PATH];
    strcpy_s(bak, MAX_PATH, file);
    strcat_s(bak, MAX_PATH, BACKUP_EXTENSION);

    {
        const unsigned int len = (unsigned int)strlen(bak);

        for (unsigned int i = 0; i < len; i++)
        {
            if (bak[i] == '/') { bak[i] = '\\'; }
        }
    }

    // Acquire files to be archived
    std::vector<char*>* files = AcquireFiles(path);

    if (files->size() == 0)
    {
        FreeFiles(files);

        return ARC_FILE_NO_FILES_FOUND;
    }

    // Create the archive file
    HANDLE handle = INVALID_HANDLE_VALUE;

    {
        handle = CreateFileA(bak, GENERIC_READ | GENERIC_WRITE, 0,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        if (handle == INVALID_HANDLE_VALUE)
        {
            if (err != NULL) { err(ARC_FILE_UNABLE_TO_CREATE_FILE, NULL); }

            FreeFiles(files);

            return ARC_FILE_UNABLE_TO_CREATE_FILE;
        }
    }

    // Write the data into the archive file
    const int result = WriteArcFile(handle, comment, files, err);

    CloseHandle(handle);
    FreeFiles(files);

    if (result == ARC_FILE_OK)
    {
        MoveFileExA(bak, file, MOVEFILE_REPLACE_EXISTING);
    }
    else { DeleteFileA(bak); }

    return result;
}

int OpenArcFile(const char* file, ArcFile** archive)
{
    HANDLE handle = CreateFileA(file, GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle == INVALID_HANDLE_VALUE) { return ARC_FILE_NO_FILE; }
    
    ArcHeader header;
    
    {
        DWORD bytes = 0;
        if (!ReadFile(handle, &header, sizeof(ArcHeader), &bytes, NULL)
            || bytes != sizeof(ArcHeader))
        {
            CloseHandle(handle);
            return ARC_FILE_INVALID_CONTENT;
        }
    }

    if (header.Magic != ARC_FILE_MAGIC || header.Version != ARC_FILE_VERSION)
    {
        CloseHandle(handle);
        return ARC_FILE_INVALID_CONTENT;
    }

    ArcContent content;
    ZeroMemory(&content, sizeof(ArcContent));
    
    {
        DWORD bytes = 0;
        if (!ReadFile(handle, &content, sizeof(ArcContent), &bytes, NULL)
            || bytes != sizeof(ArcContent))
        {
            CloseHandle(handle);
            return ARC_FILE_INVALID_CONTENT;
        }
    }

    const DWORD items_size = content.Count * sizeof(ArcItem);
    ArcItem* items = (ArcItem*)malloc(items_size);
    ZeroMemory(items, items_size);

    if (items == NULL)
    {
        free(items);
        CloseHandle(handle);
        return ARC_FILE_UNABLE_TO_EXTRACT_FILE;
    }

    {
        DWORD bytes = 0;
        if (!ReadFile(handle, items, items_size, &bytes, NULL)
            || bytes != items_size)
        {
            free(items);
            CloseHandle(handle);
            return ARC_FILE_INVALID_CONTENT;
        }
    }

    ArcFile* arc = (ArcFile*)malloc(sizeof(ArcFile));
    ZeroMemory(arc, sizeof(ArcFile));

    arc->Handle = handle;
    arc->Items = items;

    CopyMemory(&arc->Descriptor, &content, sizeof(ArcContent));

    *archive = arc;

    return ARC_FILE_OK;
}

BOOL ExtractFile(HANDLE handle, unsigned int size, void *data)
{
    DWORD offset = 0;
    DWORD todo = size;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    // Initialize the zlib stream for decompression
    z_stream strm;
    ZeroMemory(&strm, sizeof(z_stream));

    int result = Z_OK;
    if ((result = inflateInit(&strm)) != Z_OK) { return FALSE; }

    // Decompress
    do
    {
        strm.avail_in = 0;

        {
            DWORD read = min(todo, CHUNK);
            if (!ReadFile(handle, in, read, (LPDWORD)&strm.avail_in, NULL)
                || read != strm.avail_in)
            {
                (void)inflateEnd(&strm);
                return FALSE;
            }

            todo -= read;
        }

        if (strm.avail_in == 0) { break; }

        strm.next_in = in;

        // Execute inflate()
        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;

            switch (result = inflate(&strm, Z_NO_FLUSH))
            {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                case Z_STREAM_ERROR:
                {
                    (void)inflateEnd(&strm);
                    return FALSE;
                }
            }

            unsigned int have = CHUNK - strm.avail_out;

            CopyMemory(((BYTE*)data + offset), out, have);

            offset += have;
        } while (strm.avail_out == 0);
    } while (result != Z_STREAM_END);

    return inflateEnd(&strm) == Z_OK;
}

int AcquireArcFileContent(const ArcFile* archive, const ArcItem* item, void* data)
{
    if (archive == NULL || item == NULL) { return ARC_FILE_NO_FILE; }
    if (data == NULL) { return ARC_FILE_UNABLE_TO_EXTRACT_FILE; }

    if (SetFilePointer(archive->Handle, item->Offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        return ARC_FILE_UNABLE_TO_READ_FILE;
    }

    return ExtractFile(archive->Handle, item->CompressedSize, data)
        ? ARC_FILE_OK : ARC_FILE_UNABLE_TO_EXTRACT_FILE;
}

int CloseArcFile(ArcFile* archive)
{
    if (archive == NULL) { return ARC_FILE_NO_FILE; }

    CloseHandle(archive->Handle);
    free(archive->Items);
    free(archive);

    return ARC_FILE_OK;
}
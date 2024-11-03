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

#include "Assets.h"
#include <Arc.h>

#define MAX_ARCHIVE_FILE_COUNT      16
#define INVALID_ARCHIVE_FILE_INDEX  ((DWORD)(-1))

struct AssetItem
{
    CHAR        Name[MAX_PATH];
    ArcFile*    Content;
};

struct Assets
{
    AssetItem Items[MAX_ARCHIVE_FILE_COUNT];
};

STATIC DWORD AcquireAssetsIndex(LPASSETS lpAssets)
{
    for (DWORD i = 0; i < MAX_ARCHIVE_FILE_COUNT; i++)
    {
        if (lpAssets->Items[i].Content == NULL) { return i; }
    }

    return INVALID_ARCHIVE_FILE_INDEX;
}

STATIC DWORD AcquireAssetsCount(LPASSETS lpAssets)
{
    DWORD result = 0;

    for (DWORD i = 0; i < MAX_ARCHIVE_FILE_COUNT; i++)
    {
        if (lpAssets->Items[i].Content != NULL) { result++; }
    }

    return result;
}

HRESULT CreateAssets(LPCSTR lpszPath, LPASSETS* lpAssets)
{
    if (lpszPath == NULL || lpAssets == NULL) { return AERR_INVALIDPARAMS; }

    CHAR szPattern[MAX_PATH];
    strcpy_s(szPattern, MAX_PATH, lpszPath);
    strcat_s(szPattern, MAX_PATH, "\\*.arc");

    LPASSETS assets = (LPASSETS)malloc(sizeof(ASSETS));

    if (assets == NULL) { return AERR_OUTOFMEMORY; }

    ZeroMemory(assets, sizeof(ASSETS));

    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA(szPattern, &ffd);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        free(assets);
        return AERR_NOFILES;
    }

    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            ArcFile* arc = NULL;
            if (OpenArcFile(ffd.cFileName, &arc) == ARC_FILE_OK)
            {
                CONST DWORD indx = AcquireAssetsIndex(assets);

                if (indx == INVALID_ARCHIVE_FILE_INDEX)
                {
                    CloseArcFile(arc);
                    break;
                }

                assets->Items[indx].Content = arc;
                strcpy_s(assets->Items[indx].Name, MAX_PATH, ffd.cFileName);
            }
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);

    if (AcquireAssetsCount(assets) == 0)
    {
        free(assets);
        return AERR_NOFILES;
    }

    *lpAssets = assets;

    return A_OK;
}

HRESULT GetAssetsContent(LPASSETS lpAssets, LPCSTR lpszName, LPASSETCONTENT lpAssetContent)
{
    if (lpAssets == NULL || lpszName == NULL || lpAssetContent == NULL) { return AERR_INVALIDPARAMS; }

    for (DWORD i = 0; i < MAX_ARCHIVE_FILE_COUNT; i++)
    {
        ArcFile* item = lpAssets->Items[i].Content;

        if (item == NULL) { continue; }

        for (DWORD k = 0; k < item->Descriptor.Count; k++)
        {
            if (_strcmpi(lpszName, item->Items[k].Name) == 0)
            {
                lpAssetContent->Size = item->Items[k].UncompressedSize;
                lpAssetContent->State = &item->Items[k];

                return A_OK;
            }
        }
    }

    return AERR_NOFILES;
}

HRESULT GetAssetsContentContent(LPASSETS lpAssets, LPASSETCONTENT lpAssetContent, LPVOID lpContent)
{
    if (lpAssets == NULL || lpAssetContent == NULL || lpContent == NULL) { return AERR_INVALIDPARAMS; }

    for (DWORD i = 0; i < MAX_ARCHIVE_FILE_COUNT; i++)
    {
        ArcFile* item = lpAssets->Items[i].Content;

        if (item == NULL) { continue; }

        for (DWORD k = 0; k < item->Descriptor.Count; k++)
        {
            if (_strcmpi(item->Items[k].Name, ((const ArcItem*)lpAssetContent->State)->Name) == 0)
            {
                return AcquireArcFileContent(item, ((const ArcItem*)lpAssetContent->State), lpContent) == ARC_FILE_OK
                    ? A_OK : AERR_NOFILES;
            }
        }
    }

    return AERR_NOFILES;
}

HRESULT ReleaseAssets(LPASSETS lpAssets)
{
    if (lpAssets == NULL) { return AERR_INVALIDPARAMS; }

    for (int i = 0; i < MAX_ARCHIVE_FILE_COUNT; i++)
    {
        if (lpAssets->Items[i].Content != NULL)
        {
            CloseArcFile(lpAssets->Items[i].Content);
            ZeroMemory(&lpAssets->Items[i], sizeof(AssetItem));
        }
    }

    free(lpAssets);

    return A_OK;
}
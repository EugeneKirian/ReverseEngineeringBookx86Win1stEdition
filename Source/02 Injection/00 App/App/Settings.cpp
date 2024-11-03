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

#include "Settings.h"

#include <stdio.h>

HRESULT AcquireSettingsValue(LPCSTR lpszContent, LPCSTR lpszName, LPSTR lpszValue)
{
    LPSTR lpszStart = StrStrIA(lpszContent, lpszName);

    if (lpszStart == NULL) { return SERR_GENERIC; }

    lpszStart = lpszStart + strlen(lpszName);

    while (*lpszStart != NULL)
    {
        if (*lpszStart == '=' || *lpszStart == ' ' || *lpszStart == '\t')
        {
            lpszStart++;
        }
        else { break; }
    }

    if (*lpszStart == NULL) { return SERR_GENERIC; }

    LPSTR lpszDst = lpszValue;
    while (*lpszStart != NULL && *lpszStart != '\r' && *lpszStart != '\n')
    {
        *lpszDst = *lpszStart;

        lpszStart++;
        lpszDst++;
    }

    *lpszDst = NULL;

    return S_OK;
}

HRESULT AcquireSettingsV3(LPCSTR lpszInput, LPV3 lpV3)
{
    if (lpszInput == NULL || lpV3 == NULL) { return E_FAIL; }

    FLOAT fX = 0, fY = 0, fZ = 0;
    if (sscanf_s(lpszInput, "%f %f %f", &fX, &fY, &fZ) != 3) { return E_FAIL; }

    lpV3->X = fX;
    lpV3->Y = fY;
    lpV3->Z = fZ;

    return S_OK;
}

HRESULT AcquireSettingsXRGB(LPCSTR lpszInput, LPDWORD lpdwColor)
{
    if (lpszInput == NULL || lpdwColor == NULL) { return E_FAIL; }

    DWORD dwRed = 0, dwGreen = 0, dwBlue = 0;
    if (sscanf_s(lpszInput, "%u %u %u", &dwRed, &dwGreen, &dwBlue) != 3) { return E_FAIL; }

    *lpdwColor = D3DCOLOR_XRGB(min(255, dwRed), min(255, dwGreen), min(255, dwBlue));

    return S_OK;
}


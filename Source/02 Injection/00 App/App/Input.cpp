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

#include "Input.h"

Input::Input()
{
    ZeroMemory(this->Current, 256);
    ZeroMemory(this->Prior, 256);

    ZeroMemory(this->Mouse, INPUT_MOUSE_COUNT * sizeof(BOOL));
    ZeroMemory(this->InitialX, INPUT_MOUSE_COUNT * sizeof(INT));
    ZeroMemory(this->InitialY, INPUT_MOUSE_COUNT * sizeof(INT));
    ZeroMemory(this->CurrentX, INPUT_MOUSE_COUNT * sizeof(INT));
    ZeroMemory(this->CurrentY, INPUT_MOUSE_COUNT * sizeof(INT));
}

VOID Input::Update()
{
    BYTE state[256];

    for (DWORD i = 0; i < 256; i++)
    {
        state[i] = (BYTE)(GetAsyncKeyState(i) & 0x01);
    }

    CopyMemory(this->Prior, this->Current, 256);
    CopyMemory(this->Current, state, 256);
}

BOOL Input::IsPressing(CONST INT nKey)
{
    return this->Current[nKey];
}

BOOL Input::IsPressed(CONST INT nKey)
{
    return !this->Current[nKey] && this->Prior[nKey];
}

BOOL Input::IsMouse(CONST INT nMouse)
{
    return this->Mouse[nMouse];
}

VOID Input::SetMouse(CONST INT nMouse, CONST BOOL bPressed, CONST INT nX, CONST INT nY)
{
    if (!this->Mouse[nMouse] && bPressed)
    {
        this->Mouse[nMouse] = TRUE;
        this->InitialX[nMouse] = this->CurrentX[nMouse] = nX;
        this->InitialY[nMouse] = this->CurrentY[nMouse] = nY;
    }
    else if (bPressed)
    {
        this->CurrentX[nMouse] = nX;
        this->CurrentY[nMouse] = nY;
    }
    else
    {
        this->Mouse[nMouse] = FALSE;
        this->InitialX[nMouse] = this->CurrentX[nMouse] = 0;
        this->InitialY[nMouse] = this->CurrentY[nMouse] = 0;
    }
}

VOID Input::GetMouse(CONST INT nMouse, CONST BOOL bReset, LPINT lpnX, LPINT lpnY)
{
    if (lpnX != NULL)
    {
        *lpnX = this->CurrentX[nMouse] - this->InitialX[nMouse];
    }

    if (lpnY != NULL)
    {
        *lpnY = this->CurrentY[nMouse] - this->InitialY[nMouse];
    }

    if (bReset)
    {
        this->InitialX[nMouse] = this->CurrentX[nMouse];
        this->InitialY[nMouse] = this->CurrentY[nMouse];
    }
}
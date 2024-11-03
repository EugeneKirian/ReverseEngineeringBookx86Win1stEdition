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

#include "Base.h"

#define INPUT_LEFT_MOUSE    0
#define INPUT_RIGHT_MOUSE   1
#define INPUT_MOUSE_COUNT   (INPUT_RIGHT_MOUSE + 1)

class Input
{
public:
    Input();
    VOID Update();
    BOOL IsPressing(CONST INT nKey);
    BOOL IsPressed(CONST INT nKey);
    BOOL IsMouse(CONST INT nMouse);
    VOID SetMouse(CONST INT nMouse, CONST BOOL bPressed, CONST INT nX, CONST INT nY);
    VOID GetMouse(CONST INT nMouse, CONST BOOL bReset, LPINT lpnX, LPINT lpnY);
protected:
    BYTE    Current[256];
    BYTE    Prior[256];
    
    BOOL    Mouse[INPUT_MOUSE_COUNT];
    INT     InitialX[INPUT_MOUSE_COUNT];
    INT     InitialY[INPUT_MOUSE_COUNT];
    INT     CurrentX[INPUT_MOUSE_COUNT];
    INT     CurrentY[INPUT_MOUSE_COUNT];
};

typedef Input INPUT, * LPINPUT;
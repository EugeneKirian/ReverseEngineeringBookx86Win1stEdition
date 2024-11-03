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

#define OBJ_OK                  0
#define OBJERR_GENERIC          1
#define OBJERR_INVALIDPARAMS    2
#define OBJERR_OUTOFMEMORY      3
#define OBJERR_INVALIDFORMAT    4

typedef float FLOAT;
typedef char* LPCHAR;

typedef struct ObjV2
{
    FLOAT X, Y;
} OBJV2, * LPOBJV2;

typedef struct ObjV3
{
    FLOAT X, Y, Z;
} OBJV3, * LPOBJV3;

typedef struct ObjVertex
{
    OBJV3   Coord;
    OBJV3   Normal;
    OBJV2   UV;
} OBJVERTEX, * LPOBJVERTEX;

typedef struct ObjTriangle
{
    OBJVERTEX A, B, C;
} OBJTRIANGLE, * LPOBJTRIANGLE;

typedef struct ObjObject
{
    DWORD           Count;
    LPOBJTRIANGLE   Triangles;
} OBJOBJECT, * LPOBJOBJECT;

int CreateObjObject(LPCHAR lpInput, DWORD dwLength, LPOBJOBJECT* lplpObj);
int ReleaseObjObject(LPOBJOBJECT lpObj);
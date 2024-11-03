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

#include "Obj.h"

#include <vector>

#define MAX_TOKEN_LENGTH    128

typedef struct ObjState
{
    std::vector<OBJV3>          Coords;
    std::vector<OBJV3>          Norms;
    std::vector<OBJV2>          UVs;
    std::vector<OBJTRIANGLE>    Triangles;
} OBJSTATE, * LPOBJSTATE;

static int CreateObjV2(CONST LPCHAR lpInput, LPOBJV2 lpV2)
{
    CHAR token[MAX_TOKEN_LENGTH];
    return sscanf(lpInput, "%s %f %f", token, &lpV2->X, &lpV2->Y) == 3
        ? OBJ_OK : OBJERR_INVALIDFORMAT;
}

static int CreateObjV3(CONST LPCHAR lpInput, LPOBJV3 lpV3)
{
    CHAR token[MAX_TOKEN_LENGTH];
    return sscanf(lpInput, "%s %f %f %f", token, &lpV3->X, &lpV3->Y, &lpV3->Z) == 4
        ? OBJ_OK : OBJERR_INVALIDFORMAT;
}

static int CreateObjTriangle(CONST LPCHAR lpInput, LPOBJSTATE lpState, LPOBJTRIANGLE lpTriangle)
{
    SIZE_T ac, at, an, bc, bt, bn, cc, ct, cn;
    CHAR token[MAX_TOKEN_LENGTH];

    if (sscanf(lpInput, "%s %u/%u/%u %u/%u/%u %u/%u/%u", token, &ac, &at, &an, &bc, &bt, &bn, &cc, &ct, &cn) != 10) { return OBJERR_INVALIDFORMAT; }

    CONST SIZE_T coords = lpState->Coords.size();
    if (ac < 1 || coords < ac || bc < 1 || coords < bc || cc < 1 || coords < cc) { return OBJERR_INVALIDFORMAT; }

    CONST SIZE_T norms = lpState->Norms.size();
    if (an < 1 || norms < an || bn < 1 || norms < bn || cn < 1 || norms < cn) { return OBJERR_INVALIDFORMAT; }

    CONST SIZE_T uvs = lpState->UVs.size();
    if (at < 1 || uvs < at || bt < 1 || uvs < bt || ct < 1 || uvs < ct) { return OBJERR_INVALIDFORMAT; }

    lpTriangle->A.Coord = lpState->Coords[ac - 1];
    lpTriangle->A.Normal = lpState->Norms[an - 1];
    lpTriangle->A.UV = lpState->UVs[at - 1];

    lpTriangle->B.Coord = lpState->Coords[bc - 1];
    lpTriangle->B.Normal = lpState->Norms[bn - 1];
    lpTriangle->B.UV = lpState->UVs[bt - 1];

    lpTriangle->C.Coord = lpState->Coords[cc - 1];
    lpTriangle->C.Normal = lpState->Norms[cn - 1];
    lpTriangle->C.UV = lpState->UVs[ct - 1];

    return OBJ_OK;
}

// Note: Input is expected to be null-terminated.
int CreateObjObject(LPCHAR lpszInput, DWORD dwLength, LPOBJOBJECT* lplpObj)
{
    if (lpszInput == NULL || dwLength == 0 || lplpObj == NULL) { return OBJERR_INVALIDPARAMS; }

    LPCHAR current = lpszInput;
    LPCHAR end = lpszInput + dwLength;

    OBJSTATE state;

    while (current != NULL && current < end)
    {
        while (*current == ' ' || *current == '\n')
        {
            current++;

            if (end <= current) { break; }
        }

        switch (*current)
        {
        case 'v':
        case 'V':
            {
                if (end <= current + 1) { return OBJERR_INVALIDFORMAT; }

                int result = OBJ_OK;
                switch (*(current + 1))
                {
                case 't':
                case 'T':
                    {
                        OBJV2 v2;
                        if ((result = CreateObjV2(current, &v2)) != OBJ_OK) { return result; }

                        state.UVs.push_back(v2);
                        break;
                    }
                case 'n':
                case 'N':
                    {
                        OBJV3 v3;
                        if ((result = CreateObjV3(current, &v3)) != OBJ_OK) { return result; }

                        state.Norms.push_back(v3);
                        break;
                    }
                default:
                    {
                        OBJV3 v3;
                        if ((result = CreateObjV3(current, &v3)) != OBJ_OK) { return result; }

                        state.Coords.push_back(v3);
                        break;
                    }
                }

                break;
            }
        case 'f':
        case 'F':
            {
                int result = OBJ_OK;
                OBJTRIANGLE triangle;
                if ((result = CreateObjTriangle(current, &state, &triangle)) != OBJ_OK) { return result; }

                state.Triangles.push_back(triangle);
                break;
            }
        }

        current = strchr(current, '\n');
    }

    {
        LPOBJOBJECT object = (LPOBJOBJECT)malloc(sizeof(OBJOBJECT));

        if (object == NULL) { return OBJERR_OUTOFMEMORY; }

        object->Count = (DWORD)state.Triangles.size();

        LPOBJTRIANGLE triangles = (LPOBJTRIANGLE)malloc(object->Count * sizeof(OBJTRIANGLE));

        if (triangles == NULL) { return OBJERR_OUTOFMEMORY; }

        object->Triangles = triangles;

        std::copy(state.Triangles.begin(), state.Triangles.end(), object->Triangles);

        *lplpObj = object;
    }

    return OBJ_OK;
}

int ReleaseObjObject(LPOBJOBJECT lpObj)
{
    if (lpObj == NULL) { return OBJERR_INVALIDPARAMS; }

    if (lpObj->Triangles != NULL)
    {
        free(lpObj->Triangles);
    }

    free(lpObj);

    return OBJ_OK;
}
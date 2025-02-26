/*
Copyright (c) 2025 Eugene Kirian

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

#include "Common.h"

#include <windows.h>

extern HMODULE exe;
extern HMODULE lib;

typedef int (*PFBGETASSETSCONTENT)(void* assets,
                                   char* name, AssetContent* content);

struct LibFunctions
{
    PFBGETASSETSCONTENT GetAssetsContent;
};

extern LibFunctions LibFuncs;

int CreateGameImplementation(Game* self, char* name);

#pragma pack(push, 1)
struct BulletStruct
{
    void*   Self;
    char    Unk00[0x8C];
    int     Health;
    int     MaxHealth;
    char    Unk01[0x64];
};
#pragma pack(pop)

void BulletStructSetHealth(BulletStruct* self, int value);
void BulletStructSetMaxHealth(BulletStruct* self, int value);
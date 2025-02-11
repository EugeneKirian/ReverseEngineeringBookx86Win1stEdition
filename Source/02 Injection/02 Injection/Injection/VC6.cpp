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

#include "Configuration.h"
#include "Wrapper.h"

#include <stdlib.h>

#ifdef VC6

// Constructor              0x00401e00
// Virtual Function Table   0x0040f1d0
//struct BulletStruct

// 0x0040f1d0 + 0x20 (8)
// 0x00401300
void __cdecl BulletStructSetHealth(BulletStruct* self, int value)
{
    self->Health = 10000000;
}

// 0x0040f1d0 + 0x28 (10)
// 0x00401330
void __cdecl BulletStructSetMaxHealth(BulletStruct* self, int value)
{
    self->MaxHealth = 10000000;
}

typedef int (* FUN_00404540PTR)(void* assets, void* p2,
                                void* p3, AssetContent* content, void* p5);

int CreateGameImplementation(Game* self, char* name)
{
    if (self == NULL) { return 0x80070057; }

    char scene[MAX_PATH];
    strcpy(scene, strcmp(name, "scene1.scene") == 0 ? "cube.scene" : name);

    AssetContent content;

    int result = LibFuncs.GetAssetsContent(self->Assets, scene, &content);

    if (-1 < result)
    {
        // Calculate the function address.
        FUN_00404540PTR FUN_00404540 =
            (FUN_00404540PTR)((unsigned)exe + (0x00404540 - 0x00400000));

        result = FUN_00404540(self->Assets, self->Unk03,
            self->Unk02, &content, &self->Unk04);
    }

    return result;
}

#endif
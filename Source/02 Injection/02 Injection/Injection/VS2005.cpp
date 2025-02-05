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

#ifdef VS2005

// 0x004046D0
int FUN_004046D0(void* assets, void* p2,
                 void* p3, AssetContent* content, void* p5)
{
    int result = 0;

    unsigned addr = (unsigned)exe + (0x004046D0 - 0x00400000);

    // One of the parameters is passed through register EAX.

    __asm
    {
        push p5;
        push p3;
        push p2;
        push assets;
        mov eax, content;

        mov ebx, addr;
        call ebx;
        add esp, 0x10;

        mov result, eax;
    };

    return result;
}

// 0x00403090
int CreateGameImplementation(Game* self, char* name)
{
    if (self == NULL) { return 0x80070057; }

    char scene[MAX_PATH];
    strcpy(scene, strcmp(name, "scene1.scene") == 0 ? "cube.scene" : name);

    AssetContent content;

    int result = LibFuncs.GetAssetsContent(self->Assets, name, &content);

      if (-1 < result)
      {
          result = FUN_004046D0(self->Assets, self->Unk03,
              self->Unk02, &content, &self->Unk04);
      }

      return result;
}

#endif
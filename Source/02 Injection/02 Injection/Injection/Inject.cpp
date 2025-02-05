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
#include "Inject.h"
#include "Wrapper.h"

#include <detours.h>

#define BASE_ADDRESS        0x00400000

#ifdef VS2005
#define CREATE_GAME_ADDRESS 0x00403090

// Parameters are passed in registers ESI & ECX.
__declspec(naked) void CreateGameWrapper(void)
{
    __asm
    {
        push ecx;
        push esi;

        call CreateGameImplementation;
        add  esp, 0x8;
        ret;          // Return from the function!
    };
}

#else
#define CREATE_GAME_ADDRESS 0x004030A0

// Parameters are passed on stack.
int CreateGameWrapper(Game* self, char* name)
{
    // Note. The detour can point directly to
    // the implementation function, given there
    // is no need to transform the arguments storage.

   return CreateGameImplementation(self, name);
}
#endif

void* CreateGameFunction = NULL;

BOOL IsInjectionAttached()
{
    return DetourIsHelperProcess();
}

BOOL InitializeWrapper()
{
    exe = GetModuleHandleA(NULL);
    lib = LoadLibraryA("lib.dll");

    if (lib == NULL) { return FALSE; }

    LibFuncs.GetAssetsContent =
        (PFBGETASSETSCONTENT)GetProcAddress(lib, "GetAssetsContent");

    return TRUE;
}

BOOL InitializeInjection()
{
    if(!InitializeWrapper()) { return FALSE; }

    DetourTransactionBegin();

    CreateGameFunction =
        (void*)((unsigned)exe + CREATE_GAME_ADDRESS - BASE_ADDRESS);

    DetourAttach(&(PVOID)CreateGameFunction, CreateGameWrapper);

    return DetourTransactionCommit() == NO_ERROR;
}

BOOL ReleaseInjection()
{
    DetourTransactionBegin();

    if (CreateGameFunction != NULL)
    {
        DetourDetach(&(PVOID)CreateGameFunction, CreateGameWrapper);
    }

    return DetourTransactionCommit() == NO_ERROR;
}
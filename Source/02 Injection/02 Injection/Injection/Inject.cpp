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

#include "BulletClass.h"
#include "Configuration.h"
#include "Inject.h"
#include "Wrapper.h"

#include <detours.h>

#define BASE_ADDRESS                    0x00400000

#ifdef VS2005
#define CREATE_GAME_ADDRESS             0x00403090
#define SET_HEALTH_ADDRESS              0x004012E0
#define SET_MAX_HEALTH_ADDRESS          0x00401310
#define VIRTUAL_FUNCTION_TABLE_ADDRESS  0x0040FA34

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
#define CREATE_GAME_ADDRESS             0x004030A0
#define SET_HEALTH_ADDRESS              0x00401300
#define SET_MAX_HEALTH_ADDRESS          0x00401330
#define VIRTUAL_FUNCTION_TABLE_ADDRESS  0x0040F1D0

// Parameters are passed on stack.
int CreateGameWrapper(Game* self, char* name)
{
    // Note. The detour can point directly to
    // the implementation function, given there
    // is no need to transform the arguments storage.

   return CreateGameImplementation(self, name);
}
#endif

__declspec(naked) void SetHealthWrapper(void)
{
    __asm
    {
        pop eax;
        pop edx;  // Cannot use ebx in this case

        push eax; // Return Address
        push edx;
        push ecx;

        call BulletStructSetHealth;

        add esp, 0x8;
        
        ret;
    }
}

__declspec(naked) void SetMaxHealthWrapper(void)
{
    __asm
    {
        pop eax;
        pop edx;

        push eax; // Return Address
        push edx;
        push ecx;

        call BulletStructSetMaxHealth;

        add esp, 0x8;

        ret;
    }
}

void* CreateGameFunction = NULL;
void* SetHealthFunction = NULL;
void* SetMaxHealthFunction = NULL;

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

void (BulletClass:: * BulletClassSetHealth)(int) = &BulletClass::SetHealth;
void (BulletClass:: * BulletClassSetMaxHealth)(int) = &BulletClass::SetMaxHealth;

BOOL InitializeVirtualFunctions()
{
    // Bullet Virtual Function Table
    void** vfptr = (void**)
        ((unsigned)exe + VIRTUAL_FUNCTION_TABLE_ADDRESS - BASE_ADDRESS);

    // Change Memory Access Permissions
    DWORD ex = 0;
    if(!VirtualProtect(vfptr, 24 * sizeof(void*), PAGE_READWRITE, &ex))
    {
        return FALSE;
    }

    // SetHealth & SetMaxHealth

    //vfptr[8]  = SetHealthWrapper;
    //vfptr[10] = SetMaxHealthWrapper;

    vfptr[8]    = *(void**)&BulletClassSetHealth;
    vfptr[10]   = *(void**)&BulletClassSetMaxHealth;

    return TRUE;
}

BOOL InitializeInjection()
{
    if(!InitializeWrapper()) { return FALSE; }

    if(!InitializeVirtualFunctions()) { return FALSE; }

    DetourTransactionBegin();

    CreateGameFunction =
        (void*)((unsigned)exe + CREATE_GAME_ADDRESS - BASE_ADDRESS);

    DetourAttach(&(PVOID)CreateGameFunction, CreateGameWrapper);

    SetHealthFunction =
        (void*)((unsigned)exe + SET_HEALTH_ADDRESS - BASE_ADDRESS);
    SetMaxHealthFunction =
        (void*)((unsigned)exe + SET_MAX_HEALTH_ADDRESS - BASE_ADDRESS);

    //DetourAttach(&(PVOID)SetHealthFunction, SetHealthWrapper);
    //DetourAttach(&(PVOID)SetMaxHealthFunction, SetMaxHealthWrapper);

    //DetourAttach(&(PVOID)SetHealthFunction, *(PVOID*)&BulletClassSetHealth);
    //DetourAttach(&(PVOID)SetMaxHealthFunction, *(PVOID*)&BulletClassSetMaxHealth);

    return DetourTransactionCommit() == NO_ERROR;
}

BOOL ReleaseInjection()
{
    DetourTransactionBegin();

    if (CreateGameFunction != NULL)
    {
        DetourDetach(&(PVOID)CreateGameFunction, CreateGameWrapper);
        //DetourDetach(&(PVOID)SetHealthFunction, SetHealthWrapper);
        //DetourDetach(&(PVOID)SetMaxHealthFunction, SetMaxHealthWrapper);
        
        //DetourDetach(&(PVOID)SetHealthFunction, *(PVOID*)&BulletClassSetHealth);
        //DetourDetach(&(PVOID)SetMaxHealthFunction, *(PVOID*)&BulletClassSetMaxHealth);
    }

    return DetourTransactionCommit() == NO_ERROR;
}
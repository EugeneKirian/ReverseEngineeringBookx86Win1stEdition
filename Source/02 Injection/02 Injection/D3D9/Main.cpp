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

#include "Inject.h"

#define STATIC  static

struct
{
    HMODULE Module;
    FARPROC CheckFullscreen;
    FARPROC Direct3DShaderValidatorCreate9;
    FARPROC PSGPError;
    FARPROC PSGPSampleTexture;
    FARPROC D3DPERF_BeginEvent;
    FARPROC D3DPERF_EndEvent;
    FARPROC D3DPERF_GetStatus;
    FARPROC D3DPERF_QueryRepeatFrame;
    FARPROC D3DPERF_SetMarker;
    FARPROC D3DPERF_SetOptions;
    FARPROC D3DPERF_SetRegion;
    FARPROC DebugSetLevel;
    FARPROC DebugSetMute;
    FARPROC Direct3DCreate9;
} D3D9;

STATIC BOOL InitializeD3D9()
{
    CHAR path[MAX_PATH];
    if (GetSystemDirectoryA(path, MAX_PATH) == 0) { return FALSE; }

    strcat_s(path, MAX_PATH, "\\d3d9.dll");

    D3D9.Module = LoadLibraryA(path);

    if (D3D9.Module == NULL) { return FALSE; }

    D3D9.CheckFullscreen = GetProcAddress(D3D9.Module, "CheckFullscreen");
    D3D9.Direct3DShaderValidatorCreate9 =
        GetProcAddress(D3D9.Module, "Direct3DShaderValidatorCreate9");
    D3D9.PSGPError = GetProcAddress(D3D9.Module, "PSGPError");
    D3D9.PSGPSampleTexture = GetProcAddress(D3D9.Module, "PSGPSampleTexture");
    D3D9.D3DPERF_BeginEvent = GetProcAddress(D3D9.Module, "D3DPERF_BeginEvent");
    D3D9.D3DPERF_EndEvent = GetProcAddress(D3D9.Module, "D3DPERF_EndEvent");
    D3D9.D3DPERF_GetStatus = GetProcAddress(D3D9.Module, "D3DPERF_GetStatus");
    D3D9.D3DPERF_QueryRepeatFrame =
        GetProcAddress(D3D9.Module, "D3DPERF_QueryRepeatFrame");
    D3D9.D3DPERF_SetMarker = GetProcAddress(D3D9.Module, "D3DPERF_SetMarker");
    D3D9.D3DPERF_SetOptions = GetProcAddress(D3D9.Module, "D3DPERF_SetOptions");
    D3D9.D3DPERF_SetRegion = GetProcAddress(D3D9.Module, "D3DPERF_SetRegion");
    D3D9.DebugSetLevel = GetProcAddress(D3D9.Module, "DebugSetLevel");
    D3D9.DebugSetMute = GetProcAddress(D3D9.Module, "DebugSetMute");
    D3D9.Direct3DCreate9 = GetProcAddress(D3D9.Module, "Direct3DCreate9");

    return TRUE;
}

STATIC VOID ReleaseD3D9()
{
    if (D3D9.Module != NULL)
    {
        FreeLibrary(D3D9.Module);
        D3D9.Module = NULL;
    }
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
    if (IsInjectionAttached()) { return TRUE; }

    switch(dwReason) 
    { 
        case DLL_PROCESS_ATTACH:
        {
            if (!InitializeD3D9()) { return FALSE; }

            if (!InitializeInjection()) { return FALSE; }

            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH: { break; }
        case DLL_PROCESS_DETACH:
        {
            if (lpReserved != NULL) { break; }

            ReleaseInjection();
            
            ReleaseD3D9();

            break;
        }
    }

    return TRUE;
}

__declspec(naked) void CheckFullscreen() { _asm { jmp[D3D9.CheckFullscreen] } }
__declspec(naked) void Direct3DShaderValidatorCreate9() { _asm { jmp[D3D9.Direct3DShaderValidatorCreate9] } }
__declspec(naked) void PSGPError() { _asm { jmp[D3D9.PSGPError] } }
__declspec(naked) void PSGPSampleTexture() { _asm { jmp[D3D9.PSGPSampleTexture] } }
__declspec(naked) void D3DPERF_BeginEvent() { _asm { jmp[D3D9.D3DPERF_BeginEvent] } }
__declspec(naked) void D3DPERF_EndEvent() { _asm { jmp[D3D9.D3DPERF_EndEvent] } }
__declspec(naked) void D3DPERF_GetStatus() { _asm { jmp[D3D9.D3DPERF_GetStatus] } }
__declspec(naked) void D3DPERF_QueryRepeatFrame() { _asm { jmp[D3D9.D3DPERF_QueryRepeatFrame] } }
__declspec(naked) void D3DPERF_SetMarker() { _asm { jmp[D3D9.D3DPERF_SetMarker] } }
__declspec(naked) void D3DPERF_SetOptions() { _asm { jmp[D3D9.D3DPERF_SetOptions] } }
__declspec(naked) void D3DPERF_SetRegion() { _asm { jmp[D3D9.D3DPERF_SetRegion] } }
__declspec(naked) void DebugSetLevel() { _asm { jmp[D3D9.DebugSetLevel] } }
__declspec(naked) void DebugSetMute() { _asm { jmp[D3D9.DebugSetMute] } }
__declspec(naked) void Direct3DCreate9() { _asm { jmp[D3D9.Direct3DCreate9] } }
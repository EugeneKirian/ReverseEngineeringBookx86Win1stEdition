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

#define INVALID_RENDERER_HANDLE             0xFFFFFF

#define MAX_RENDERER_TEXTURE_COUNT          512
#define MAX_RENDERER_VERTEXBUFFER_COUNT     512

#define R_OK                    S_OK
#define RERR_GENERIC            E_FAIL
#define RERR_INVALIDPARAMS      E_INVALIDARG
#define RERR_OUTOFMEMORY        E_OUTOFMEMORY

typedef struct RendererMesh
{
    DWORD Vertex;
    DWORD VertexCount;
    DWORD VertexFormat;
    DWORD VertexSize;
    DWORD Texture;
} RENDERERMESH, * LPRENDERERMESH;

typedef struct Renderer
{
    struct
    {
        HWND                HWND;
        INT                 Width;
        INT                 Height;
    } Window;

    LPDIRECT3D9             D3D;
    D3DPRESENT_PARAMETERS   PresentParameters;
    LPDIRECT3DDEVICE9       Device;
    LPDIRECT3DVERTEXBUFFER9 VertexBuffers[MAX_RENDERER_VERTEXBUFFER_COUNT];
    LPDIRECT3DTEXTURE9      Textures[MAX_RENDERER_TEXTURE_COUNT];
} RENDERER, * LPRENDERER;

HRESULT CreateRenderer(HWND hWnd, BOOL bWindow, DWORD dwWidth, DWORD dwHeight, LPRENDERER* lpRenderer);
HRESULT ReleaseRenderer(LPRENDERER lpRndr);

HRESULT BeginRendererScene(LPRENDERER lpRndr, DWORD dwColor);
HRESULT EndRendererScene(LPRENDERER lpRndr);

HRESULT SetRendererCullMode(LPRENDERER lpRndr, DWORD dwMode);
HRESULT SetRendererMatrixes(LPRENDERER lpRndr, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProjection);

HRESULT RenderRendererMesh(LPRENDERER lpRndr, LPRENDERERMESH lpMesh);

HRESULT CreateRendererTexture(LPRENDERER lpRndr, DWORD dwWidth, DWORD dwHeight, DWORD dwLevels, D3DFORMAT ddFormat, LPDWORD lpdwTexture);
HRESULT UpdateRendererTexture(LPRENDERER lpRndr, DWORD dwTexture, LPRECT lpRect, LPBYTE lpContent, DWORD dwStride);
HRESULT ReleaseRendererTexture(LPRENDERER lpRndr, DWORD dwTexture);

HRESULT CreateRendererVertexBuffer(LPRENDERER lpRndr, DWORD dwSize, DWORD dwCount, DWORD dwFormat, LPDWORD lpdwVB);
HRESULT UpdateRendererVertexBuffer(LPRENDERER lpRndr, DWORD dwVB, DWORD dwSize, LPBYTE lpContent);
HRESULT ReleaseRendererVertexBuffer(LPRENDERER lpRndr, DWORD dwVB);
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

#include "Renderer.h"

HRESULT CreateRenderer(HWND hWnd, BOOL bWindow, DWORD dwWidth, DWORD dwHeight, LPRENDERER* lplpRenderer)
{
    if (hWnd == NULL || dwWidth == 0 || dwHeight == 0
        || lplpRenderer == NULL || !IsWindow(hWnd)) { return RERR_INVALIDPARAMS; }

    LPRENDERER instance = (LPRENDERER)malloc(sizeof(RENDERER));
    ZeroMemory(instance, sizeof(RENDERER));

    if (instance == NULL) { return RERR_OUTOFMEMORY; }

    instance->Window.HWND = hWnd;
    instance->Window.Width = dwWidth;
    instance->Window.Height = dwHeight;

    if (NULL == (instance->D3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        free(instance);
        return RERR_GENERIC;
    }

    D3DDISPLAYMODE d3ddMode;
    if (FAILED(instance->D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddMode)))
    {
        instance->D3D->Release();
        free(instance);
        return RERR_GENERIC;
    }

    instance->PresentParameters.Windowed = bWindow;

    if (bWindow)
    {
        instance->PresentParameters.BackBufferWidth = dwWidth;
        instance->PresentParameters.BackBufferHeight = dwHeight;

        instance->PresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
        instance->PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    }
    else
    {
        instance->PresentParameters.BackBufferWidth = d3ddMode.Width;
        instance->PresentParameters.BackBufferHeight = d3ddMode.Height;

        D3DFORMAT d3dFormat = D3DFMT_A8R8G8B8;
        if (FAILED(instance->D3D->CheckDeviceType(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL, d3ddMode.Format, d3dFormat, FALSE)))
        {
            d3dFormat = D3DFMT_X8R8G8B8;
            if (FAILED(instance->D3D->CheckDeviceType(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL, d3ddMode.Format, d3dFormat, FALSE)))
            {
                d3dFormat = D3DFMT_R5G6B5;
                if (FAILED(instance->D3D->CheckDeviceType(D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL, d3ddMode.Format, d3dFormat, FALSE)))
                {
                    instance->D3D->Release();
                    free(instance);
                    return RERR_GENERIC;
                }
            }
        }
        
        instance->PresentParameters.BackBufferFormat = d3dFormat;
        instance->PresentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;
        instance->PresentParameters.FullScreen_RefreshRateInHz = d3ddMode.RefreshRate;
        instance->PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    }

    instance->PresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    instance->PresentParameters.hDeviceWindow = hWnd;
    instance->PresentParameters.EnableAutoDepthStencil = TRUE;
    instance->PresentParameters.AutoDepthStencilFormat = D3DFMT_D16;

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    if (FAILED(instance->D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                    hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                    &instance->PresentParameters, &instance->Device)))
    {
        instance->D3D->Release();
        free(instance);
        return RERR_GENERIC;
    }

    instance->Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    instance->Device->SetRenderState(D3DRS_LIGHTING, FALSE);
    instance->Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    instance->Device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
    instance->Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    instance->Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
    instance->Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

    *lplpRenderer = instance;

    return R_OK;
}

HRESULT ReleaseRenderer(LPRENDERER lpRndr)
{
    for (DWORD i = 0; i < MAX_RENDERER_TEXTURE_COUNT; i++)
    {
        if (lpRndr->Textures[i] != NULL)
        {
            while (lpRndr->Textures[i]->Release() != 0) { }

            lpRndr->Textures[i] = NULL;
        }
    }

    for (DWORD i = 0; i < MAX_RENDERER_VERTEXBUFFER_COUNT; i++)
    {
        if (lpRndr->VertexBuffers[i] != NULL)
        {
            while (lpRndr->VertexBuffers[i]->Release() != 0) { }

            lpRndr->VertexBuffers[i] = NULL;
        }
    }

    if (lpRndr->Device != NULL) { lpRndr->Device->Release(); lpRndr->Device = NULL; }
    if (lpRndr->D3D != NULL) { lpRndr->D3D->Release(); lpRndr->D3D = NULL; }

    return R_OK;
}

HRESULT BeginRendererScene(LPRENDERER lpRndr, DWORD dwColor)
{
    if (lpRndr == NULL) { return RERR_INVALIDPARAMS; }

    HRESULT result = D3D_OK;
    if (FAILED(result = lpRndr->Device->TestCooperativeLevel()))
    {
        if (result == D3DERR_DEVICENOTRESET)
        {
            if (FAILED(result = lpRndr->Device->Reset(&lpRndr->PresentParameters))) { return result; }
        }
        else { return result; }
    }

    lpRndr->Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, dwColor, 1.0f, 0);

    return lpRndr->Device->BeginScene();
}

HRESULT EndRendererScene(LPRENDERER lpRndr)
{
    if (lpRndr == NULL) { return RERR_INVALIDPARAMS; }

    lpRndr->Device->EndScene();

    return lpRndr->Device->Present(NULL, NULL, NULL, NULL);
}

HRESULT SetRendererCullMode(LPRENDERER lpRndr, DWORD dwMode)
{
    if (lpRndr == NULL) { return RERR_INVALIDPARAMS; }

    return lpRndr->Device->SetRenderState(D3DRS_CULLMODE, dwMode);
}

HRESULT SetRendererMatrixes(LPRENDERER lpRndr, D3DXMATRIXA16* mWorld, D3DXMATRIXA16* mView, D3DXMATRIXA16* mProjection)
{
    if (lpRndr == NULL || mWorld == NULL || mView == NULL || mProjection == NULL) { return RERR_INVALIDPARAMS; }

    lpRndr->Device->SetTransform(D3DTS_WORLD, mWorld);
    lpRndr->Device->SetTransform(D3DTS_VIEW, mView);
    lpRndr->Device->SetTransform(D3DTS_PROJECTION, mProjection);

    return R_OK;
}

HRESULT RenderRendererMesh(LPRENDERER lpRndr, LPRENDERERMESH lpMesh)
{
    if (lpRndr == NULL || lpMesh == NULL) { return RERR_INVALIDPARAMS; }
    if (lpMesh->Vertex == INVALID_RENDERER_HANDLE && lpMesh->Texture == INVALID_RENDERER_HANDLE) { return RERR_INVALIDPARAMS; }

    LPDIRECT3DTEXTURE9 tex = lpMesh->Texture == INVALID_RENDERER_HANDLE
        ? NULL : lpRndr->Textures[lpMesh->Texture];

    lpRndr->Device->SetTexture(0, tex);

    if (tex == NULL)
    {
        lpRndr->Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
        lpRndr->Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    }
    else
    {
        lpRndr->Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        lpRndr->Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        lpRndr->Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        lpRndr->Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        lpRndr->Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
        lpRndr->Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
    }
    
    if (lpMesh->Vertex != INVALID_RENDERER_HANDLE)
    {
        lpRndr->Device->SetStreamSource(0, lpRndr->VertexBuffers[lpMesh->Vertex], 0, lpMesh->VertexSize);
        lpRndr->Device->SetFVF(lpMesh->VertexFormat);
        lpRndr->Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, lpMesh->VertexCount / 3);
    }

    return R_OK;
}

HRESULT CreateRendererTexture(LPRENDERER lpRndr, DWORD dwWidth, DWORD dwHeight, DWORD dwLevels, D3DFORMAT ddFormat, LPDWORD lpdwTexture)
{
    if (lpRndr == NULL || dwWidth == 0 || dwHeight == 0 || lpdwTexture == NULL) { return RERR_INVALIDPARAMS; }

    HRESULT result = D3D_OK;
    LPDIRECT3DTEXTURE9 texture = NULL;
    if (FAILED(result = lpRndr->Device->CreateTexture(dwWidth, dwHeight,
        dwLevels, 0, ddFormat, D3DPOOL_MANAGED, &texture, NULL))) { return result; }

    // NOTE: D3DPOOL_SYSTEMMEM will result in white texture.

    for (DWORD i = 0; i < MAX_RENDERER_TEXTURE_COUNT; i++)
    {
        if (lpRndr->Textures[i] == NULL)
        {
            lpRndr->Textures[i] = texture;
            *lpdwTexture = i;
            return R_OK;
        }
    }

    texture->Release();

    return RERR_GENERIC;
}

HRESULT UpdateRendererTexture(LPRENDERER lpRndr, DWORD dwTexture, LPRECT lpRect, LPBYTE lpContent, DWORD dwStride)
{
    if (lpRndr == NULL || MAX_RENDERER_TEXTURE_COUNT <= dwTexture
        || lpRect == NULL || lpContent == NULL || dwStride == 0) { return RERR_INVALIDPARAMS; }

    D3DLOCKED_RECT lock;
    if (FAILED(lpRndr->Textures[dwTexture]->LockRect(0, &lock, lpRect, 0))) { return RERR_GENERIC; }

    LPBYTE lpSrc = lpContent + (lpRect->bottom * dwStride);
    LPBYTE lpDst = (LPBYTE)lock.pBits;
    for (LONG i = 0; i < lpRect->bottom; i++)
    {
        CopyMemory(lpDst, lpSrc, dwStride);

        lpSrc = lpSrc - dwStride;
        lpDst = lpDst + lock.Pitch;
    }

    return lpRndr->Textures[dwTexture]->UnlockRect(0);
}

HRESULT ReleaseRendererTexture(LPRENDERER lpRndr, DWORD dwTexture)
{
    if (lpRndr == NULL || MAX_RENDERER_TEXTURE_COUNT <= dwTexture) { return RERR_INVALIDPARAMS; }

    while (lpRndr->Textures[dwTexture]->Release() != 0) {}

    lpRndr->Textures[dwTexture] = NULL;

    return R_OK;
}

HRESULT CreateRendererVertexBuffer(LPRENDERER lpRndr, DWORD dwSize, DWORD dwCount, DWORD dwFormat, LPDWORD lpdwVB)
{
    if (lpRndr == NULL || dwSize == 0 || dwCount == 0 || dwFormat == D3DFMT_UNKNOWN || lpdwVB == NULL) { return RERR_INVALIDPARAMS; }

    HRESULT result = D3D_OK;
    LPDIRECT3DVERTEXBUFFER9 vb = NULL;
    if (FAILED(result = lpRndr->Device->CreateVertexBuffer(dwSize * dwCount,
        0, dwFormat, D3DPOOL_DEFAULT, &vb, NULL))) { return result; }

    for (DWORD i = 0; i < MAX_RENDERER_VERTEXBUFFER_COUNT; i++)
    {
        if (lpRndr->VertexBuffers[i] == NULL)
        {
            lpRndr->VertexBuffers[i] = vb;
            *lpdwVB = i;
            return R_OK;
        }
    }

    vb->Release();

    return RERR_GENERIC;
}

HRESULT UpdateRendererVertexBuffer(LPRENDERER lpRndr, DWORD dwVB, DWORD dwSize, LPBYTE lpContent)
{
    if (lpRndr == NULL || MAX_RENDERER_VERTEXBUFFER_COUNT <= dwVB || lpContent == NULL) { return RERR_INVALIDPARAMS; }

    LPVOID lpLock = NULL;
    if (FAILED(lpRndr->VertexBuffers[dwVB]->Lock(0, dwSize, &lpLock, 0))) { return RERR_GENERIC; }

    CopyMemory(lpLock, lpContent, dwSize);

    return lpRndr->VertexBuffers[dwVB]->Unlock();
}

HRESULT ReleaseRendererVertexBuffer(LPRENDERER lpRndr, DWORD dwVB)
{
    if (lpRndr == NULL || MAX_RENDERER_VERTEXBUFFER_COUNT <= dwVB) { return RERR_INVALIDPARAMS; }

    while (lpRndr->VertexBuffers[dwVB]->Release() != 0) {}

    lpRndr->VertexBuffers[dwVB] = NULL;

    return R_OK;
}
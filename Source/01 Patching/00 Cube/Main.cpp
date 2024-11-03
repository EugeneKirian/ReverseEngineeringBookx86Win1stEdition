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

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

#define WINDOW_CLASS_NAME   "Cube"
#define WINDOW_TITLE        "Cube!"

#define CUBE_TEXTURE_NAME   "cube.bmp"

#define WINDOW_WIDTH        640
#define WINDOW_HEIGHT       480

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
LPDIRECT3DTEXTURE9      g_pTexture = NULL;

BOOL g_paused       = FALSE;
INT g_window_width  = WINDOW_WIDTH;
INT g_window_height = WINDOW_HEIGHT;

struct CUSTOMVERTEX
{
    D3DXVECTOR3 position;
    D3DCOLOR    color;
    FLOAT       tu, tv;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define CUBE_TRIANGLE_COUNT 12
#define CUBE_VERTEX_COUNT   (CUBE_TRIANGLE_COUNT * 3)

HRESULT InitD3D(HWND hWnd)
{
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        return E_FAIL;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

    return S_OK;
}

HRESULT InitGeometry()
{
    if (FAILED(D3DXCreateTextureFromFileA(g_pd3dDevice, CUBE_TEXTURE_NAME, &g_pTexture)))
    {
        if (FAILED(D3DXCreateTextureFromFileA(g_pd3dDevice, CUBE_TEXTURE_NAME, &g_pTexture)))
        {
            MessageBoxA(NULL, "Could not find texture!", WINDOW_TITLE, MB_OK | MB_ICONERROR);

            return E_FAIL;
        }
    }

    if (FAILED(g_pd3dDevice->CreateVertexBuffer(CUBE_VERTEX_COUNT * sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL)))
    {
        return E_FAIL;
    }

    CUSTOMVERTEX* pVertices;

    if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
    {
        return E_FAIL;
    }

    // Top
    pVertices[0].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
    pVertices[0].color = 0xFFFFFFFF;
    pVertices[0].tu = 1.0f;
    pVertices[0].tv = 1.0f;

    pVertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
    pVertices[1].color = 0xFFFFFFFF;
    pVertices[1].tu = 0.0f;
    pVertices[1].tv = 1.0f;

    pVertices[2].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
    pVertices[2].color = 0xFFFFFFFF;
    pVertices[2].tu = 1.0f;
    pVertices[2].tv = 0.0f;

    pVertices[3].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
    pVertices[3].color = 0xFFFFFFFF;
    pVertices[3].tu = 1.0f;
    pVertices[3].tv = 0.0f;

    pVertices[4].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
    pVertices[4].color = 0xFFFFFFFF;
    pVertices[4].tu = 0.0f;
    pVertices[4].tv = 1.0f;

    pVertices[5].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
    pVertices[5].color = 0xFFFFFFFF;
    pVertices[5].tu = 0.0f;
    pVertices[5].tv = 0.0f;

    // Bottom
    pVertices[6].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
    pVertices[6].color = 0xFFFFFFFF;
    pVertices[6].tu = 1.0f;
    pVertices[6].tv = 1.0f;

    pVertices[7].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
    pVertices[7].color = 0xFFFFFFFF;
    pVertices[7].tu = 0.0f;
    pVertices[7].tv = 1.0f;

    pVertices[8].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
    pVertices[8].color = 0xFFFFFFFF;
    pVertices[8].tu = 1.0f;
    pVertices[8].tv = 0.0f;

    pVertices[9].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
    pVertices[9].color = 0xFFFFFFFF;
    pVertices[9].tu = 1.0f;
    pVertices[9].tv = 0.0f;

    pVertices[10].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
    pVertices[10].color = 0xFFFFFFFF;
    pVertices[10].tu = 0.0f;
    pVertices[10].tv = 1.0f;

    pVertices[11].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
    pVertices[11].color = 0xFFFFFFFF;
    pVertices[11].tu = 0.0f;
    pVertices[11].tv = 0.0f;

    // Front
    pVertices[12].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
    pVertices[12].color = 0xFFFFFFFF;
    pVertices[12].tu = 1.0f;
    pVertices[12].tv = 1.0f;

    pVertices[13].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
    pVertices[13].color = 0xFFFFFFFF;
    pVertices[13].tu = 0.0f;
    pVertices[13].tv = 1.0f;

    pVertices[14].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
    pVertices[14].color = 0xFFFFFFFF;
    pVertices[14].tu = 1.0f;
    pVertices[14].tv = 0.0f;

    pVertices[15].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
    pVertices[15].color = 0xFFFFFFFF;
    pVertices[15].tu = 1.0f;
    pVertices[15].tv = 0.0f;

    pVertices[16].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
    pVertices[16].color = 0xFFFFFFFF;
    pVertices[16].tu = 0.0f;
    pVertices[16].tv = 1.0f;

    pVertices[17].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
    pVertices[17].color = 0xFFFFFFFF;
    pVertices[17].tu = 0.0f;
    pVertices[17].tv = 0.0f;

    // Back
    pVertices[18].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
    pVertices[18].color = 0xFFFFFFFF;
    pVertices[18].tu = 0.0f;
    pVertices[18].tv = 1.0f;

    pVertices[19].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
    pVertices[19].color = 0xFFFFFFFF;
    pVertices[19].tu = 1.0f;
    pVertices[19].tv = 1.0f;

    pVertices[20].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
    pVertices[20].color = 0xFFFFFFFF;
    pVertices[20].tu = 0.0f;
    pVertices[20].tv = 0.0f;

    pVertices[21].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
    pVertices[21].color = 0xFFFFFFFF;
    pVertices[21].tu = 0.0f;
    pVertices[21].tv = 0.0f;

    pVertices[22].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
    pVertices[22].color = 0xFFFFFFFF;
    pVertices[22].tu = 1.0f;
    pVertices[22].tv = 1.0f;

    pVertices[23].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
    pVertices[23].color = 0xFFFFFFFF;
    pVertices[23].tu = 1.0f;
    pVertices[23].tv = 0.0f;

    // Left
    pVertices[24].position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
    pVertices[24].color = 0xFFFFFFFF;
    pVertices[24].tu = 0.0f;
    pVertices[24].tv = 1.0f;

    pVertices[25].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
    pVertices[25].color = 0xFFFFFFFF;
    pVertices[25].tu = 1.0f;
    pVertices[25].tv = 1.0f;

    pVertices[26].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
    pVertices[26].color = 0xFFFFFFFF;
    pVertices[26].tu = 0.0f;
    pVertices[26].tv = 0.0f;

    pVertices[27].position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
    pVertices[27].color = 0xFFFFFFFF;
    pVertices[27].tu = 0.0f;
    pVertices[27].tv = 0.0f;

    pVertices[28].position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
    pVertices[28].color = 0xFFFFFFFF;
    pVertices[28].tu = 1.0f;
    pVertices[28].tv = 1.0f;

    pVertices[29].position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
    pVertices[29].color = 0xFFFFFFFF;
    pVertices[29].tu = 1.0f;
    pVertices[29].tv = 0.0f;

    // Right
    pVertices[30].position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
    pVertices[30].color = 0xFFFFFFFF;
    pVertices[30].tu = 1.0f;
    pVertices[30].tv = 1.0f;

    pVertices[31].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
    pVertices[31].color = 0xFFFFFFFF;
    pVertices[31].tu = 0.0f;
    pVertices[31].tv = 1.0f;

    pVertices[32].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
    pVertices[32].color = 0xFFFFFFFF;
    pVertices[32].tu = 1.0f;
    pVertices[32].tv = 0.0f;

    pVertices[33].position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
    pVertices[33].color = 0xFFFFFFFF;
    pVertices[33].tu = 1.0f;
    pVertices[33].tv = 0.0f;

    pVertices[34].position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
    pVertices[34].color = 0xFFFFFFFF;
    pVertices[34].tu = 0.0f;
    pVertices[34].tv = 1.0f;

    pVertices[35].position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
    pVertices[35].color = 0xFFFFFFFF;
    pVertices[35].tu = 0.0f;
    pVertices[35].tv = 0.0f;

    return g_pVB->Unlock();
}

VOID Cleanup()
{
    if (g_pTexture != NULL)
    {
        g_pTexture->Release();
    }

    if (g_pVB != NULL)
    {
        g_pVB->Release();
    }

    if (g_pd3dDevice != NULL)
    {
        g_pd3dDevice->Release();
    }

    if (g_pD3D != NULL)
    {
        g_pD3D->Release();
    }
}

VOID SetupMatrices()
{
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity(&matWorld);

    CONST FLOAT value = timeGetTime() / 1000.0f;

    D3DXMatrixRotationYawPitchRoll(&matWorld, value, value, value);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

    D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIXA16 matProj;

    CONST FLOAT aspect_ratio = (FLOAT)g_window_width / (FLOAT)g_window_height;

    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, aspect_ratio, 1.0f, 100.0f);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID Render()
{
    if (g_window_width == 0 || g_window_height == 0) { return; }

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB(192, 192, 192), 1.0f, 0);

    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        if (!g_paused) { SetupMatrices(); }

        g_pd3dDevice->SetTexture(0, g_pTexture);
        g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

        g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
        g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, CUBE_TRIANGLE_COUNT);

        g_pd3dDevice->EndScene();
    }

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);

            g_window_width = rect.right - rect.left;
            g_window_height = rect.bottom - rect.top;

            break;
        }
    case WM_KEYUP:
    case WM_SYSKEYUP:
        {
            if (wParam == VK_SPACE)
            {
                g_paused = !g_paused;
            }

            break;
        }
    case WM_DESTROY:
        {
            Cleanup();

            PostQuitMessage(0);

            return 0;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    WNDCLASSEXA wc =
    {
        sizeof(WNDCLASSEXA),
        CS_CLASSDC,
        MsgProc,
        0,
        0,
        hInst,
        NULL,
        NULL,
        NULL,
        NULL,
        WINDOW_CLASS_NAME,
        NULL
    };

    RegisterClassExA(&wc);

    HWND hWnd = CreateWindowA(
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // WS_OVERLAPPEDWINDOW
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        NULL,
        NULL,
        hInst,
        NULL);

    if (SUCCEEDED(InitD3D(hWnd)))
    {
        if (SUCCEEDED(InitGeometry()))
        {
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            {
                RECT rect;
                GetClientRect(hWnd, &rect);

                g_window_width = rect.right - rect.left;
                g_window_height = rect.bottom - rect.top;
            }

            MSG msg;
            ZeroMemory(&msg, sizeof(msg));

            while (msg.message != WM_QUIT)
            {
                if (!PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    Render();

                    continue;
                }

                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
    }

    UnregisterClassA(WINDOW_CLASS_NAME, hInst);

    return 0;
}
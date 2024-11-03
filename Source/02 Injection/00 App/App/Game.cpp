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

#include "Game.h"
#include "Scene.h"

#include <Img.h>

typedef struct SimpleVertex
{
    FLOAT       X, Y, Z;
    D3DCOLOR    Color;
    FLOAT       U, V;
} SIMPLEVERTEX, * LPSIMPLEVERTEX;

#define MAX_SQUARE_VERTEX_COUNT 6

typedef struct Panel
{
    RENDERERMESH    Mesh;
    SIMPLEVERTEX    Vertexes[MAX_SQUARE_VERTEX_COUNT];
    D3DXMATRIXA16   World;
    D3DXMATRIXA16   View;
    D3DXMATRIXA16   Projection;
} PANEL, *lpPanel;

struct Game
{
    BOOL                IsPause;
    BOOL                IsWin;

    LPASSETS            Assets;
    LPRENDERER          Renderer;

    LPINPUT             Input;

    LPSCENE             Scene;

    PANEL               Back;
    PANEL               Pause;
    PANEL               Win;
    
    struct
    {
        BYTE            Current[256];
        BYTE            Last[256];
    } Keyboard;
};

#define FVFSIMPLEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

STATIC HRESULT AcquireTexture(LPASSETS lpAssets, LPRENDERER lpRenderer, LPCSTR lpszName, LPDWORD lpdwTexture)
{
    ASSETCONTENT asset;
    if (FAILED(GetAssetsContent(lpAssets, lpszName, &asset))) { return GERR_GENERIC; }

    LPIMG image = (LPIMG)malloc(asset.Size);

    if (image == NULL) { return GERR_OUTOFMEMORY; }

    ZeroMemory(image, asset.Size);

    if (FAILED(GetAssetsContentContent(lpAssets, &asset, image)))
    {
        free(image);
        return GERR_GENERIC;
    }

    CONST D3DFORMAT ddFormat = image->Info.Format == IMG_FORMAT_RGB565
        ? D3DFMT_R5G6B5 : D3DFMT_UNKNOWN;

    if (ddFormat == D3DFMT_UNKNOWN)
    {
        free(image);
        return GERR_GENERIC;
    }

    if (FAILED(CreateRendererTexture(lpRenderer, image->Info.Width,
        image->Info.Height, 0, ddFormat, lpdwTexture)))
    {
        free(image);
        return GERR_GENERIC;
    }

    RECT rect = { 0, 0, image->Info.Width, image->Info.Height };
    UpdateRendererTexture(lpRenderer, *lpdwTexture, &rect,
        ((LPBYTE)image + sizeof(IMGHEAD) + sizeof(IMGINFO)), image->Info.Stride);

    free(image);

    return G_OK;
}

STATIC HRESULT CreateBackground(LPASSETS lpAssets, LPRENDERER lpRenderer, lpPanel lpBack)
{
    {
        D3DXMatrixIdentity(&lpBack->World);

        D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);
        D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
        D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&lpBack->View, &vEyePt, &vLookatPt, &vUpVec);

        D3DXMatrixOrthoLH(&lpBack->Projection,
            (FLOAT)lpRenderer->Window.Width / (FLOAT)lpRenderer->Window.Height,
            1.0f, 1.0f, 100.0f);
    }

    if (FAILED(AcquireTexture(lpAssets, lpRenderer, "main.img", &lpBack->Mesh.Texture)))
    {
        lpBack->Mesh.Texture = INVALID_RENDERER_HANDLE;
    }
    
    lpBack->Mesh.VertexCount = MAX_SQUARE_VERTEX_COUNT;
    lpBack->Mesh.VertexFormat = FVFSIMPLEVERTEX;
    lpBack->Mesh.VertexSize = sizeof(SIMPLEVERTEX);

    // Top-Left
    lpBack->Vertexes[0].X = 1.0f;
    lpBack->Vertexes[0].Y = 1.0f;
    lpBack->Vertexes[0].Z = 0.0f;
    lpBack->Vertexes[0].Color = D3DCOLOR_XRGB(0, 100, 200);
    lpBack->Vertexes[0].U = 1.0f;
    lpBack->Vertexes[0].V = 1.0f;

    lpBack->Vertexes[1].X = -1.0f;
    lpBack->Vertexes[1].Y = 1.0f;
    lpBack->Vertexes[1].Z = 0.0f;
    lpBack->Vertexes[1].Color = D3DCOLOR_XRGB(0, 100, 200);
    lpBack->Vertexes[1].U = 0.0f;
    lpBack->Vertexes[1].V = 1.0f;

    lpBack->Vertexes[2].X = -1.0f;
    lpBack->Vertexes[2].Y = -1.0f;
    lpBack->Vertexes[2].Z = 0.0f;
    lpBack->Vertexes[2].Color = D3DCOLOR_XRGB(0, 100, 200);
    lpBack->Vertexes[2].U = 0.0f;
    lpBack->Vertexes[2].V = 0.0f;

    // Bottom-Right
    lpBack->Vertexes[3].X = 1.0f;
    lpBack->Vertexes[3].Y = 1.0f;
    lpBack->Vertexes[3].Z = 0.0f;
    lpBack->Vertexes[3].Color = D3DCOLOR_XRGB(0, 100, 200);
    lpBack->Vertexes[3].U = 1.0f;
    lpBack->Vertexes[3].V = 1.0f;

    lpBack->Vertexes[4].X = -1.0f;
    lpBack->Vertexes[4].Y = -1.0f;
    lpBack->Vertexes[4].Z = 0.0f;
    lpBack->Vertexes[4].Color = D3DCOLOR_XRGB(0, 100, 200);
    lpBack->Vertexes[4].U = 0.0f;
    lpBack->Vertexes[4].V = 0.0f;

    lpBack->Vertexes[5].X = 1.0f;
    lpBack->Vertexes[5].Y = -1.0f;
    lpBack->Vertexes[5].Z = 0.0f;
    lpBack->Vertexes[5].Color = D3DCOLOR_XRGB(0, 100, 200);
    lpBack->Vertexes[5].U = 1.0f;
    lpBack->Vertexes[5].V = 0.0f;
    
    if (FAILED(CreateRendererVertexBuffer(lpRenderer, lpBack->Mesh.VertexSize,
        lpBack->Mesh.VertexCount, lpBack->Mesh.VertexFormat, &lpBack->Mesh.Vertex)))
    {
        lpBack->Mesh.Vertex = INVALID_RENDERER_HANDLE;
    }

    if (lpBack->Mesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        UpdateRendererVertexBuffer(lpRenderer, lpBack->Mesh.Vertex,
            lpBack->Mesh.VertexSize * lpBack->Mesh.VertexCount, (LPBYTE)lpBack->Vertexes);
    }

    return G_OK;
}

STATIC HRESULT CreatePanel(LPASSETS lpAssets, LPRENDERER lpRenderer, LPCSTR lpszName, lpPanel lpPanel)
{
    {
        D3DXMatrixIdentity(&lpPanel->World);

        D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);
        D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
        D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
        D3DXMatrixLookAtLH(&lpPanel->View, &vEyePt, &vLookatPt, &vUpVec);

        D3DXMatrixOrthoLH(&lpPanel->Projection,
            (FLOAT)lpRenderer->Window.Width / (FLOAT)lpRenderer->Window.Height,
            1.0f, 1.0f, 100.0f);
    }

    if (FAILED(AcquireTexture(lpAssets, lpRenderer, lpszName, &lpPanel->Mesh.Texture)))
    {
        lpPanel->Mesh.Texture = INVALID_RENDERER_HANDLE;
    }
    
    lpPanel->Mesh.VertexCount = MAX_SQUARE_VERTEX_COUNT;
    lpPanel->Mesh.VertexFormat = FVFSIMPLEVERTEX;
    lpPanel->Mesh.VertexSize = sizeof(SIMPLEVERTEX);

    // Top-Left
    lpPanel->Vertexes[0].X = 1.0f;
    lpPanel->Vertexes[0].Y = 1.0f;
    lpPanel->Vertexes[0].Z = 0.0f;
    lpPanel->Vertexes[0].Color = D3DCOLOR_ARGB(20, 255, 255, 255);
    lpPanel->Vertexes[0].U = 1.0f;
    lpPanel->Vertexes[0].V = 1.0f;

    lpPanel->Vertexes[1].X = -1.0f;
    lpPanel->Vertexes[1].Y = 1.0f;
    lpPanel->Vertexes[1].Z = 0.0f;
    lpPanel->Vertexes[1].Color = D3DCOLOR_ARGB(20, 255, 255, 255);
    lpPanel->Vertexes[1].U = 0.0f;
    lpPanel->Vertexes[1].V = 1.0f;

    lpPanel->Vertexes[2].X = -1.0f;
    lpPanel->Vertexes[2].Y = -1.0f;
    lpPanel->Vertexes[2].Z = 0.0f;
    lpPanel->Vertexes[2].Color = D3DCOLOR_ARGB(20, 255, 255, 255);
    lpPanel->Vertexes[2].U = 0.0f;
    lpPanel->Vertexes[2].V = 0.0f;

    // Bottom-Right
    lpPanel->Vertexes[3].X = 1.0f;
    lpPanel->Vertexes[3].Y = 1.0f;
    lpPanel->Vertexes[3].Z = 0.0f;
    lpPanel->Vertexes[3].Color = D3DCOLOR_ARGB(20, 255, 255, 255);
    lpPanel->Vertexes[3].U = 1.0f;
    lpPanel->Vertexes[3].V = 1.0f;

    lpPanel->Vertexes[4].X = -1.0f;
    lpPanel->Vertexes[4].Y = -1.0f;
    lpPanel->Vertexes[4].Z = 0.0f;
    lpPanel->Vertexes[4].Color = D3DCOLOR_ARGB(20, 255, 255, 255);
    lpPanel->Vertexes[4].U = 0.0f;
    lpPanel->Vertexes[4].V = 0.0f;

    lpPanel->Vertexes[5].X = 1.0f;
    lpPanel->Vertexes[5].Y = -1.0f;
    lpPanel->Vertexes[5].Z = 0.0f;
    lpPanel->Vertexes[5].Color = D3DCOLOR_ARGB(20, 255, 255, 255);
    lpPanel->Vertexes[5].U = 1.0f;
    lpPanel->Vertexes[5].V = 0.0f;
    
    if (FAILED(CreateRendererVertexBuffer(lpRenderer, lpPanel->Mesh.VertexSize,
        lpPanel->Mesh.VertexCount, lpPanel->Mesh.VertexFormat, &lpPanel->Mesh.Vertex)))
    {
        lpPanel->Mesh.Vertex = INVALID_RENDERER_HANDLE;
    }

    if (lpPanel->Mesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        UpdateRendererVertexBuffer(lpRenderer, lpPanel->Mesh.Vertex,
            lpPanel->Mesh.VertexSize * lpPanel->Mesh.VertexCount, (LPBYTE)lpPanel->Vertexes);
    }

    return G_OK;
}

HRESULT CreateGame(LPASSETS lpAssets, LPRENDERER lpRenderer, LPINPUT lpInput, LPGAME* lplpGame)
{
    if (lpAssets == NULL || lpRenderer == NULL || lpInput == NULL || lplpGame == NULL) { return GERR_INVALIDPARAMS; }

    LPGAME instance = (LPGAME)malloc(sizeof(GAME));

    if (instance == NULL) { return GERR_OUTOFMEMORY; }

    ZeroMemory(instance, sizeof(GAME));

    instance->IsPause = FALSE;

    instance->Assets = lpAssets;
    instance->Renderer = lpRenderer;
    instance->Input = lpInput;

    CreateBackground(lpAssets, lpRenderer, &instance->Back);
    CreatePanel(lpAssets, lpRenderer, "pause.img", &instance->Pause);
    CreatePanel(lpAssets, lpRenderer, "win.img", &instance->Win);

    *lplpGame = instance;

    return S_OK;
}

HRESULT StartGame(LPGAME lpGame, LPCSTR lpszName)
{
    if (lpGame == NULL) { return GERR_INVALIDPARAMS; }

    HRESULT result = A_OK;
    ASSETCONTENT content;
    if (FAILED(result = GetAssetsContent(lpGame->Assets, lpszName, &content))) { return result; }

    return CreateScene(lpGame->Assets, lpGame->Input, lpGame->Renderer, &content, &lpGame->Scene);
}

HRESULT UpdateGame(LPGAME lpGame, CONST FLOAT fTime)
{
    if (lpGame == NULL) { return GERR_INVALIDPARAMS; }

    lpGame->Input->Update();

    HRESULT result = R_OK;
    DWORD dwColor = D3DCOLOR_XRGB(0, 0, 0);

    if (lpGame->Scene != NULL) { GetSceneColor(lpGame->Scene, &dwColor); }

    if (FAILED(result = BeginRendererScene(lpGame->Renderer, dwColor))) { return result; }

    if (!lpGame->IsWin)
    {
        if (lpGame->Input->IsPressing(VK_ESCAPE))
        {
            lpGame->IsPause = !lpGame->IsPause;
        }
    }

    if (lpGame->IsPause || lpGame->IsWin)
    {
        SetRendererCullMode(lpGame->Renderer, D3DCULL_NONE);
        SetRendererMatrixes(lpGame->Renderer,
            &lpGame->Back.World, &lpGame->Back.View, &lpGame->Back.Projection);
        RenderRendererMesh(lpGame->Renderer, &lpGame->Back.Mesh);
        
        RenderRendererMesh(lpGame->Renderer,
            lpGame->IsWin ? &lpGame->Win.Mesh : &lpGame->Pause.Mesh);
    }
    else if (lpGame->Scene != NULL)
    {
        HRESULT result = UpdateScene(lpGame->Scene, fTime);

        if (result == S_WIN)
        {
            CHAR next[MAX_SCENE_NAME_LENGTH];
            ZeroMemory(next, MAX_SCENE_NAME_LENGTH);
            GetSceneNext(lpGame->Scene, next, MAX_SCENE_NAME_LENGTH);

            if (strlen(next) == 0)
            {
                lpGame->IsWin = TRUE;
                ReleaseScene(lpGame->Scene);
                lpGame->Scene = NULL;
            }
            else
            {
                ReleaseScene(lpGame->Scene);
                lpGame->Scene = NULL;

                if (FAILED(StartGame(lpGame, next))) { lpGame->IsWin = TRUE; }
            }
        }
    }

    return EndRendererScene(lpGame->Renderer);
}

HRESULT ReleaseGame(LPGAME lpGame)
{
    if (lpGame == NULL) { return GERR_INVALIDPARAMS; }

    if (lpGame->Pause.Mesh.Texture != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererTexture(lpGame->Renderer, lpGame->Pause.Mesh.Texture);
    }

    if (lpGame->Pause.Mesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererVertexBuffer(lpGame->Renderer, lpGame->Pause.Mesh.Vertex);
    }

    if (lpGame->Win.Mesh.Texture != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererTexture(lpGame->Renderer, lpGame->Win.Mesh.Texture);
    }

    if (lpGame->Win.Mesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererVertexBuffer(lpGame->Renderer, lpGame->Win.Mesh.Vertex);
    }

    if (lpGame->Back.Mesh.Texture != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererTexture(lpGame->Renderer, lpGame->Back.Mesh.Texture);
    }

    if (lpGame->Back.Mesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererVertexBuffer(lpGame->Renderer, lpGame->Back.Mesh.Vertex);
    }

    if (lpGame->Scene != NULL) { ReleaseScene(lpGame->Scene); }

    free(lpGame);

    return G_OK;
}
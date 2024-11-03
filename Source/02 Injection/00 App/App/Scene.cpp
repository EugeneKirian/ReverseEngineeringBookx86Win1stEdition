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

#include "Actor.h"
#include "Bullet.h"
#include "Camera.h"
#include "Scene.h"
#include "Settings.h"
#include "Renderer.h"

#include <stdio.h>

#define MAX_SCENE_BULLET_COUNT  128

#define MAX_SCENE_BUFFER_LENGTH 128

#define DEFAULT_BULLET_TIMER_VALUE  0.5f

struct Scene
{
    LPASSETS        Assets;
    LPINPUT         Input;
    LPCAMERA        Camera;
    LPRENDERER      Renderer;

    CHAR            Name[MAX_SCENE_NAME_LENGTH];
    CHAR            Next[MAX_PATH];
    DWORD           Color;

    struct
    {
        DWORD       Count;
        LPACTOR*    Actors;
    } Actors;

    FLOAT           BulletTimer;
    LPBULLET        Bullets[MAX_SCENE_BULLET_COUNT];
};

STATIC HRESULT CreateScene(LPSCENE lpScene, LPASSETS lpAssets, LPINPUT lpInput, LPRENDERER lpRenderer, LPCSTR lpszContent)
{
    AcquireSettingsValue(lpszContent, "name", lpScene->Name);
    AcquireSettingsValue(lpszContent, "nextscene", lpScene->Next);

    {
        CHAR szValue[MAX_SCENE_BUFFER_LENGTH];
        AcquireSettingsValue(lpszContent, "color", szValue);

        AcquireSettingsXRGB(szValue, &lpScene->Color);
    }

    // Camera
    {
        CHAR szValue[MAX_SCENE_BUFFER_LENGTH];
        AcquireSettingsValue(lpszContent, "cameraposition", szValue);

        V3 vPosition;
        ZeroMemory(&vPosition, sizeof(V3));
        AcquireSettingsV3(szValue, &vPosition);

        AcquireSettingsValue(lpszContent, "cameratarget", szValue);

        V3 vTarget;
        ZeroMemory(&vTarget, sizeof(V3));
        AcquireSettingsV3(szValue, &vTarget);

        V3 vUp = { 0.0f, 1.0f, 0.0f };
        V3 vForward = { 0.0f, 0.0f, 1.0f };
        V3 vRight = { 1.0f, 0.0f, 0.0f };
        
        lpScene->Camera = new Camera(lpInput, lpRenderer, vPosition, vTarget, vUp, vForward, vRight);
        if (lpScene->Camera == NULL) { return SERR_OUTOFMEMORY; }
    }

    // Actors
    {
        CHAR szValue[MAX_SCENE_BUFFER_LENGTH];
        AcquireSettingsValue(lpszContent, "actors", szValue);

        if (sscanf_s(szValue, "%u", &lpScene->Actors.Count) == 0) { return S_OK; }

        lpScene->Actors.Actors = (LPACTOR*)malloc(lpScene->Actors.Count * sizeof(LPACTOR));

        if (lpScene->Actors.Actors == NULL) { goto CLEANUP; }

        ZeroMemory(lpScene->Actors.Actors, lpScene->Actors.Count * sizeof(LPACTOR));

        for (DWORD i = 0; i < lpScene->Actors.Count; i++)
        {
            CreateActor(lpAssets, lpRenderer, lpszContent, i, &lpScene->Actors.Actors[i]);
        }
    }

    // Bullets
    for (DWORD i = 0; i < MAX_SCENE_BULLET_COUNT; i++)
    {
        if (SUCCEEDED(CreateBullet(lpAssets, lpRenderer, &lpScene->Bullets[i])))
        {
            lpScene->Bullets[i]->SetIsActive(FALSE);
        }
    }

    return S_OK;

CLEANUP:
    if (lpScene->Camera != NULL) { delete lpScene->Camera; }

    if (lpScene->Actors.Actors != NULL)
    {
        for (DWORD i = 0; i < lpScene->Actors.Count; i++)
        {
            if (lpScene->Actors.Actors[i] != NULL)
            {
                ReleaseActor(lpScene->Actors.Actors[i]);
                lpScene->Actors.Actors[i] = NULL;
            }
        }

        free(lpScene->Actors.Actors);
    }

    return SERR_OUTOFMEMORY;
}

HRESULT CreateScene(LPASSETS lpAssets, LPINPUT lpInput, LPRENDERER lpRenderer, LPASSETCONTENT lpAssetContent, LPSCENE* lplpScene)
{
    if (lpAssets == NULL || lpRenderer == NULL || lpInput == NULL
        || lpAssetContent == NULL || lplpScene == NULL) { return SERR_INVALIDPARAMS; }

    LPSTR lpszContent = (LPSTR)malloc(lpAssetContent->Size + 1);

    if (lpszContent == NULL) { return SERR_OUTOFMEMORY; }

    ZeroMemory(lpszContent, lpAssetContent->Size + 1);

    if (FAILED(GetAssetsContentContent(lpAssets, lpAssetContent, lpszContent)))
    {
        free(lpszContent);
        return SERR_GENERIC;
    }

    LPSCENE instance = (LPSCENE)malloc(sizeof(SCENE));

    if (instance == NULL)
    {
        free(lpszContent);
        return SERR_OUTOFMEMORY;
    }

    ZeroMemory(instance, sizeof(SCENE));

    instance->Assets = lpAssets;
    instance->Input = lpInput;
    instance->Renderer = lpRenderer;

    HRESULT result = S_OK;
    if (SUCCEEDED(result = CreateScene(instance, lpAssets, lpInput, lpRenderer, lpszContent)))
    {
        *lplpScene = instance;
    }

    free(lpszContent);

    return result;
}

STATIC HRESULT UpdateSceneActors(LPSCENE lpScene)
{
    if (lpScene->Actors.Count == 0) { return S_OK; }

    for (DWORD i = 0; i < MAX_SCENE_BULLET_COUNT; i++)
    {
        BOOL active = FALSE;
        lpScene->Bullets[i]->GetIsActive(&active);

        if (!active) { continue; }

        BOUNDINGBOX bullet;
        lpScene->Bullets[i]->GetBoundingBox(&bullet);

        for (DWORD k = 0; k < lpScene->Actors.Count; k++)
        {
            active = FALSE;
            lpScene->Actors.Actors[k]->GetIsActive(&active);

            if (!active) { continue; }

            BOUNDINGBOX actor;
            lpScene->Actors.Actors[k]->GetBoundingBox(&actor);

            if (Intersect(&bullet, &actor))
            {
                INT bhp = 0;
                lpScene->Bullets[i]->GetHealth(&bhp);
                lpScene->Bullets[i]->SetIsActive(FALSE);

                INT ahp = 0;
                lpScene->Actors.Actors[k]->GetHealth(&ahp);

                CONST INT health = ahp - bhp <= 0 ? 0 : ahp - bhp;

                lpScene->Actors.Actors[k]->SetHealth(health);

                if (health <= 0) { lpScene->Actors.Actors[k]->SetIsActive(FALSE); }
            }
        }
    }

    return S_OK;
}

HRESULT UpdateScene(LPSCENE lpScene, CONST FLOAT fTime)
{
    if (lpScene == NULL) { return SERR_INVALIDPARAMS; }

    UpdateSceneActors(lpScene);

    lpScene->Camera->Update(fTime);

    lpScene->BulletTimer -= fTime;

    if (lpScene->Input->IsMouse(INPUT_LEFT_MOUSE) && lpScene->BulletTimer <= 0.0f)
    {
        for (DWORD i = 0; i < MAX_SCENE_BULLET_COUNT; i++)
        {
            BOOL bActive = FALSE;
            lpScene->Bullets[i]->GetIsActive(&bActive);

            if (!bActive)
            {
                V3 position, direction;

                lpScene->Camera->GetPosition(&position);
                lpScene->Camera->GetDirection(&direction);

                D3DXVec3Scale((LPD3DXVECTOR3)&direction, (LPD3DXVECTOR3)&direction, 2.0f);
                D3DXVec3Add((LPD3DXVECTOR3)&position, (LPD3DXVECTOR3)&position, (LPD3DXVECTOR3)&direction);

                lpScene->Bullets[i]->SetPosition(position);
                lpScene->Bullets[i]->SetDirection(direction);
                lpScene->Bullets[i]->SetIsActive(TRUE);

                lpScene->BulletTimer = DEFAULT_BULLET_TIMER_VALUE;

                break;
            }
        }
    }

    for (DWORD i = 0; i < lpScene->Actors.Count; i++)
    {
        if (lpScene->Actors.Actors[i] != NULL)
        {
            BOOL bIsActive = FALSE;
            lpScene->Actors.Actors[i]->GetIsActive(&bIsActive);

            if (bIsActive) { lpScene->Actors.Actors[i]->Update(fTime); }
        }
    }

    for (DWORD i = 0; i < MAX_SCENE_BULLET_COUNT; i++)
    {
        if (lpScene->Bullets[i] != NULL)
        {
            BOOL bIsActive = FALSE;
            lpScene->Bullets[i]->GetIsActive(&bIsActive);

            if (bIsActive) { lpScene->Bullets[i]->Update(fTime); }
        }
    }

    BOOL bAlive = FALSE;
    for (DWORD i = 0; i < lpScene->Actors.Count; i++)
    {
        BOOL bIsActive = FALSE;
        lpScene->Actors.Actors[i]->GetIsActive(&bIsActive);

        if (bIsActive) { bAlive = TRUE; break; }
    }

    return bAlive ? S_OK : S_WIN;
}

HRESULT ReleaseScene(LPSCENE lpScene)
{
    if (lpScene == NULL) { return SERR_INVALIDPARAMS; }

    if (lpScene->Bullets != NULL)
    {
        for (DWORD i = 0; i < MAX_SCENE_BULLET_COUNT; i++)
        {
            ReleaseBullet(lpScene->Bullets[i]);
            lpScene->Bullets[i] = NULL;
        }
    }

    if (lpScene->Actors.Actors != NULL)
    {
        for (DWORD i = 0; i < lpScene->Actors.Count; i++)
        {
            ReleaseActor(lpScene->Actors.Actors[i]);
            lpScene->Actors.Actors[i] = NULL;
        }

        free(lpScene->Actors.Actors);
    }

    if (lpScene->Camera != NULL)
    {
        delete lpScene->Camera;
        lpScene->Camera = NULL;
    }

    return S_OK;
}

HRESULT GetSceneColor(LPSCENE lpScene, LPDWORD lpdwColor)
{
    if (lpScene == NULL || lpdwColor == NULL) { return SERR_INVALIDPARAMS; }

    *lpdwColor = lpScene->Color;

    return S_OK;
}

HRESULT GetSceneNext(LPSCENE lpScene, LPSTR lpszNext, DWORD dwSize)
{
    if (lpScene == NULL || lpszNext == NULL || dwSize == 0) { return SERR_INVALIDPARAMS; }

    strcpy_s(lpszNext, dwSize, lpScene->Next);

    return S_OK;
}
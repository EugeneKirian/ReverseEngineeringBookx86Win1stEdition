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
#include "Settings.h"

#include <Img.h>

#include <stdio.h>

#define DEFAULT_ACTOR_HIT_POINT_VALUE   100

#define MAX_ACTOR_BUFFER_LENGTH         128

Actor::Actor(LPRENDERER lpRenderer)
{
    this->m_lpRenderer = lpRenderer;

    ZeroMemory(this->m_szName, MAX_ACTOR_NAME_LENGTH);

    this->m_bIsActive = TRUE;
    this->m_atType = ACTORTYPE_NONE;

    this->m_nHealth = this->m_nMaxHealth = DEFAULT_ACTOR_HIT_POINT_VALUE;

    this->m_fAngle = (FLOAT)rand() / 360.0f;
    this->m_fMultiplier = (FLOAT)((rand() % 10) - 5);
    ZeroMemory(&this->m_vPosition, sizeof(V3));
    ZeroMemory(&this->m_vScale, sizeof(V3));

    this->m_Vertexes.Current = NULL;
    this->m_Vertexes.Original = NULL;

    ZeroMemory(&this->m_bBox, sizeof(BOUNDINGBOX));
    ZeroMemory(&this->m_rMesh, sizeof(RENDERERMESH));

    this->m_rMesh.Vertex = INVALID_RENDERER_HANDLE;
    this->m_rMesh.Texture = INVALID_RENDERER_HANDLE;
}

Actor::~Actor()
{
    if (this->m_rMesh.Texture != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererTexture(this->m_lpRenderer, this->m_rMesh.Texture);
        this->m_rMesh.Texture = INVALID_RENDERER_HANDLE;
    }

    if (this->m_rMesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererVertexBuffer(this->m_lpRenderer, this->m_rMesh.Vertex);
        this->m_rMesh.Vertex = INVALID_RENDERER_HANDLE;
    }

    if (this->m_Vertexes.Current != NULL) { free(this->m_Vertexes.Current); }
    if (this->m_Vertexes.Original != NULL) { free(this->m_Vertexes.Original); }
}

VOID Actor::GetName(LPSTR lpszName, DWORD dwLength)
{
    strcpy_s(lpszName, dwLength, this->m_szName);
}

VOID Actor::SetName(LPCSTR lpszName)
{
    strcpy_s(this->m_szName, MAX_ACTOR_NAME_LENGTH, lpszName);
}

VOID Actor::GetBoundingBox(LPBOUNDINGBOX lpbBox)
{
    if (lpbBox != NULL) { *lpbBox = this->m_bBox; }
}

VOID Actor::GetIsActive(LPBOOL lpbActive)
{
    if (lpbActive != NULL) { *lpbActive = this->m_bIsActive; }
}

VOID Actor::SetIsActive(BOOL bActive)
{
    this->m_bIsActive = bActive;
}

VOID Actor::GetType(LPACTORTYPE lpaType)
{
    if (lpaType != NULL) { *lpaType = this->m_atType; }
}

VOID Actor::SetType(ACTORTYPE aType)
{
    this->m_atType = aType;
}

VOID Actor::GetHealth(LPINT lpnHealth)
{
    if (lpnHealth != NULL) { *lpnHealth = this->m_nHealth; }
}

VOID Actor::SetHealth(INT nHealth)
{
    this->m_nHealth = nHealth;
}

VOID Actor::GetMaxHealth(LPINT lpnHealth)
{
    if (lpnHealth != NULL) { *lpnHealth = this->m_nMaxHealth; }
}

VOID Actor::SetMaxHealth(INT nHealth)
{
    this->m_nMaxHealth = nHealth;
}

VOID Actor::GetPosition(LPV3 lpV3)
{
    if (lpV3 != NULL) { *lpV3 = this->m_vPosition; }
}

VOID Actor::SetPosition(V3& v3)
{
    this->m_vPosition = v3;
}

VOID Actor::GetScale(LPV3 lpV3)
{
    if (lpV3 != NULL) { *lpV3 = this->m_vScale; }
}

VOID Actor::SetScale(V3& v3)
{
    this->m_vScale = v3;
}

VOID Actor::SetTexture(DWORD dwWidth, DWORD dwHeight, D3DFORMAT ddFormat, LPVOID lpContent, DWORD dwStride)
{
    if (this->m_rMesh.Texture != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererTexture(this->m_lpRenderer, this->m_rMesh.Texture);
    }

    if (SUCCEEDED(CreateRendererTexture(this->m_lpRenderer,
        dwWidth, dwHeight, 0, ddFormat, &this->m_rMesh.Texture)))
    {
        RECT rect = { 0, 0, dwWidth, dwHeight };
        UpdateRendererTexture(this->m_lpRenderer,
            this->m_rMesh.Texture, &rect, (LPBYTE)lpContent, dwStride);
    }
}

VOID Actor::SetMesh(DWORD dwCount, LPTRIANGLE lpTriangles)
{
    if (this->m_Vertexes.Current != NULL)
    {
        free(this->m_Vertexes.Current);
        this->m_Vertexes.Current = NULL;
    }

    if (this->m_Vertexes.Original != NULL)
    {
        free(this->m_Vertexes.Original);
        this->m_Vertexes.Original = NULL;
    }

    if (this->m_rMesh.Vertex != INVALID_RENDERER_HANDLE)
    {
        ReleaseRendererVertexBuffer(this->m_lpRenderer, this->m_rMesh.Vertex);
        this->m_rMesh.Vertex = INVALID_RENDERER_HANDLE;
    }

    if (lpTriangles == NULL) { return;}

    this->m_Vertexes.Current = (LPACTORVERTEX)malloc(dwCount * 3 * sizeof(ACTORVERTEX));
    this->m_Vertexes.Original = (LPACTORVERTEX)malloc(dwCount * 3 * sizeof(ACTORVERTEX));

    if (this->m_Vertexes.Current != NULL && this->m_Vertexes.Original != NULL)
    {
        this->m_rMesh.VertexCount = 3 * dwCount;
        this->m_rMesh.VertexFormat = FVFACTORVERTEX;
        this->m_rMesh.VertexSize = sizeof(ACTORVERTEX);

        LPVERTEX vertexes = (LPVERTEX)lpTriangles;
       
        for (DWORD i = 0; i < this->m_rMesh.VertexCount; i++)
        {
            this->m_Vertexes.Original[i].Coord = vertexes[i].Coord;
            this->m_Vertexes.Original[i].Normal = vertexes[i].Normal;
            this->m_Vertexes.Original[i].Color = D3DCOLOR_XRGB(255, 255, 255);
            this->m_Vertexes.Original[i].UV = vertexes[i].UV;
        }

        CopyMemory(this->m_Vertexes.Current, this->m_Vertexes.Original,
            this->m_rMesh.VertexCount * this->m_rMesh.VertexSize);

        if (SUCCEEDED(CreateRendererVertexBuffer(this->m_lpRenderer,
            this->m_rMesh.VertexSize, this->m_rMesh.VertexCount,
            this->m_rMesh.VertexFormat, &this->m_rMesh.Vertex)))
        {
            UpdateRendererVertexBuffer(this->m_lpRenderer, this->m_rMesh.Vertex,
                this->m_rMesh.VertexCount * this->m_rMesh.VertexSize,
                (LPBYTE)this->m_Vertexes.Current);

        }

        this->UpdateBoundingBox(0.0f);
    }
}

VOID Actor::Update(CONST FLOAT fTime)
{
    if (!this->m_bIsActive) { return; }

    if (fTime > 0.0f)
    {
        this->m_fAngle += fTime;

        while (this->m_fAngle > 360.0f) { this->m_fAngle -= 360.0f; }

        if (this->m_Vertexes.Current != NULL && this->m_Vertexes.Original != NULL)
        {
            this->UpdateMeshCoords(fTime);
            this->UpdateMeshColors(fTime);
            this->UpdateBoundingBox(fTime);

            UpdateRendererVertexBuffer(this->m_lpRenderer, this->m_rMesh.Vertex,
                this->m_rMesh.VertexCount * this->m_rMesh.VertexSize,
                (LPBYTE)this->m_Vertexes.Current);
        }
    }

    RenderRendererMesh(this->m_lpRenderer, &this->m_rMesh);
}

VOID Actor::UpdateMeshCoords(CONST FLOAT fTime)
{
    D3DXMATRIX matRotate;
    D3DXMatrixRotationY(&matRotate, this->m_fAngle * this->m_fMultiplier);

    D3DXMATRIX matScale;
    D3DXMatrixScaling(&matScale, this->m_vScale.X, this->m_vScale.Y, this->m_vScale.Z);
    
    D3DXMATRIX matTranslate;
    D3DXMatrixTranslation(&matTranslate, this->m_vPosition.X, this->m_vPosition.Y, this->m_vPosition.Z);

    D3DXMATRIX matActor;
    D3DXMatrixIdentity(&matActor);
    D3DXMatrixMultiply(&matActor, &matActor, &matRotate);
    D3DXMatrixMultiply(&matActor, &matActor, &matScale);
    D3DXMatrixMultiply(&matActor, &matActor, &matTranslate);

    D3DXVec3TransformCoordArray(
        (LPD3DXVECTOR3)this->m_Vertexes.Current, this->m_rMesh.VertexSize,
        (LPD3DXVECTOR3)this->m_Vertexes.Original, this->m_rMesh.VertexSize,
        &matActor, this->m_rMesh.VertexCount);
}

VOID Actor::UpdateMeshColors(CONST FLOAT fTime)
{
    CONST INT value = (INT)(45.0f * sinf(this->m_fAngle));
    for (DWORD i = 0; i < this->m_rMesh.VertexCount; i++)
    {
        this->m_Vertexes.Current[i].Color = D3DCOLOR_XRGB(0, value + 120, value + 170);
    }
}

VOID Actor::UpdateBoundingBox(CONST FLOAT fTime)
{
    BOUNDINGBOX bBox;
    bBox.Min.X = bBox.Min.Y = bBox.Min.Z = FLT_MAX;
    bBox.Max.X = bBox.Max.Y = bBox.Max.Z = -FLT_MAX;

    for (DWORD i = 0; i < this->m_rMesh.VertexCount; i++)
    {
        LPACTORVERTEX vertex = &this->m_Vertexes.Current[i];

        if (vertex->Coord.X > bBox.Max.X) { bBox.Max.X = vertex->Coord.X; }
        if (vertex->Coord.Y > bBox.Max.Y) { bBox.Max.Y = vertex->Coord.Y; }
        if (vertex->Coord.Z > bBox.Max.Z) { bBox.Max.Z = vertex->Coord.Z; }

        if (vertex->Coord.X < bBox.Min.X) { bBox.Min.X = vertex->Coord.X; }
        if (vertex->Coord.Y < bBox.Min.Y) { bBox.Min.Y = vertex->Coord.Y; }
        if (vertex->Coord.Z < bBox.Min.Z) { bBox.Min.Z = vertex->Coord.Z; }
    }

    CopyMemory(&this->m_bBox, &bBox, sizeof(BOUNDINGBOX));
}

STATIC HRESULT CreateActorTexture(LPACTOR lpActor, LPASSETS lpAssets, LPCSTR lpszName)
{
    ASSETCONTENT assetContent;
    if (FAILED(GetAssetsContent(lpAssets, lpszName, &assetContent))) { return AERR_GENERIC; }

    LPIMG image = (LPIMG)malloc(assetContent.Size);
    if (image == NULL) { return AERR_OUTOFMEMORY; }

    if (FAILED(GetAssetsContentContent(lpAssets, &assetContent, image)))
    {
        free(image);
        return AERR_GENERIC;
    }

    CONST D3DFORMAT ddFormat = image->Info.Format == IMG_FORMAT_RGB565
        ? D3DFMT_R5G6B5 : D3DFMT_UNKNOWN;

    if (ddFormat == D3DFMT_UNKNOWN)
    {
        free(image);
        return AERR_GENERIC;
    }

    lpActor->SetTexture(image->Info.Width, image->Info.Height, ddFormat,
        (LPBYTE)(image) + (sizeof(IMGHEAD) + sizeof(IMGINFO)), image->Info.Stride);

    free(image);

    return S_OK;
}

STATIC HRESULT CreateActorModel(LPACTOR lpActor, LPASSETS lpAssets, LPCSTR lpszName)
{
    ASSETCONTENT assetContent;
    if (FAILED(GetAssetsContent(lpAssets, lpszName, &assetContent))) { return AERR_GENERIC; }

    LPTRIANGLEMESH mesh = (LPTRIANGLEMESH)malloc(assetContent.Size);
    if (mesh == NULL) { return AERR_OUTOFMEMORY; }

    if (FAILED(GetAssetsContentContent(lpAssets, &assetContent, mesh)))
    {
        free(mesh);
        return AERR_GENERIC;
    }

    lpActor->SetMesh(mesh->Count, mesh->Triangles);

    free(mesh);
    
    return S_OK;
}

STATIC HRESULT CreateActorDetails(LPACTOR lpActor, LPASSETS lpAssets)
{
    CHAR szName[MAX_ACTOR_BUFFER_LENGTH];
    CHAR szValue[MAX_ACTOR_BUFFER_LENGTH];
    lpActor->GetName(szValue, MAX_ACTOR_BUFFER_LENGTH);

    ASSETCONTENT assetContent;
    if (FAILED(GetAssetsContent(lpAssets, szValue, &assetContent))) { return AERR_GENERIC; }

    LPSTR lpszContent = (LPSTR)malloc(assetContent.Size + 1);
    if (lpszContent == NULL) { return AERR_OUTOFMEMORY; }

    ZeroMemory(lpszContent, assetContent.Size + 1);

    if (FAILED(GetAssetsContentContent(lpAssets, &assetContent, lpszContent)))
    {
        free(lpszContent);
        return AERR_GENERIC;
    }
    
    // HP
    DWORD dwHitPoints = DEFAULT_ACTOR_HIT_POINT_VALUE;
    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    strcpy_s(szName, MAX_ACTOR_BUFFER_LENGTH, "hp");
    AcquireSettingsValue(lpszContent, szName, szValue);
    sscanf_s(szValue, "%u", &dwHitPoints);
    lpActor->SetHealth(dwHitPoints);
    lpActor->SetMaxHealth(dwHitPoints);

    // Texture
    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    strcpy_s(szName, MAX_ACTOR_BUFFER_LENGTH, "texture");
    AcquireSettingsValue(lpszContent, szName, szValue);
    CreateActorTexture(lpActor, lpAssets, szValue);

    // Mesh
    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    strcpy_s(szName, MAX_ACTOR_BUFFER_LENGTH, "model");
    AcquireSettingsValue(lpszContent, szName, szValue);
    CreateActorModel(lpActor, lpAssets, szValue);

    free(lpszContent);

    return S_OK;
}

HRESULT CreateActor(LPASSETS lpAssets, LPRENDERER lpRenderer, LPCSTR lpszContent, DWORD indx, LPACTOR* lplpActor)
{
    if (lpAssets == NULL || lpRenderer == NULL || lplpActor == NULL) { return AERR_INVALIDPARAMS; }

    LPACTOR actor = new Actor(lpRenderer);
    if (actor == NULL) { return AERR_OUTOFMEMORY; }

    CHAR szName[MAX_ACTOR_BUFFER_LENGTH];
    CHAR szValue[MAX_ACTOR_BUFFER_LENGTH];

    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    sprintf_s(szName, MAX_ACTOR_BUFFER_LENGTH, "actor[%u]name", indx);
    AcquireSettingsValue(lpszContent, szName, szValue);
    actor->SetName(szValue);

    ACTORTYPE aType = ACTORTYPE_NONE;
    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    sprintf_s(szName, MAX_ACTOR_BUFFER_LENGTH, "actor[%u]type", indx);
    AcquireSettingsValue(lpszContent, szName, szValue);
    sscanf_s(szValue, "%u", &aType);
    actor->SetType(aType);

    V3 v3 = { 0.0f, 0.0f, 0.0f };
    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    sprintf_s(szName, MAX_ACTOR_BUFFER_LENGTH, "actor[%u]position", indx);
    AcquireSettingsValue(lpszContent, szName, szValue);
    AcquireSettingsV3(szValue, &v3);
    actor->SetPosition(v3);

    ZeroMemory(szValue, MAX_ACTOR_BUFFER_LENGTH);
    sprintf_s(szName, MAX_ACTOR_BUFFER_LENGTH, "actor[%u]scale", indx);
    AcquireSettingsValue(lpszContent, szName, szValue);
    AcquireSettingsV3(szValue, &v3);
    actor->SetScale(v3);

    if (FAILED(CreateActorDetails(actor, lpAssets)))
    {
        ReleaseActor(actor);
        return AERR_GENERIC;
    }

    *lplpActor = actor;

    return A_OK;
}

HRESULT ReleaseActor(LPACTOR lpActor)
{
    if (lpActor == NULL) { return AERR_INVALIDPARAMS; }
    
    delete lpActor;

    return A_OK;
}
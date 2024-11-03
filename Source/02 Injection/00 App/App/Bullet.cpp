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

#include "Bullet.h"
#include "Settings.h"

#include <Img.h>

#include <stdio.h>

#define DEFAULT_BULLET_HIT_POINT_VALUE  100
#define DEFAULT_BULLET_MOVE_SPEED       5.0f

#define MAX_BULLET_BUFFER_LENGTH        128
#define MAX_BULLET_LIFE_TIME            30.0f

Bullet::Bullet(LPRENDERER lpRenderer) : Actor(lpRenderer)
{
    this->m_atType = ACTORTYPE_BULLET;
    this->m_nHealth = this->m_nMaxHealth = DEFAULT_BULLET_HIT_POINT_VALUE;

    this->m_fTime = 0.0f;
    ZeroMemory(&this->m_vDirection, sizeof(V3));
}

VOID Bullet::SetIsActive(BOOL bActive)
{
    this->m_bIsActive = bActive;

    if (this->m_bIsActive)
    {
        this->m_fTime = 0.0f;
        if (this->m_Vertexes.Current != NULL && this->m_Vertexes.Original != NULL)
        {
            CopyMemory(this->m_Vertexes.Current, this->m_Vertexes.Original,
                this->m_rMesh.VertexCount * this->m_rMesh.VertexSize);
        }
    }
}

VOID Bullet::SetType(ACTORTYPE aType) {}

VOID Bullet::GetDirection(LPV3 lpV3)
{
    if (lpV3 != NULL) { *lpV3 = this->m_vDirection; }
}

VOID Bullet::SetDirection(V3& v3)
{
    this->m_vDirection = v3;
}

VOID Bullet::Update(CONST FLOAT fTime)
{
    this->m_fTime += fTime;

    if (this->m_fTime >= MAX_BULLET_LIFE_TIME)
    {
        this->m_fTime = 0.0f;
        this->m_bIsActive = FALSE;
    }

    Actor::Update(fTime);
}

VOID Bullet::UpdateMeshCoords(CONST FLOAT fTime)
{
    D3DXMATRIX matRotate;
    D3DXMatrixRotationY(&matRotate, this->m_fAngle * this->m_fMultiplier);

    D3DXMATRIX matScale;
    D3DXMatrixScaling(&matScale, this->m_vScale.X, this->m_vScale.Y, this->m_vScale.Z);
    
    D3DXMATRIX matTranslate;
    D3DXMatrixTranslation(&matTranslate,
        this->m_vPosition.X + this->m_fTime * this->m_vDirection.X * DEFAULT_BULLET_MOVE_SPEED,
        this->m_vPosition.Y + this->m_fTime * this->m_vDirection.Y * DEFAULT_BULLET_MOVE_SPEED,
        this->m_vPosition.Z + this->m_fTime * this->m_vDirection.Z * DEFAULT_BULLET_MOVE_SPEED);

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

VOID Bullet::UpdateMeshColors(CONST FLOAT fTime)
{
    CONST INT value = (INT)(45.0f * sinf(this->m_fAngle));
    for (DWORD i = 0; i < this->m_rMesh.VertexCount; i++)
    {
        this->m_Vertexes.Current[i].Color = D3DCOLOR_XRGB(value + 170, value + 120, 0);
    }
}

STATIC HRESULT CreateBulletTexture(LPBULLET lpBullet, LPASSETS lpAssets, LPCSTR lpszName)
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

    lpBullet->SetTexture(image->Info.Width, image->Info.Height, ddFormat,
        (LPBYTE)(image) + (sizeof(IMGHEAD) + sizeof(IMGINFO)), image->Info.Stride);

    free(image);

    return S_OK;
}

STATIC HRESULT CreateBulletModel(LPBULLET lpBullet, LPASSETS lpAssets, LPCSTR lpszName)
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

    lpBullet->SetMesh(mesh->Count, mesh->Triangles);

    free(mesh);
    
    return S_OK;
}

STATIC HRESULT CreateBulletDetails(LPBULLET lpBullet, LPASSETS lpAssets)
{
    CHAR szName[MAX_BULLET_BUFFER_LENGTH];
    CHAR szValue[MAX_BULLET_BUFFER_LENGTH];

    ASSETCONTENT assetContent;
    if (FAILED(GetAssetsContent(lpAssets, "bullet.desc", &assetContent))) { return AERR_GENERIC; }

    LPSTR lpszContent = (LPSTR)malloc(assetContent.Size + 1);
    if (lpszContent == NULL) { return AERR_OUTOFMEMORY; }

    ZeroMemory(lpszContent, assetContent.Size + 1);

    if (FAILED(GetAssetsContentContent(lpAssets, &assetContent, lpszContent)))
    {
        free(lpszContent);
        return AERR_GENERIC;
    }
    
    // HP
    DWORD dwHitPoints = DEFAULT_BULLET_HIT_POINT_VALUE;
    ZeroMemory(szValue, MAX_BULLET_BUFFER_LENGTH);
    strcpy_s(szName, MAX_BULLET_BUFFER_LENGTH, "hp");
    AcquireSettingsValue(lpszContent, szName, szValue);
    sscanf_s(szValue, "%u", &dwHitPoints);
    lpBullet->SetHealth(dwHitPoints);
    lpBullet->SetMaxHealth(dwHitPoints);

    // Texture
    ZeroMemory(szValue, MAX_BULLET_BUFFER_LENGTH);
    strcpy_s(szName, MAX_BULLET_BUFFER_LENGTH, "texture");
    AcquireSettingsValue(lpszContent, szName, szValue);
    CreateBulletTexture(lpBullet, lpAssets, szValue);

    // Mesh
    ZeroMemory(szValue, MAX_BULLET_BUFFER_LENGTH);
    strcpy_s(szName, MAX_BULLET_BUFFER_LENGTH, "model");
    AcquireSettingsValue(lpszContent, szName, szValue);
    CreateBulletModel(lpBullet, lpAssets, szValue);

    free(lpszContent);

    return S_OK;
}

HRESULT CreateBullet(LPASSETS lpAssets, LPRENDERER lpRenderer, LPBULLET* lplpBullet)
{
    if (lpAssets == NULL || lpRenderer == NULL || lplpBullet == NULL) { return AERR_INVALIDPARAMS; }

    LPBULLET bullet = new Bullet(lpRenderer);
    if (bullet == NULL) { return AERR_OUTOFMEMORY; }

    V3 position = { 0.0f, 0.0f, 0.0f };
    bullet->SetPosition(position);

    V3 scale = { 1.0f, 1.0f, 1.0f };
    bullet->SetScale(scale);

    if (FAILED(CreateBulletDetails(bullet, lpAssets)))
    {
        ReleaseBullet(bullet);
        return AERR_GENERIC;
    }

    *lplpBullet = bullet;

    return A_OK;
}

HRESULT ReleaseBullet(LPBULLET lpBullet)
{
    if (lpBullet == NULL) { return AERR_INVALIDPARAMS; }
    
    delete lpBullet;

    return A_OK;
}
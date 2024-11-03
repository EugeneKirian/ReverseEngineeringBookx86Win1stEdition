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

#include "Assets.h"
#include "AABB.h"
#include "Renderer.h"

#define MAX_ACTOR_NAME_LENGTH   128

typedef enum ActorType
{
    ACTORTYPE_NONE          = 0,
    ACTORTYPE_ENEMY         = 1,
    ACTORTYPE_BULLET        = 2,
    ACTORTYPE_FORCE_DWORD   = 0x7FFFFF
} ACTORTYPE, * LPACTORTYPE;

#define FVFACTORVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

typedef struct ActorVertex
{
    V3      Coord;
    V3      Normal;
    DWORD   Color;
    V2      UV;
} ACTORVERTEX, * LPACTORVERTEX;

class Actor
{
public:
    Actor(LPRENDERER lpRenderer);
    virtual void        GetName(LPSTR lpszName, DWORD dwLength);
    virtual void        SetName(LPCSTR lpszName);
    virtual void        GetBoundingBox(LPBOUNDINGBOX lpbBox);
    virtual void        GetIsActive(LPBOOL lpbActive);
    virtual void        SetIsActive(BOOL bActive);
    virtual void        GetType(LPACTORTYPE lpaType);
    virtual void        SetType(ACTORTYPE aType);
    virtual void        GetHealth(LPINT lpnHealth);
    virtual void        SetHealth(INT nHealth);
    virtual void        GetMaxHealth(LPINT lpnHealth);
    virtual void        SetMaxHealth(INT nHealth);
    virtual void        GetPosition(LPV3 lpV3);
    virtual void        SetPosition(V3& v3);
    virtual void        GetScale(LPV3 lpV3);
    virtual void        SetScale(V3& v3);
    virtual void        SetTexture(DWORD dwWidth, DWORD dwHeight, D3DFORMAT ddFormat, LPVOID lpContent, DWORD dwStride);
    virtual void        SetMesh(DWORD dwCount, LPTRIANGLE lpTriangles);
    virtual void        Update(CONST FLOAT fTime);
    virtual ~Actor();
protected:
    virtual void        UpdateMeshCoords(CONST FLOAT fTime);
    virtual void        UpdateMeshColors(CONST FLOAT fTime);
    virtual void        UpdateBoundingBox(CONST FLOAT fTime);

protected:
    LPRENDERER          m_lpRenderer;

    CHAR                m_szName[MAX_ACTOR_NAME_LENGTH];

    BOOL                m_bIsActive;
    ACTORTYPE           m_atType;

    INT                 m_nHealth;
    INT                 m_nMaxHealth;

    FLOAT               m_fAngle;
    FLOAT               m_fMultiplier;
    V3                  m_vPosition;
    V3                  m_vScale;

    struct
    {
        LPACTORVERTEX   Current;
        LPACTORVERTEX   Original;
    } m_Vertexes;
    BOUNDINGBOX         m_bBox;
    RENDERERMESH        m_rMesh;
};

typedef Actor ACTOR, * LPACTOR;

HRESULT CreateActor(LPASSETS lpAssets, LPRENDERER lpRenderer, LPCSTR lpszContent, DWORD indx, LPACTOR* lpActor);
HRESULT ReleaseActor(LPACTOR lpActor);
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

#include "Camera.h"

#define CAMERA_MOVE_SPEED    50.0f

Camera::Camera(LPINPUT lpInput, LPRENDERER lpRenderer, V3& vPosition, V3& vTarget, V3& vUp, V3 vForward, V3& vRight)
{
    this->m_lpInput = lpInput;
    this->m_lpRenderer = lpRenderer;

    this->m_vPosition = vPosition;
    this->m_vTarget = vTarget;

    this->m_vUp = vUp;
    this->m_vForward = vForward;
    this->m_vRight = vRight;
}

VOID Camera::Update(CONST FLOAT fTime)
{
    if (fTime >= 0.0f)
    {
        this->Move(fTime);

        if (this->m_lpInput->IsMouse(INPUT_RIGHT_MOUSE))
        {
            INT nX = 0, nY = 0;
            this->m_lpInput->GetMouse(INPUT_RIGHT_MOUSE, TRUE, &nX, &nY);

            if (nX != 0 || nY != 0)
            {
                CONST FLOAT fAngleX = (FLOAT)nX / ((FLOAT)this->m_lpRenderer->Window.Width / 2.0f);
                CONST FLOAT fAngleY = (FLOAT)nY / ((FLOAT)this->m_lpRenderer->Window.Height / 2.0f);

                this->Rotate(-fAngleX, -fAngleY);
            }
        }
    }

    this->UpdateMatrixes();
}

VOID Camera::GetPosition(LPV3 lpV3)
{
    if (lpV3 != NULL) { *lpV3 = this->m_vPosition; }
}

VOID Camera::GetDirection(LPV3 lpV3)
{
    if (lpV3 != NULL)
    {
        D3DXVec3Subtract((LPD3DXVECTOR3)lpV3, (LPD3DXVECTOR3)&this->m_vTarget, (LPD3DXVECTOR3)&this->m_vPosition);
        D3DXVec3Normalize((LPD3DXVECTOR3)lpV3, (LPD3DXVECTOR3)lpV3);
    }
}

VOID Camera::UpdateMatrixes()
{
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity(&matWorld);

    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, (LPD3DXVECTOR3)&this->m_vPosition,
        (LPD3DXVECTOR3)&this->m_vTarget, (LPD3DXVECTOR3)&this->m_vUp);

    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f,
        (FLOAT)this->m_lpRenderer->Window.Width / (FLOAT)this->m_lpRenderer->Window.Height,
        1.0f, 100.0f);

    SetRendererCullMode(this->m_lpRenderer, D3DCULL_CCW);
    SetRendererMatrixes(this->m_lpRenderer, &matWorld, &matView, &matProj);
}

VOID Camera::Move(CONST FLOAT fTime)
{
    CONST FLOAT fForwardBack = fTime * CAMERA_MOVE_SPEED * 
        ((this->m_lpInput->IsPressing('W') ? 1.0f : 0.0f)
        + (this->m_lpInput->IsPressing('S') ? -1.0f : 0.0f));

    CONST FLOAT fRightLeft = fTime * CAMERA_MOVE_SPEED *
        ((this->m_lpInput->IsPressing('D') ? 1.0f : 0.0f)
        + (this->m_lpInput->IsPressing('A') ? -1.0f : 0.0f));

    if (fForwardBack != 0.0f)
    {
        this->m_vPosition.X += fForwardBack * this->m_vForward.X;
        this->m_vPosition.Y += fForwardBack * this->m_vForward.Y;
        this->m_vPosition.Z += fForwardBack * this->m_vForward.Z;

        this->m_vTarget.X += fForwardBack * this->m_vForward.X;
        this->m_vTarget.Y += fForwardBack * this->m_vForward.Y;
        this->m_vTarget.Z += fForwardBack * this->m_vForward.Z;
    }

    if (fRightLeft != 0.0f)
    {
        this->m_vPosition.X += fRightLeft * this->m_vRight.X;
        this->m_vPosition.Y += fRightLeft * this->m_vRight.Y;
        this->m_vPosition.Z += fRightLeft * this->m_vRight.Z;

        this->m_vTarget.X += fRightLeft * this->m_vRight.X;
        this->m_vTarget.Y += fRightLeft * this->m_vRight.Y;
        this->m_vTarget.Z += fRightLeft * this->m_vRight.Z;
    }
}

VOID Camera::Rotate(CONST FLOAT fAngleX, CONST FLOAT fAngleY)
{
    D3DXMATRIX matRotation;
    D3DXMatrixRotationYawPitchRoll(&matRotation, fAngleX, fAngleY, 0.0f);

    D3DXVec3TransformCoord((LPD3DXVECTOR3)&this->m_vTarget, (LPD3DXVECTOR3)&this->m_vForward, &matRotation);
    D3DXVec3TransformCoord((LPD3DXVECTOR3)&this->m_vUp, (LPD3DXVECTOR3)&this->m_vUp, &matRotation);

    D3DXVec3Normalize((LPD3DXVECTOR3)&this->m_vForward, (LPD3DXVECTOR3)&this->m_vTarget);
    D3DXVec3Cross((LPD3DXVECTOR3)&this->m_vRight, (LPD3DXVECTOR3)&this->m_vUp, (LPD3DXVECTOR3)&this->m_vTarget);
    D3DXVec3Normalize((LPD3DXVECTOR3)&this->m_vRight, (LPD3DXVECTOR3)&this->m_vRight);

    this->m_vTarget.X = this->m_vPosition.X + this->m_vTarget.X;
    this->m_vTarget.Y = this->m_vPosition.Y + this->m_vTarget.Y;
    this->m_vTarget.Z = this->m_vPosition.Z + this->m_vTarget.Z;
}
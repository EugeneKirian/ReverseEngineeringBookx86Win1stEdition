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

#include "Input.h"
#include "Renderer.h"

class Camera
{
public:
    Camera(LPINPUT lpInput, LPRENDERER lpRenderer, V3& vPosition, V3& vTarget, V3& vUp, V3 vForward, V3& vRight);
    virtual void Update(CONST FLOAT fTime);
    void GetPosition(LPV3 lpV3);
    void GetDirection(LPV3 lpV3);
protected:
    void Move(CONST FLOAT fTime);
    void Rotate(CONST FLOAT fAngleX, CONST FLOAT fAngleY);
    void UpdateMatrixes();
private:
    LPINPUT     m_lpInput;
    LPRENDERER  m_lpRenderer;
    V3          m_vPosition;
    V3          m_vTarget;
    V3          m_vUp;
    V3          m_vForward;
    V3          m_vRight;
};

typedef Camera CAMERA, * LPCAMERA;
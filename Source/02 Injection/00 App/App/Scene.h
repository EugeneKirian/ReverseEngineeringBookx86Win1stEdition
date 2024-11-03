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
#include "Input.h"
#include "Renderer.h"

#define MAX_SCENE_NAME_LENGTH   256

#define SCENENAME(x) ("scene" # x ##".scene")

#define SERR_GENERIC        E_FAIL
#define SERR_INVALIDPARAMS  E_INVALIDARG
#define SERR_OUTOFMEMORY    E_OUTOFMEMORY

#define S_WIN               1

struct Scene;
typedef struct Scene SCENE, * LPSCENE;

HRESULT CreateScene(LPASSETS lpAssets, LPINPUT lpInput, LPRENDERER lpRenderer, LPASSETCONTENT lpAssetContent, LPSCENE* lplpScene);
HRESULT ReleaseScene(LPSCENE lpScene);

HRESULT UpdateScene(LPSCENE lpScene, CONST FLOAT fTime);
HRESULT GetSceneColor(LPSCENE lpScene, LPDWORD lpdwColor);
HRESULT GetSceneNext(LPSCENE lpScene, LPSTR lpszNext, DWORD dwSize);
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
#include "Scene.h"

#define GAME_NAME           "Space Arcade!"

#define G_OK                S_OK
#define GERR_GENERIC        E_FAIL
#define GERR_INVALIDPARAMS  E_INVALIDARG
#define GERR_OUTOFMEMORY    E_OUTOFMEMORY

struct Game;
typedef struct Game GAME, * LPGAME;

HRESULT CreateGame(LPASSETS lpAssets, LPRENDERER lpRenderer, LPINPUT lpInput, LPGAME* lplpGame);
HRESULT StartGame(LPGAME lpGame, LPCSTR lpszName);
HRESULT UpdateGame(LPGAME lpGame, FLOAT fTime);
HRESULT ReleaseGame(LPGAME lpGame);
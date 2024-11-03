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

#include <dxerr.h>

#include <stdio.h>

#define MAX_ERROR_MESSAGE_LENGTH    255

#define WINDOW_WIDTH        640
#define WINDOW_HEIGHT       480

HWND            g_hWnd;
LPASSETS        g_lpAssets;
LPRENDERER      g_lpRenderer;
LPGAME          g_lpGame;
LPINPUT         g_lpInput;

LARGE_INTEGER   g_liStart, g_liNow, g_liFrequency;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
        {
            if (g_lpInput == NULL) { break; }

            g_lpInput->SetMouse(INPUT_LEFT_MOUSE, msg == WM_LBUTTONDOWN,
                GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

            break;
        }
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        {
            if (g_lpInput == NULL) { break; }

            g_lpInput->SetMouse(INPUT_RIGHT_MOUSE, msg == WM_RBUTTONDOWN,
                GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

            break;
        }
    case WM_MOUSEMOVE:
        {
            if (g_lpInput == NULL) { break; }

            if (wParam & MK_LBUTTON)
            {
                g_lpInput->SetMouse(INPUT_LEFT_MOUSE, TRUE,
                    GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }

            if (wParam & MK_RBUTTON)
            {
                g_lpInput->SetMouse(INPUT_RIGHT_MOUSE, TRUE,
                    GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }

            break;
        }
    case WM_NCMOUSEMOVE:
        {
            if (g_lpInput == NULL) { break; }

            if ((wParam & MK_LBUTTON) || (wParam & MK_RBUTTON))
            {
                g_lpInput->SetMouse(INPUT_LEFT_MOUSE, FALSE, 0, 0);
                g_lpInput->SetMouse(INPUT_RIGHT_MOUSE, FALSE, 0, 0);
            }

            break;
        }
    case WM_SIZE:
        {
            CONST UINT uWidth = LOWORD(lParam);
            CONST UINT uHeight = HIWORD(lParam);

            if (uWidth != 0 && uHeight != 0 && g_lpRenderer != NULL)
            {
                g_lpRenderer->Window.Width = uWidth;
                g_lpRenderer->Window.Height = uHeight;
            }
            break;
        }
    case WM_DESTROY:
        {
            PostQuitMessage(0);

            return 0;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL Initialize(HINSTANCE hInst)
{
    srand(timeGetTime());

    if (!QueryPerformanceFrequency(&g_liFrequency))
    {
        MessageBoxA(NULL, "Unable to get performance counter.", GAME_NAME, MB_ICONHAND);
        return FALSE;
    }

    {
        CHAR szWorkDir[MAX_PATH];
        if (GetCurrentDirectoryA(MAX_PATH, szWorkDir) == 0)
        {
            MessageBoxA(NULL, "Unable to get current working directory.", GAME_NAME, MB_ICONHAND);
            return FALSE;
        }

        HRESULT result = A_OK;
        if (FAILED(result = CreateAssets(szWorkDir, &g_lpAssets)))
        {
            MessageBoxA(NULL, "Unable to load game assets.", GAME_NAME, MB_ICONHAND);
            return FALSE;
        }
    }

    WNDCLASSEXA wc =
    {
        sizeof(WNDCLASSEXA), CS_CLASSDC, MsgProc,
        0, 0, hInst, NULL, NULL, NULL, NULL, GAME_NAME, NULL
    };

    if (RegisterClassExA(&wc) == NULL)
    {
        MessageBoxA(NULL, "Unable to create game window class.", GAME_NAME, MB_ICONHAND);
        ReleaseAssets(g_lpAssets);
        return FALSE;
    }

    BOOL bWindow = StrStrIA(GetCommandLineA(), "-fullscreen") == NULL;
    DWORD dwWidth = WINDOW_WIDTH;
    DWORD dwHeight = WINDOW_HEIGHT;

    {
        INT dwX = CW_USEDEFAULT;
        INT dwY = CW_USEDEFAULT;

        DWORD dwWindow = WS_OVERLAPPEDWINDOW;

        if (!bWindow)
        {
            dwX = dwY = 0;
            dwWindow = WS_POPUP;
            dwWidth = GetSystemMetrics(SM_CXSCREEN);
            dwHeight = GetSystemMetrics(SM_CYSCREEN);
        }

        if ((g_hWnd = CreateWindowA(GAME_NAME, GAME_NAME, dwWindow,
            dwX, dwY, dwWidth, dwHeight, NULL, NULL, hInst, NULL)) == NULL)
        {
            MessageBoxA(NULL, "Unable to create game window.", GAME_NAME, MB_ICONHAND);
            ReleaseAssets(g_lpAssets);
            UnregisterClassA(GAME_NAME, hInst);
            return FALSE;
        }
    }

    {
        HRESULT result = R_OK;
        if (FAILED(result = CreateRenderer(g_hWnd, bWindow, dwWidth, dwHeight, &g_lpRenderer)))
        {
            CHAR message[MAX_ERROR_MESSAGE_LENGTH];
            sprintf_s(message, MAX_ERROR_MESSAGE_LENGTH,
                "Unable to initialize Direct3D9: %s", DXGetErrorDescription(result));
            MessageBoxA(NULL, message, GAME_NAME, MB_ICONHAND);
            goto CLEANUP;
        }
    }

    {
        HRESULT result = G_OK;
        g_lpInput = new Input();
        if (FAILED(result = CreateGame(g_lpAssets, g_lpRenderer, g_lpInput, &g_lpGame)))
        {
            MessageBoxA(NULL, "Unable to create game class.", GAME_NAME, MB_ICONHAND);
            goto CLEANUP;
        }
    }

    {
        HRESULT result = G_OK;
        LPCSTR lpszSceneName = SCENENAME(1);
        if (FAILED(result = StartGame(g_lpGame, lpszSceneName)))
        {
            CHAR message[MAX_ERROR_MESSAGE_LENGTH];
            sprintf_s(message, MAX_ERROR_MESSAGE_LENGTH,
                "Unable to start game level (%s).", lpszSceneName);
            MessageBoxA(NULL, message, GAME_NAME, MB_ICONHAND);
            goto CLEANUP;
        }
    }

    return TRUE;

    CLEANUP:
    if (g_lpGame != NULL) { ReleaseGame(g_lpGame); g_lpGame = NULL; }
    if (g_lpAssets != NULL) { ReleaseAssets(g_lpAssets); g_lpAssets = NULL; }
    if (g_lpRenderer != NULL) { ReleaseRenderer(g_lpRenderer); g_lpRenderer = NULL; }
    if (g_lpInput != NULL) { delete g_lpInput; g_lpInput = NULL; }

    DestroyWindow(g_hWnd);
    UnregisterClassA(GAME_NAME, hInst);
    return FALSE;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    if (!Initialize(hInst)) { return EXIT_FAILURE; }

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    QueryPerformanceCounter(&g_liStart);

    while (msg.message != WM_QUIT)
    {
        if (!PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            QueryPerformanceCounter(&g_liNow);

            CONST DOUBLE llDiff = (DOUBLE)(g_liNow.QuadPart - g_liStart.QuadPart);
            CONST FLOAT fTime = (FLOAT)(llDiff / (DOUBLE)g_liFrequency.QuadPart);

            HRESULT result = R_OK;
            if (FAILED(result = UpdateGame(g_lpGame, fTime)))
            {
                if (!IsIconic(g_hWnd))
                {
                    CHAR message[MAX_ERROR_MESSAGE_LENGTH];
                    sprintf_s(message, MAX_ERROR_MESSAGE_LENGTH, "Direct3D9: %s",
                        DXGetErrorDescription(result));
                    MessageBox(NULL, message, GAME_NAME, MB_ICONHAND);
                    break;
                }
            }

            g_liStart = g_liNow;

            continue;
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (g_lpGame != NULL) { ReleaseGame(g_lpGame); g_lpGame = NULL; }
    if (g_lpAssets != NULL) { ReleaseAssets(g_lpAssets); g_lpAssets = NULL; }
    if (g_lpRenderer != NULL) { ReleaseRenderer(g_lpRenderer); g_lpRenderer = NULL; }
    if (g_lpInput != NULL) { delete g_lpInput; g_lpInput = NULL; }
    DestroyWindow(g_hWnd);
    UnregisterClassA(GAME_NAME, hInst);
    return EXIT_SUCCESS;
}
// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "CMainApp.h"
#include "CDInputMgr.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND    g_hWnd;
UINT    g_ResizeWidth = 0, g_ResizeHeight;
IDirect3DTexture9* g_pMinimapTexture = nullptr;
IDirect3DSurface9* g_pMinimapSurface = nullptr;
IDirect3DSurface9* g_pMinimapDepthStencil = nullptr;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HMODULE hUser = LoadLibraryW(L"user32.dll");
    if (hUser)
    {
        auto pSetCtx = reinterpret_cast<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>(
            GetProcAddress(hUser, "SetProcessDpiAwarenessContext"));
        if (pSetCtx)
        {
            // 최신 권장: 모니터별 DPI v2
            pSetCtx(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }
        else
        {
            // 폴백(구 API)
            SetProcessDPIAware();
        }
        FreeLibrary(hUser);
    }

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;
    msg.message = WM_NULL;

    CMainApp* pMainApp = CMainApp::Create();

    // 입력 장치
    CDInputMgr::GetInstance()->Ready_InputDev(g_hInst, g_hWnd);

    if (nullptr == pMainApp)
        return FALSE;

    // 타이머 설치
    if (FAILED(CTimerMgr::GetInstance()->Ready_Timer(L"Timer_Global")))
        return E_FAIL;

    if (FAILED(CTimerMgr::GetInstance()->Ready_Timer(L"Timer_FPS60")))
        return E_FAIL;

    // 프레임 설치
    if (FAILED(CFrameMgr::GetInstance()->Ready_Frame(L"Frame60", 60.f)))
        return E_FAIL;

    // 기본 메시지 루프입니다
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        if (pMainApp->IsLost_Device()) {
            if (pMainApp->ResetTest())
                continue;
        }

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            pMainApp->Resize_MainApp(g_ResizeWidth, g_ResizeHeight);
            g_ResizeWidth = g_ResizeHeight = 0;
            pMainApp->Reset_MainApp();
        }

        CTimerMgr::GetInstance()->Set_TimeDelta(L"Timer_Global");
        _float fGlobal_TimeDelta = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_Global");


        CDInputMgr::GetInstance()->Update_InputDev();

        if (CFrameMgr::GetInstance()->IsPermit_Call(L"Frame60", fGlobal_TimeDelta))
        {
            CTimerMgr::GetInstance()->Set_TimeDelta(L"Timer_FPS60");
            _float fFPS60_DeltaTime = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
            _float fFixed_DeltaTime;
            int fixedStep = CTimerMgr::GetInstance()->Get_FixedStep(L"Timer_FPS60", &fFixed_DeltaTime);

            CDInputMgr::GetInstance()->Update_InputDev();

            for(int i=0; i<fixedStep; ++i)
                pMainApp->FixedUpdate_MainApp(fFixed_DeltaTime);

            // 이 부분이 FixedUpdate 말한대로 이번 프레임에 FixedUpdate를 몇번 호출해야하나 횟수를 얻어서 여러번
            // 실행한다.

            pMainApp->Update_MainApp(fFPS60_DeltaTime);
            pMainApp->LateUpdate_MainApp(fFPS60_DeltaTime);
            pMainApp->Render_MainApp();
        }
    }

    _ulong dwRefCnt(0);

    if (dwRefCnt = Engine::Safe_Release(pMainApp))
    {
        MSG_BOX("MainApp Release Failed");
        return -1;
    }

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    RECT rc{ 0,0, WINCX, WINCY };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        rc.right - rc.left,
        rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(g_hWnd);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

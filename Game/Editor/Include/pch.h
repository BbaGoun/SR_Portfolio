// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#define _EDITOR

#include "Engine_Define.h"
#include "Editor_Function.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include <cmath>
#include <commdlg.h>
#pragma comment(lib, "Comdlg32.lib")

#pragma warning(disable : 4005)

#include <process.h>

extern LPDIRECT3D9				g_pD3D;
extern LPDIRECT3DDEVICE9		g_pd3dDevice;
extern bool						g_DeviceLost;
extern UINT						g_ResizeWidth;
extern UINT						g_ResizeHeight;
extern D3DPRESENT_PARAMETERS	g_d3dpp;
extern float					main_scale;
extern HWND						g_hWnd;
extern ImFont*					g_pFontBold;


extern bool						g_bSelected;
extern uint64_t					g_uSelected;

extern ImGuizmo::OPERATION		g_GizmoOp;
extern ImGuizmo::MODE			g_GizmoMode;

using namespace std;

#endif //PCH_H

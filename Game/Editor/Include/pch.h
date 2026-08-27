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

#include <functional>

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
extern uint32_t					g_uSelected;

extern bool						g_bEdit;
extern bool                     g_bPointSelected;
extern uint32_t                 g_uPointSelected;

extern bool                     g_bHMPick;
extern _vec3                    g_vHMPickPos;

extern bool                     g_bMoveTo;

extern ImGuizmo::OPERATION		g_GizmoOp;
extern ImGuizmo::MODE			g_GizmoMode;


inline void Set_ObjSelected(uint32_t id) {
    if (!g_bEdit) {
        g_bSelected = true;
        g_uSelected = id;
    }
}

inline void Free_ObjSelected() {
    if (!g_bEdit) {
        g_bSelected = false;
        g_uSelected = 0;
    }
}
inline void Set_PointSelected(uint32_t id) {
    if (g_bEdit) {
        g_bPointSelected = true;
        g_uPointSelected = id;
    }
}

inline void Free_PointSelected() {
    if (g_bEdit) {
        g_bPointSelected = false;
        g_uPointSelected = 0;
    }
}

inline void Set_HMPick(_vec3 pos) {
    if (g_bEdit) {
        g_bHMPick = true;
        g_vHMPickPos = pos;
    }
}

inline void Free_HMPick() {
    if (g_bEdit) {
        g_bHMPick = false;
        g_vHMPickPos = { 0, 0, 0 };
    }
}

using namespace std;

#endif //PCH_H

#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator
{
public:
	static _matrix* CustomLookAtLH(_matrix* pOut, _vec3* pEye, _vec3* pAt, _vec3* pUp);

	static _matrix* CustomPerspectiveFovLH(_matrix* pOut, _float FOV, _float fAspect, _float fNear, _float fFar);

	template<int N>
	static uint32_t Get_LayerMask(COLLISION_LAYER layers[N]);

	static void DrawRayLine(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vStart, _vec3 vEnd, D3DCOLOR color);
};

template<int N>
inline uint32_t CCalculator::Get_LayerMask(COLLISION_LAYER layers[N])
{
	uint32_t collisionMask = 0;
	for (int i = 0; i < N; ++i) {
		collisionMask |= (1u << layers[i]);
	}
	return collisionMask;
}

END
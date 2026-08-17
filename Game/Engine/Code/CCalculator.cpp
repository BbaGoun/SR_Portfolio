#include "CCalculator.h"

_matrix* CCalculator::CustomLookAtLH(_matrix* pOut, _vec3* pEye, _vec3* pAt, _vec3* pUp)
{
	// 1. pEye를 원점으로 보내는 행렬
	_matrix matTrans;
	// 항등 행렬 만들기
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j)
				matTrans.m[i][j] = 1;
			else
				matTrans.m[i][j] = 0;
		}
	}

	_vec3 vInvEye = -(*pEye);
	memcpy(&matTrans.m[3][0], &vInvEye, sizeof(vInvEye));

	// 2. Eye -> At이 +z 방향인 로컬 축을 월드 축으로 돌려놓기 위한 행렬
	_matrix matRot;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j)
				matRot.m[i][j] = 1;
			else
				matRot.m[i][j] = 0;
		}
	}

	_vec3 vLook = *(pAt)-*(pEye);
	// 외적을 할 것이므로 정규화
	_float length = sqrtf(
		vLook.x * vLook.x +
		vLook.y * vLook.y +
		vLook.z * vLook.z
	);
	vLook /= length;

	// 2-1. vToAt과 Up을 외적하여 로컬 right 축을 얻기
	/*	외적 식
		x1	y1	z1	x1
		x2	y2	z2	x2
		y1z2 - z1y2, z1x2 - x1z2, x1y2 - y1x2
	*/
	// 오른쪽이 나오도록 up에 vToAt을 외적
	_vec3 vUp = *(pUp);
	_vec3 vRight = {
		vUp.y * vLook.z - vUp.z * vLook.y,
		vUp.z * vLook.x - vUp.x * vLook.z,
		vUp.x * vLook.y - vUp.y * vLook.x
	};
	length = sqrtf(
		vRight.x * vRight.x +
		vRight.y * vRight.y +
		vRight.z * vRight.z
	);
	vRight /= length;

	// 2-2. vToAt에 right를 외적하여 진짜 up을 구함
	vUp = {
		vLook.y * vRight.z - vLook.z * vRight.y,
		vLook.z * vRight.x - vLook.x * vRight.z,
		vLook.x * vRight.y - vLook.y * vRight.x
	};
	length = sqrtf(
		vUp.x * vUp.x +
		vUp.y * vUp.y +
		vUp.z * vUp.z
	);
	vUp /= length;

	// 2-3. 로컬 축의 Right, Up, Look을 붙인 행렬 A라 해보자.
	// 행렬 A는 직교행렬이므로 행렬 A의 전치행렬이 행렬 A의 역행렬이다.
	matRot.m[0][0] = vRight.x;
	matRot.m[0][1] = vUp.x;
	matRot.m[0][2] = vLook.x;

	matRot.m[1][0] = vRight.y;
	matRot.m[1][1] = vUp.y;
	matRot.m[1][2] = vLook.y;
	
	matRot.m[2][0] = vRight.z;
	matRot.m[2][1] = vUp.z;
	matRot.m[2][2] = vLook.z;

	// 3. 두 행렬을 결합.
	_matrix matView = matTrans * matRot;

	memcpy(pOut->m, &matView.m, sizeof(float) * 4 * 4);

	return pOut;
}

_matrix* CCalculator::CustomPerspectiveFovLH(_matrix* pOut, _float FOV, _float fAspect, _float fNear, _float fFar)
{
	_matrix matProj;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j)
				matProj.m[i][j] = 1;
			else
				matProj.m[i][j] = 0;
		}
	}

	float scale = 1/tan(FOV * 0.5f);

	matProj.m[0][0] = scale / fAspect;
	matProj.m[1][1] = scale;
	matProj.m[2][2] = fFar / (fFar - fNear);
	matProj.m[3][2] = -(fNear * fFar) / (fFar - fNear);
	matProj.m[2][3] = 1;
	matProj.m[3][3] = 0;

	memcpy(pOut->m, &matProj.m, sizeof(float) * 4 * 4);

	return pOut;
}

void CCalculator::DrawRayLine(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vStart, _vec3 vEnd, D3DXCOLOR color)
{
	VTXLINE vertices[]{
		{vStart, color},
		{vEnd, color}
	};

	_matrix matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	pGraphicDev->SetTransform(D3DTS_WORLD, &matIdentity);

	pGraphicDev->SetVertexDeclaration(nullptr);
	pGraphicDev->SetTexture(0, nullptr);
	pGraphicDev->SetFVF(FVF_LINE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	pGraphicDev->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(VTXLINE));

	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

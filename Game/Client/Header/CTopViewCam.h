#pragma once
#include "CCamera.h"

class CTopViewCam : public CCamera
{
private:
	explicit CTopViewCam(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTopViewCam(const CTopViewCam& rhs);
	virtual ~CTopViewCam();

public:
	HRESULT		Ready_GameObject(const _vec3& pEye,
								const _vec3& pAt,
								const _vec3& pUp,
								const _float& fFov,
								const _float& fAspect,
								const _float& fNear,
								const _float& fFar);

	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual void	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject() {}

private:
	void Mouse_Input();
	void ClickTarget();

private:
	_float m_fYaw;
	_float m_fDistScale;

public:
	static CTopViewCam* Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _vec3& pEye = { 0, 30, -30 },
								const _vec3& pAt = { 0, 0, 0 },
								const _vec3& pUp = { 0, 1, 0 },
								const _float& fFov = D3DXToRadian(60.f),
								const _float& fAspect = (_float)WINCX / WINCY,
								const _float& fNear = 1.f,
								const _float& fFar = 1000.f);

private:
	virtual void		Free();

};

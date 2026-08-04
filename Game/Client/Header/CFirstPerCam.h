#pragma once
#include "CCamera.h"
class CFirstPerCam :
    public CCamera
{
private:
	explicit CFirstPerCam(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFirstPerCam(const CFirstPerCam& rhs);
	~CFirstPerCam() override;

public:
	HRESULT		Ready_GameObject(
		const _vec3& pEye,
		const _vec3& pAt,
		const _vec3& pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual		_int	Update_GameObject(const _float& fDeltaTime) override;
	virtual		void	LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual		void	Render_GameObject() override {};

public:
	static CFirstPerCam* Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3& pEye = { 0, 0, 0 },
										const _vec3& pAt = {0, 0, 1},
										const _vec3& pUp = {0, 1, 0},
										const _float& fFov = D3DXToRadian(60.f),
										const _float& fAspect = (_float)WINCX / WINCY,
										const _float& fNear = 1.f,
										const _float& fFar = 1000.f);

protected:
	virtual		void		Free() override;
};

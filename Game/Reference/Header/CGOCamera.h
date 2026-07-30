#pragma once
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CGOCamera :  public CGameObject
{
protected:
	explicit CGOCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGOCamera(const CGOCamera& rhs);
	virtual ~CGOCamera();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

protected:
	_matrix		m_matView, m_matProj;
	_vec3		m_vEye, m_vAt, m_vUp;
	_float		m_fFov, m_fAspect, m_fNear, m_fFar;

protected:
	virtual void		Free();

};

END
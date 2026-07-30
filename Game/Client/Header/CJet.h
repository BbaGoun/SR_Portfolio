#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CAirplane.h"
#include "CTexture.h"
#include "CCamera.h"

class CJet :
	public CGameObject
{
private:
	explicit CJet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJet(const CGameObject& rhs);
	~CJet() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

private:
	void	Key_Input(const _float& fTimeDelta);

private:
	CAirplane* m_pBufferCom;
	CCamera* m_pCameraCom;

	_float m_fSpeed;

public:
	static CJet* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CThunderFloorEffect : public CGameObject
{
private:
	explicit CThunderFloorEffect(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget);
	explicit CThunderFloorEffect(const CGameObject& rhs);
	~CThunderFloorEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float			m_fTimer;
	float			m_fFrame;
	CGameObject*	m_pTarget;
public:
	static CThunderFloorEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget);

protected:
	virtual		void		Free() override;
};



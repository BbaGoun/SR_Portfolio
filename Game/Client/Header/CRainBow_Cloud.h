#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CCube_Collider.h"

class CRainbow_Cloud : public CGameObject
{
private:
	explicit CRainbow_Cloud(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRainbow_Cloud(const CGameObject& rhs);
	~CRainbow_Cloud() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTextureCom;
	CCube_Collider* m_pColliderCom;

	float	m_fTimer;

public:
	static CRainbow_Cloud* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



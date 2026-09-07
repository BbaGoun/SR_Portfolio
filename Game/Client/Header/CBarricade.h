#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CCube.h"
#include "CTexture.h"
#include "CCube_Collider.h"

class CBarricade : public CGameObject
{
private:
	explicit CBarricade(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBarricade(const CGameObject& rhs);
	~CBarricade() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

	void	Set_OriginPos(_vec3 pos) { m_vOriginPos = pos; }

private:
	CCube*				m_pBufferCom;
	CTexture*			m_pTextureCom;
	CCube_Collider*		m_pColliderCom;

	_vec3				m_vOriginPos;
	float				m_fTimer = 0;
	bool				m_bFirst = true;

public:
	static CBarricade* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



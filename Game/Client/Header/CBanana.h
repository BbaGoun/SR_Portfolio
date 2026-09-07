#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CCube_Collider;
	class CSphere_Collider;
	class CRcTex;
	class CTexture;
}
class CBanana : public CGameObject
{
private:
	explicit CBanana(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBanana(const CGameObject& rhs);
	~CBanana() override;

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
	CRcTex*				m_pBufferCom;
	CTexture*			m_pTextureCom;
	CSphere_Collider*	m_pColliderCom;

public:
	static CBanana* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



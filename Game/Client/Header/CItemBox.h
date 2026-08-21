#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CInnerBox;
	class CTexture;
	class CCube_Collider;
}
class CItemBox : public CGameObject
{
private:
	explicit CItemBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItemBox(const CGameObject& rhs);
	virtual ~CItemBox() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

	bool			GetShow() { return m_bShow; }
	void			SetShow(bool bShow) { m_bShow = bShow; }

public:
	static CItemBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CInnerBox* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCube_Collider* m_pColliderCom;

	float		m_fTimer;
	bool		m_bShow;

protected:
	virtual		void		Free() override;
};


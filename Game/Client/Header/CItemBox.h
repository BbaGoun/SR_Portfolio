#pragma once
#include "CGameObject.h"
#include "CSphere_Collider.h"

namespace Engine
{
	class COuterBox;
	class CTexture;
	class CSphere_Collider;
}
class CItemBox : public CGameObject
{
private:
	explicit CItemBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItemBox(const CGameObject& rhs);
	virtual ~CItemBox() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		PostReady_GameObject() override;

	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

	bool			GetShow() { return m_bShow; }
	void			SetShow(bool bShow) { 
		m_bShow = bShow; 
		Get_Component<CSphere_Collider>()->Set_Active(bShow);
	}

public:
	static CItemBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::COuterBox* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CSphere_Collider* m_pColliderCom;

	_vec3		m_vOrigin;
	float		m_fTimer;
	bool		m_bShow;

protected:
	virtual		void		Free() override;
};


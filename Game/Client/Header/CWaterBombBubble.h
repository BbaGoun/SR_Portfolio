#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CCube_Collider.h"

namespace Engine
{
	class CSphere;
	class CRcTex;
}

class CWaterBombBubble : public CGameObject
{
private:
	explicit CWaterBombBubble(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterBombBubble(const CGameObject& rhs);
	~CWaterBombBubble() override;

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

private:
	CSphere* m_pBufferCom;
	CTexture* m_pTextureCom;
	CRcTex* m_pColliderCom;
	// CRcTex* m_pBufferCom;
	_float	m_fTimer;
	// _float			m_fSpeed;
	// _float			m_fAngle;	// 풍선  회전 이거 말고 -> D3DXToRadian 이걸로?
	_bool	m_bBubbleAppear;

public:
	static CWaterBombBubble* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CQuadrangularPrism;
}
class CInvenSlotCart : public CGameObject
{
private:
	explicit CInvenSlotCart(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID);
	explicit CInvenSlotCart(const CGameObject& rhs);
	virtual ~CInvenSlotCart() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;
public:
	static CInvenSlotCart* Create(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID);
	void						SetTurn(bool bTurn) { m_bTurn = bTurn; }
	
private:
	Engine::CQuadrangularPrism* m_pBufferCom;
	bool						m_bTurn;
	INEN_SLOT_NUM	m_eSlotNum;

protected:
	virtual		void		Free() override;
};


#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CQuadrangularPrism;
}
class CScene3_Char : public CGameObject
{
private:
	explicit CScene3_Char(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID);
	explicit CScene3_Char(const CGameObject& rhs);
	virtual ~CScene3_Char() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;
public:
	static CScene3_Char* Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID);
	void						SetTurn(bool bTurn) { m_bTurn = bTurn; }
	void						Set_Char(CHAR_TYPE pChar) { m_eSlotNum = pChar; }
	bool						Get_Char(bool pChar) { return m_eSlotNum; }

private:
	Engine::CQuadrangularPrism* m_pBufferCom;
	bool						m_bTurn;
	CHAR_TYPE	m_eSlotNum;

protected:
	virtual		void		Free() override;
};


#pragma once
#include "CGameObject.h"
#include "CMenu_Set.h"

namespace Engine
{
	class CRcTex;
}
class CScene3_CharSlot :
	public CGameObject
{
private:
	explicit CScene3_CharSlot(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID);
	explicit CScene3_CharSlot(const CScene3_CharSlot& rhs);
	virtual ~CScene3_CharSlot();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


public:
	void SetChar(CGameObject* pChar) { m_pChar = pChar; }
	void SetBG(CGameObject* pBG) { m_pBG = pBG; }

	void	Set_Slot(CHAR_TYPE eSlot) { m_eSelectSlot = eSlot; }



	void	Set_Show(bool bShow) { m_bShow = bShow; };
	bool	Get_Show() { return m_bShow; }



private:
	Engine::CRcTex* m_pVIBufferCom;



public:
	static CScene3_CharSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID);

	_vec3			m_vPos;
	_vec3			m_vScale;

	bool			m_bShow;


	CGameObject* m_pChar;
	CGameObject* m_pBG;
	
	CHAR_TYPE	m_eSlotNum;
	CHAR_TYPE m_eSelectSlot;

protected:
	virtual		void		Free() override;

};
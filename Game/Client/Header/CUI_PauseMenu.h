#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CRcTex;
	class CTexture;

}
class CUI_PauseMenu :
	public CGameObject
{
private:
	explicit CUI_PauseMenu(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_PauseMenu(const CUI_PauseMenu& rhs);
	virtual ~CUI_PauseMenu();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void SetBtn1(CGameObject* pReplay) { m_pReplay = pReplay; }
	void SetBtn2(CGameObject* pMenu) { m_pMenu = pMenu; }
	void	Set_Show(bool bShow) { m_bShow = bShow; };
	bool	Get_Show() { return m_bShow; }

private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;



public:
	static CUI_PauseMenu* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	bool			m_bShow;

	CGameObject* m_pReplay;
	CGameObject* m_pMenu;
	

protected:
	virtual		void		Free() override;

};
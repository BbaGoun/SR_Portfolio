#pragma once
#include "CGameObject.h"
#include "CCartBot.h"
#include "CCart.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CUI_RankNumber :
	public CGameObject
{
private:
	explicit CUI_RankNumber(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_RankNumber(const CUI_RankNumber& rhs);
	virtual ~CUI_RankNumber();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			Set_Player(CCart* pCart) { m_pCart = pCart; m_pCartBot = nullptr; }
	void			Set_Bot(CCartBot* pCartBot) { m_pCart = nullptr; m_pCartBot = pCartBot; }

private:
	Engine::CRcTex* m_pVIBufferCom;
	Engine::CTexture* m_pTextureCom;

	CCart* m_pCart = nullptr;
	CCartBot* m_pCartBot = nullptr;

	int		m_iRank = 0;

public:
	static CUI_RankNumber* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};
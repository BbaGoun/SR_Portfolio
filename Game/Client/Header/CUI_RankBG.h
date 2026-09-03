#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_RankBG :
	public CGameObject
{
private:
	explicit CUI_RankBG(LPDIRECT3DDEVICE9 pGraphicDev, RANK_ROW_OWNER eOwner, RANK_MARK_COLOR eColor);
	explicit CUI_RankBG(const CGameObject& rhs);
	~CUI_RankBG() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void		SetMarkColor(RANK_MARK_COLOR eColor) { m_eColor = eColor; }
	void		SetRowOwner(RANK_ROW_OWNER eOwner) { m_eOwner = eOwner; }


private:
	CRcTex*				m_pBufferCom;
	CTexture*			m_pTextureCom;

	RANK_ROW_OWNER		m_eOwner;
	RANK_MARK_COLOR		m_eColor;

public:
	static CUI_RankBG* Create(LPDIRECT3DDEVICE9 pGraphicDev, RANK_ROW_OWNER eOwner, RANK_MARK_COLOR eColor);

protected:
	virtual		void		Free() override;
};



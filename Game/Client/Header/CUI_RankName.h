#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_RankName :
	public CGameObject
{
private:
	explicit CUI_RankName(LPDIRECT3DDEVICE9 pGraphicDev, RANK_NAME eName);
	explicit CUI_RankName(const CGameObject& rhs);
	~CUI_RankName() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


	void		SetRankName(RANK_NAME eName) { m_eName = eName; }

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTextureCom;

	RANK_NAME		m_eName;

public:
	static CUI_RankName* Create(LPDIRECT3DDEVICE9 pGraphicDev, RANK_NAME eName);

protected:
	virtual		void		Free() override;
};



#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CUI_InvenSlot.h"

class CScene3_KartBtn :
	public CGameObject
{
private:
	explicit CScene3_KartBtn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene3_KartBtn(const CGameObject& rhs);
	~CScene3_KartBtn() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void						Set_WindowShow(bool bWindowShow) { dynamic_cast<CUI_InvenSlot*>(m_pInvenSlot)->Set_Show(bWindowShow); }
	bool						Get_WindowShow() { return dynamic_cast<CUI_InvenSlot*>(m_pInvenSlot)->Get_Show(); }

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;
public:
	static CScene3_KartBtn* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	CGameObject* m_pInvenSlot;

protected:
	virtual		void		Free() override;
};



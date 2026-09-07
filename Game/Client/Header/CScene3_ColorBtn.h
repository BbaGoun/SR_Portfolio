#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CScene3_ColorSetBG.h"

class CScene3_ColorBtn :
	public CGameObject
{
private:
	explicit CScene3_ColorBtn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene3_ColorBtn(const CGameObject& rhs);
	~CScene3_ColorBtn() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void						Set_WindowShow(bool bWindowShow) { dynamic_cast<CScene3_ColorSetBG*>(m_pColorWindow)->Set_Show(bWindowShow); }
	bool						Get_WindowShow() { return dynamic_cast<CScene3_ColorSetBG*>(m_pColorWindow)->Get_Show(); }


	//virtual			HRESULT		Set_ClickIcon(const _float& fDeltaTime);

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;



public:
	static CScene3_ColorBtn* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
	CGameObject* m_pColorWindow;

protected:
	virtual		void		Free() override;
};



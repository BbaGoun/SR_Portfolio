#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CScene3_CharSlot.h"

class CScene3_CharBtn :
	public CGameObject
{
private:
	explicit CScene3_CharBtn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene3_CharBtn(const CGameObject& rhs);
	~CScene3_CharBtn() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void						Set_WindowShow(bool bWindowShow) { dynamic_cast<CScene3_CharSlot*>(m_pChar)->Set_Show(bWindowShow); }
	bool						Get_WindowShow() { return dynamic_cast<CScene3_CharSlot*>(m_pChar)->Get_Show(); }

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;
public:
	static CScene3_CharBtn* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	CGameObject* m_pChar;


protected:
	virtual		void		Free() override;
};



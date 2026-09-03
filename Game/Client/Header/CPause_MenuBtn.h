#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CPause_MenuBtn:
	public CGameObject
{
private:
	explicit CPause_MenuBtn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPause_MenuBtn(const CPause_MenuBtn& rhs);
	virtual ~CPause_MenuBtn();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;

public:
	void	Set_Show(bool bShow) { m_bShow = bShow; };
	bool	Get_Show() { return m_bShow; }

	bool	m_bShow;


	static CPause_MenuBtn* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



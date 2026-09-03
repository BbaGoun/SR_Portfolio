#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CPause_ReplayBtn :
	public CGameObject
{
private:
	explicit CPause_ReplayBtn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPause_ReplayBtn(const CPause_ReplayBtn& rhs);
	virtual ~CPause_ReplayBtn();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void	Set_Show(bool bShow) { m_bShow = bShow; };
	bool	Get_Show() { return m_bShow; }

	bool	m_bShow;
private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;


public:
	static CPause_ReplayBtn* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



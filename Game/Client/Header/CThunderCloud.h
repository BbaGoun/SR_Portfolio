#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
class CThunderCloud : public CGameObject
{
private:
	explicit CThunderCloud(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CThunderCloud(const CGameObject& rhs);
	~CThunderCloud() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fTimer;
	float	m_fFrame;
	bool	m_bClosed;
	bool	m_bCreateThunder;

	CGameObject* m_pThunder;
	CGameObject* m_pThunderPlayerEffect;
	CGameObject* m_pThunderFloorEffect;

public:
	static CThunderCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	CreateThunder();
	void	CreateThunderPlayerEffect();
	void	CreateThunderFloorEffect();

protected:
	virtual		void		Free() override;
};



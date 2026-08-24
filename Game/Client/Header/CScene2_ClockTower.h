#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CScene2_ClockTower :
	public CGameObject
{
private:
	explicit CScene2_ClockTower(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene2_ClockTower(const CGameObject& rhs);
	~CScene2_ClockTower() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;
public:
	static CScene2_ClockTower* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



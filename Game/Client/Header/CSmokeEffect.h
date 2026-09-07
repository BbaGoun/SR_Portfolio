#pragma once
#include "CGameObject.h"
#include "CSmoke.h"
#include "CTexture.h"

class CSmokeEffect :  public CGameObject
{
private:
	explicit CSmokeEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSmokeEffect(const CGameObject& rhs);
	~CSmokeEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();
	virtual			void		SetCart(CGameObject* pObj);

	void			Set_Twin(bool _b) { m_bTwin = _b; }


private:
	CTexture*		m_pTextureCom;
	CSmoke*			m_pSmoke;

	CGameObject*	m_pCart;
	bool			m_bTwin = true;
	float			m_fSize;
	float			m_iParticleCnt;

public:
	static CSmokeEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, float fSize = 1.f, int iParticleCnt = 30);

protected:
	virtual		void		Free() override;
};


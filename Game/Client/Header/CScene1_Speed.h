#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CScene1_Speed :
	public CGameObject
{
private:
	explicit CScene1_Speed(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene1_Speed(const CScene1_Speed& rhs);
	virtual ~CScene1_Speed();

private:
	virtual			HRESULT		Ready_GameObject() override;
	//virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;

public:
	static CScene1_Speed* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



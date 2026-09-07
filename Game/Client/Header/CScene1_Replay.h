#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CScene1_Replay :
	public CGameObject
{
private:
	explicit CScene1_Replay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene1_Replay(const CScene1_Replay& rhs);
	virtual ~CScene1_Replay();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	//HRESULT		Set_ClickIcon(const _float& fDeltaTime);

private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;

public:
	static CScene1_Replay* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



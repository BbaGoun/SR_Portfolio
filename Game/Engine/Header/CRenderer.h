#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

enum RENDER_GROUP {

};

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void	Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void	Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Clear_RenderGroup();

public:

	void	Ready_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev, float fWidth, float fHeight);
	void	Render_TargetPass(LPDIRECT3DDEVICE9& pGraphicDev);

	void	Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev);
	void	Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);


	LPDIRECT3DTEXTURE9	GetRTTexture()		{ return m_pRTTexture; }

	void OnLostDevice()	{
		Safe_Release(m_pRTTexture);
		Safe_Release(m_pRTSurface);
		Safe_Release(m_pRTDepthStencil);
	}

	void OnResetDevice(LPDIRECT3DDEVICE9& pGraphicDev)	{
		Ready_RenderTarget(pGraphicDev, m_fHeight, m_fHeight);
	}

private:
	void	PreCull(LPDIRECT3DDEVICE9& pGraphicDev);
	void	PreRender(LPDIRECT3DDEVICE9& pGraphicDev);
	void	PostRender(LPDIRECT3DDEVICE9& pGraphicDev);

	void	PreParticleRender(LPDIRECT3DDEVICE9& pGraphicDev);
	void	PostParticleRender(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>	m_RenderGroup[RENDER_END];

	LPDIRECT3DTEXTURE9      m_pRTTexture = nullptr;
	LPDIRECT3DSURFACE9      m_pRTSurface = nullptr;
	LPDIRECT3DSURFACE9      m_pRTDepthStencil = nullptr;

	float					m_fWidth;
	float					m_fHeight;

private:
	virtual void	Free();

};

END
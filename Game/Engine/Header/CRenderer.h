#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Delete_RenderGroup(CGameObject* pObj);
	void		Clear_RenderGroup();

	void		Add_LeftMirrorRenderGroup(RENDERID eID, CGameObject* pGameObject);
	void		RenderLeftSideMirrorGroup(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Delete_LeftMirrorRenderGroup(CGameObject* pObj);
	void		Clear_LeftMirrorRenderGroup();

public:

	HRESULT		Add_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev, const _tchar* pName,float fWidth, float fHeight);
	void		Add_RenderTargetGroup(const _tchar* pName, CGameObject* pGameObject);
	RTINFO*		Find_RenderTarget(const _tchar* pName);
	void		Delete_RenderTarget(const _tchar* pName);

	HRESULT		Ready_BlurRT(LPDIRECT3DDEVICE9& pGraphicDev);
	void		RenderBlur(LPDIRECT3DDEVICE9& pGraphicDev);
	void		DrawFullScreen(LPDIRECT3DDEVICE9& pGraphicDev, LPDIRECT3DTEXTURE9 pTexture, _byte byAlpha);
	void		Delete_BlurRT();


	void		Render_TargetPass(LPDIRECT3DDEVICE9& pGraphicDev);

	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Skid(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Trail(LPDIRECT3DDEVICE9& pGraphicDev);

	void		Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Fog(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlphaUI(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_AlphaUI(LPDIRECT3DDEVICE9& pGraphicDev);
//LeftMirrorRender
	void		LeftMirrorRender_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorRender_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorRender_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorRender_Skid(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorRender_Trail(LPDIRECT3DDEVICE9& pGraphicDev);

	void		LeftMirrorRender_Particle(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorRender_NonAlphaUI(LPDIRECT3DDEVICE9& pGraphicDev);

	void		SetBlur(bool bBlur);
	void		SetBlurPower(float fBlurPower)	{ m_fBlurPower = fBlurPower; }


	void OnLostDevice();
	void OnResetDevice(LPDIRECT3DDEVICE9& pGraphicDev);


private:
	void		PreCull(LPDIRECT3DDEVICE9& pGraphicDev);
	void		DistanceCulling(LPDIRECT3DDEVICE9& pGraphicDev);
	void		FrustumCulling(LPDIRECT3DDEVICE9& pGraphicDev);


	void		LeftMirrorPreCull(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorDistanceCulling(LPDIRECT3DDEVICE9& pGraphicDev);
	void		LeftMirrorFrustumCulling(LPDIRECT3DDEVICE9& pGraphicDev);


	void		PreRender(LPDIRECT3DDEVICE9& pGraphicDev);
	void		PostRender(LPDIRECT3DDEVICE9& pGraphicDev);

	HRESULT		Ready_BlurA(LPDIRECT3DDEVICE9& pGraphicDev);
	HRESULT		Ready_BlurB(LPDIRECT3DDEVICE9& pGraphicDev);

	bool		GetLeftMirrorRender() { return m_bRenderLeftMirror; }
	void		SetLeftMirrorRender(bool bRenderLeftMirror) { m_bRenderLeftMirror = bRenderLeftMirror; }

private:
	list<CGameObject*>		m_RenderGroup[RENDER_END];
	list<CGameObject*>		m_LeftMirrorRenderGroup[RENDER_END];

	map<const _tchar*, RTINFO*> m_mapRenderTarget;

	RTINFO*					m_pBlurA;
	RTINFO*					m_pBlurB;

	bool					m_bBlur;
	float					m_fBlurPower;

	bool					m_bRenderLeftMirror = true;

private:
	virtual void	Free();

};

END
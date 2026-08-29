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

public:

	HRESULT		Add_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev, const _tchar* pName,float fWidth, float fHeight);
	void		Add_RenderTargetGroup(const _tchar* pName, CGameObject* pGameObject);

	RTINFO*		Find_RenderTarget(const _tchar* pName);
	void		Delete_RenderTarget(const _tchar* pName);
	void		Ready_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev, float fWidth, float fHeight);
	
	void		Render_TargetPass(LPDIRECT3DDEVICE9& pGraphicDev);

	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_NonAlphaUI(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_AlphaUI(LPDIRECT3DDEVICE9& pGraphicDev);


	LPDIRECT3DTEXTURE9	GetRTTexture()		{ return m_pRTTexture; }

	void OnLostDevice();
	void OnResetDevice(LPDIRECT3DDEVICE9& pGraphicDev);


private:
	void	PreCull(LPDIRECT3DDEVICE9& pGraphicDev);
	void	DistanceCulling(LPDIRECT3DDEVICE9& pGraphicDev);
	void	FrustumCulling(LPDIRECT3DDEVICE9& pGraphicDev);

	void	PreRender(LPDIRECT3DDEVICE9& pGraphicDev);
	void	PostRender(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>		m_RenderGroup[RENDER_END];

	// RenderTarget 텍스처 생성시 필요한 변수 -> struct에 포함시키고 map컨테이너로 관리
	LPDIRECT3DTEXTURE9      m_pRTTexture = nullptr;
	LPDIRECT3DSURFACE9      m_pRTSurface = nullptr;
	LPDIRECT3DSURFACE9      m_pRTDepthStencil = nullptr;

	float					m_fWidth;
	float					m_fHeight;

	map<const _tchar*, RTINFO*> m_mapRenderTarget;

private:
	virtual void	Free();

};

END
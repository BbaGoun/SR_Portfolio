#include "CRenderer.h"
#include "CCameraMgr.h"
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer():m_fWidth(0), m_fHeight(0)
{
	
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	if (RENDER_END <= eID || nullptr == pGameObject)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	//Render_TargetPass(pGraphicDev);

	PreCull(pGraphicDev);
	PreRender(pGraphicDev);

	Render_Priority(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	PostRender(pGraphicDev);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Ready_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev, float fWidth ,float fHeight)
{
	// 1. ÅØ½ºÃ³ »ý¼º
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	pGraphicDev->CreateTexture(
		fWidth, fHeight,		// ¹Ì´Ï¸Ê ÇØ»óµµ
		1,						// ¹Ó¸Ê ·¹º§ (·»´õÅ¸°ÙÀº º¸Åë 1)
		D3DUSAGE_RENDERTARGET,  
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,		// ·»´õÅ¸°ÙÀº ¹Ýµå½Ã DEFAULT Ç®
		&m_pRTTexture,
		nullptr
	);

	// 2. Surface »Ì¾Æ³»±â
	m_pRTTexture->GetSurfaceLevel(0, &m_pRTSurface);

	// 3. ½ºÅÙ½Ç ¹öÆÛ ÁØºñ
	pGraphicDev->CreateDepthStencilSurface(
		fWidth, fHeight,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pRTDepthStencil,
		nullptr
	);
}

void CRenderer::Render_TargetPass(LPDIRECT3DDEVICE9& pGraphicDev)
{
	
	IDirect3DSurface9* pOldRT = nullptr;
	IDirect3DSurface9* pOldDS = nullptr;
	pGraphicDev->GetRenderTarget(0, &pOldRT);
	pGraphicDev->GetDepthStencilSurface(&pOldDS);

	pGraphicDev->SetRenderTarget(0, m_pRTSurface);          
	pGraphicDev->SetDepthStencilSurface(m_pRTDepthStencil); 

	pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0, 255, 255, 255), 1.0f, 0);

	pGraphicDev->BeginScene();

	// Ä«¸Þ¶ó ¼³Á¤
	if (CCameraMgr::GetInstance()->GetCamerState() != CAMERA_END) {
		CameraInfo camInfo = CCameraMgr::GetInstance()->GetCameraInfo();
		pGraphicDev->SetTransform(D3DTS_VIEW, &camInfo.matView);
		pGraphicDev->SetTransform(D3DTS_PROJECTION, &camInfo.matProj);
	}

	for (auto& pObj : m_RenderGroup[RENDER_TARGET])
		pObj->Render_GameObject();

	pGraphicDev->SetRenderTarget(0, pOldRT);
	pGraphicDev->SetDepthStencilSurface(pOldDS);
	pOldRT->Release();
	pOldDS->Release();
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pObj : m_RenderGroup[RENDER_PRIORITY])
		pObj->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pObj : m_RenderGroup[RENDER_NONALPHA])
		pObj->Render_GameObject();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	m_RenderGroup[RENDER_ALPHA].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});
	for (auto& pObj : m_RenderGroup[RENDER_ALPHA])
		pObj->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (CCameraMgr::GetInstance()->GetMainCamera())
	{
		_matrix matView, matProj;
		D3DXMatrixIdentity(&matView);
		pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixOrthoLH(&matProj, (float)WINCX, (float)WINCY, 1.f, 1000.f);

		pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_RenderGroup[RENDER_UI].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});

	for (auto& pObj : m_RenderGroup[RENDER_UI])
		pObj->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::PreCull(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for (auto& pObj : m_RenderGroup[i])
			pObj->PreCull_GameObject();
	}
}

void CRenderer::PreRender(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for (auto& pObj : m_RenderGroup[i])
			pObj->PreRender_GameObject();
	}
}

void CRenderer::PostRender(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for (auto& pObj : m_RenderGroup[i])
			pObj->PostRender_GameObject();
	}
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}

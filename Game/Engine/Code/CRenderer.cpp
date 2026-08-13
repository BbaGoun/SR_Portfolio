#include "CRenderer.h"
#include "CCameraMgr.h"
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
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
		D3DVIEWPORT9 newViewPort;
		
		_matrix matView, matProj;
		D3DXMatrixIdentity(&matView);
		pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixOrthoLH(&matProj, (float)WINCX, (float)WINCY, 1.f, 1000.f);

		pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (auto& pObj : m_RenderGroup[RENDER_UI])
		pObj->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
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

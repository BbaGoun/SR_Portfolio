#include "CRenderer.h"
#include "CCameraMgr.h"
#include "CVIBuffer.h"
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
	m_pBlurA = nullptr;
	m_pBlurB = nullptr;
	m_bBlur = false;
	m_fBlurPower = 0.7f;
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
	if (m_bBlur == false || m_pBlurA == nullptr || m_pBlurB == nullptr)
	{
		Render_TargetPass(pGraphicDev);

		PreCull(pGraphicDev);
		PreRender(pGraphicDev);

		Render_Priority(pGraphicDev);
		Render_Fog(pGraphicDev);
		Render_NonAlpha(pGraphicDev);

		Render_Alpha(pGraphicDev);
		Render_Skid(pGraphicDev);
		Render_Trail(pGraphicDev);
		Render_Particle(pGraphicDev);
		Render_NonAlphaUI(pGraphicDev);
		Render_AlphaUI(pGraphicDev);

		PostRender(pGraphicDev);

	}
	else
	{
		RenderBlur(pGraphicDev);
	}
	Clear_RenderGroup();
}

void CRenderer::Delete_RenderGroup(CGameObject* pObj)
{
	for (int i = 0; i < RENDER_END; ++i) {
		auto it = find(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), pObj);

		if (it != m_RenderGroup[i].end()) {
			Safe_Release((*it));
			m_RenderGroup[i].erase(it);
			return;
		}
	}
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}

	for (auto& pair : m_mapRenderTarget)
	{
		for_each(pair.second->RenderList.begin(), pair.second->RenderList.end(), CDeleteObj());
		pair.second->RenderList.clear();
	}
}

HRESULT CRenderer::Add_RenderTarget(LPDIRECT3DDEVICE9& pGraphicDev, const _tchar* pName, float fWidth, float fHeight)
{
	for (auto& pair : m_mapRenderTarget)
	{
		if (wcscmp(pair.first, pName) == 0)
		{
			MSG_BOX("중복된 RT이름");
			return E_FAIL;
		}
	}

	RenderTargetInfo* pInfo = new RenderTargetInfo;
	pInfo->fWidth = fWidth;
	pInfo->fHeight = fHeight;


	if (FAILED(pGraphicDev->CreateTexture(
		fWidth, fHeight, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pInfo->pRTTexture,
		nullptr)))
	{
		delete pInfo;
		return E_FAIL;
	}

	pInfo->pRTTexture->GetSurfaceLevel(0, &pInfo->pRTSurface);

	if (FAILED(pGraphicDev->CreateDepthStencilSurface(
		fWidth, fHeight,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&pInfo->pRTDepthStencil,
		nullptr)))
	{
		Safe_Release(pInfo->pRTTexture);
		Safe_Release(pInfo->pRTSurface);
		delete pInfo;
		return E_FAIL;
	}

	m_mapRenderTarget.insert({ pName, pInfo });
	return S_OK;
}

void CRenderer::Add_RenderTargetGroup(const _tchar* pName, CGameObject* pGameObject)
{
	RTINFO* pInfo = Find_RenderTarget(pName);
	if (nullptr == pInfo || nullptr == pGameObject)
		return;

	pInfo->RenderList.push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_TargetPass(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pair : m_mapRenderTarget)
	{
		IDirect3DSurface9* pOldRT = nullptr;
		IDirect3DSurface9* pOldDS = nullptr;
		pGraphicDev->GetRenderTarget(0, &pOldRT);
		pGraphicDev->GetDepthStencilSurface(&pOldDS);

		RTINFO* pInfo = pair.second;

		pGraphicDev->SetRenderTarget(0, pInfo->pRTSurface);
		pGraphicDev->SetDepthStencilSurface(pInfo->pRTDepthStencil);

		pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0, 255, 255, 255), 1.0f, 0);


		//if (CCameraMgr::GetInstance()->GetCamerState() != CAMERA_END) {
		//	CameraInfo camInfo = CCameraMgr::GetInstance()->GetCameraInfo();
		//	pGraphicDev->SetTransform(D3DTS_VIEW, &camInfo.matView);
		//	pGraphicDev->SetTransform(D3DTS_PROJECTION, &camInfo.matProj);
		//}

		//if(wcsncmp(pair.first, L"InvenSlot", 9) == 0)
		//	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		pInfo->RenderList.sort([](CGameObject* pDst, CGameObject* pSrc)->bool
			{
				_vec3 vDst, vSrc;
				pDst->Get_Transform()->Get_Info(INFO_POS, &vDst);
				pSrc->Get_Transform()->Get_Info(INFO_POS, &vSrc);
				return vDst.z > vSrc.z;
			});

		for (auto& pObj : pInfo->RenderList)
			pObj->Render_GameObject();

		pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		pGraphicDev->SetRenderTarget(0, pOldRT);
		pGraphicDev->SetDepthStencilSurface(pOldDS);
		Safe_Release(pOldRT);
		Safe_Release(pOldDS);
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pObj : m_RenderGroup[RENDER_PRIORITY])
		pObj->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 254);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObj : m_RenderGroup[RENDER_NONALPHA])
		pObj->Render_GameObject();

	pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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

	for (auto& pObj : m_RenderGroup[RENDER_ALPHA])
	{
		pObj->Compute_ViewZ();
	}

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

void CRenderer::Render_Skid(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	for (auto& pObj : m_RenderGroup[RENDER_SKID])
	{
		pObj->Compute_ViewZ();
	}

	m_RenderGroup[RENDER_SKID].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});
	for (auto& pObj : m_RenderGroup[RENDER_SKID])
		pObj->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_Trail(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	for (auto& pObj : m_RenderGroup[RENDER_TRAIL])
	{
		pObj->Compute_ViewZ();
	}

	m_RenderGroup[RENDER_TRAIL].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});
	for (auto& pObj : m_RenderGroup[RENDER_TRAIL])
		pObj->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_Particle(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& pObj : m_RenderGroup[RENDER_PARTICLE])
		pObj->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_Fog(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, 0xFFB0C4DE); // ARGB 중 RGB만 사용

	// 2. 픽셀 안개 모드를 선형(Linear) 공식으로 설정
	pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// 3. 안개의 시작 위치와 끝 위치 설정
	float fFogStart = 15.0f;
	float fFogEnd = 300.0f;

	pGraphicDev->SetRenderState(D3DRS_FOGSTART, *((DWORD*)(&fFogStart)));
	pGraphicDev->SetRenderState(D3DRS_FOGEND, *((DWORD*)(&fFogEnd)));

	//pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void CRenderer::Render_NonAlphaUI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (CCameraMgr::GetInstance()->GetMainCamera())
	{
		_matrix matView, matProj;
		D3DXMatrixIdentity(&matView);
		pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixOrthoLH(&matProj, (float)WINCX, (float)WINCY, 1.f, 1000.f);

		pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	}
	//pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 254);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (auto& pObj : m_RenderGroup[RENDER_NONALPHAUI])
		pObj->Render_GameObject();

	pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CRenderer::Render_AlphaUI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHAUI].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			// z값이 먼 것 부터 그리겠다. 
			// 알파블랜딩은 뒤에 있는 것이 이미 있다고 생각하고 그리는것이기 때문
			// 다만 UI는 Render하는 시점에 카메라가 바뀌기 때문에 그냥 Transform의 z좌표 가져와서 정렬하는게 낫다
			_vec3 vDstPos, vSrcPos;
			pDst->Get_Transform()->Get_Info(INFO_POS, &vDstPos);
			pSrc->Get_Transform()->Get_Info(INFO_POS, &vSrcPos);
			return vDstPos.z > vSrcPos.z;
		});

	for (auto& pObj : m_RenderGroup[RENDER_ALPHAUI])
		pObj->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

RTINFO* CRenderer::Find_RenderTarget(const _tchar* pName)
{
	for (auto& pair : m_mapRenderTarget)
	{
		if (wcscmp(pair.first, pName) == 0)
			return pair.second;
	}
	return nullptr;
}

void CRenderer::Delete_RenderTarget(const _tchar* pName)
{
	for (auto iter = m_mapRenderTarget.begin(); iter != m_mapRenderTarget.end(); ++iter)
	{
		if (wcscmp(iter->first, pName) == 0)
		{
			Safe_Release(iter->second->pRTTexture);
			Safe_Release(iter->second->pRTSurface);
			Safe_Release(iter->second->pRTDepthStencil);
			delete iter->second;

			m_mapRenderTarget.erase(iter);
			return;
		}
	}
}

void CRenderer::Delete_BlurRT()
{
	if (m_pBlurA)
	{
		Safe_Release(m_pBlurA->pRTTexture);
		Safe_Release(m_pBlurA->pRTSurface);
		Safe_Release(m_pBlurA->pRTDepthStencil);
		delete m_pBlurA;
	}
	if (m_pBlurB)
	{
		Safe_Release(m_pBlurB->pRTTexture);
		Safe_Release(m_pBlurB->pRTSurface);
		Safe_Release(m_pBlurB->pRTDepthStencil);
		delete m_pBlurB;
	}
}

HRESULT CRenderer::Ready_BlurRT(LPDIRECT3DDEVICE9& pGraphicDev)
{
	if (FAILED(Ready_BlurA(pGraphicDev)))
		return E_FAIL;
	if (FAILED(Ready_BlurB(pGraphicDev)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Ready_BlurA(LPDIRECT3DDEVICE9& pGraphicDev)
{
	m_pBlurA = new RenderTargetInfo;
	m_pBlurA->fWidth = WINCX;
	m_pBlurA->fHeight = WINCY;

	if (FAILED(pGraphicDev->CreateTexture(
		m_pBlurA->fWidth, m_pBlurA->fHeight, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBlurA->pRTTexture,
		nullptr)))
	{
		delete m_pBlurA;
		return E_FAIL;
	}

	m_pBlurA->pRTTexture->GetSurfaceLevel(0, &m_pBlurA->pRTSurface);

	if (FAILED(pGraphicDev->CreateDepthStencilSurface(
		m_pBlurA->fWidth, m_pBlurA->fHeight,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pBlurA->pRTDepthStencil,
		nullptr)))
	{
		Safe_Release(m_pBlurA->pRTTexture);
		Safe_Release(m_pBlurA->pRTSurface);
		delete m_pBlurA;
		return E_FAIL;
	}
}

HRESULT CRenderer::Ready_BlurB(LPDIRECT3DDEVICE9& pGraphicDev)
{
	m_pBlurB = new RenderTargetInfo;
	m_pBlurB->fWidth = WINCX;
	m_pBlurB->fHeight = WINCY;

	if (FAILED(pGraphicDev->CreateTexture(
		m_pBlurB->fWidth, m_pBlurB->fHeight, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBlurB->pRTTexture,
		nullptr)))
	{
		delete m_pBlurB;
		return E_FAIL;
	}

	m_pBlurB->pRTTexture->GetSurfaceLevel(0, &m_pBlurB->pRTSurface);

	if (FAILED(pGraphicDev->CreateDepthStencilSurface(
		m_pBlurB->fWidth, m_pBlurB->fHeight,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pBlurB->pRTDepthStencil,
		nullptr)))
	{
		Safe_Release(m_pBlurB->pRTTexture);
		Safe_Release(m_pBlurB->pRTSurface);
		delete m_pBlurB;
		return E_FAIL;
	}
}

void CRenderer::RenderBlur(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_TargetPass(pGraphicDev);

	PreCull(pGraphicDev);
	PreRender(pGraphicDev);

	// 1. 이전 RT 저장
	IDirect3DSurface9* pOldRT = nullptr;
	IDirect3DSurface9* pOldDS = nullptr;
	pGraphicDev->GetRenderTarget(0, &pOldRT);
	pGraphicDev->GetDepthStencilSurface(&pOldDS);

	// 2. 현재 씬을 그릴 RT(m_BlurB) 설정 
	pGraphicDev->SetRenderTarget(0, m_pBlurB->pRTSurface);
	pGraphicDev->SetDepthStencilSurface(m_pBlurB->pRTDepthStencil);

	pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	// 3. 현재 씬 그리기 (UI전까지)
	Render_Priority(pGraphicDev);
	Render_Fog(pGraphicDev);
	Render_NonAlpha(pGraphicDev);

	Render_Alpha(pGraphicDev);
	Render_Skid(pGraphicDev);
	Render_Trail(pGraphicDev);

	// 4. 이전 프레임RT(m_BlurA) 덮어쓰기
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DrawFullScreen(pGraphicDev, m_pBlurA->pRTTexture, (_byte)(m_fBlurPower * 255.f));

	// 5. 표면 복사
	pGraphicDev->StretchRect(m_pBlurB->pRTSurface, nullptr, pOldRT, nullptr, D3DTEXF_NONE);

	//DrawFullScreen(pGraphicDev, m_pBlurB->pRTTexture, (_byte)(1.f * 255.f));

	// 6. RT 포인터 스왑
	swap(m_pBlurA, m_pBlurB);

	// 7. OldRT(백버퍼)로 원상복구 
	pGraphicDev->SetRenderTarget(0, pOldRT);
	pGraphicDev->SetDepthStencilSurface(pOldDS);
	Safe_Release(pOldRT);
	Safe_Release(pOldDS);

	// 8. 파티클 + UI 그리기
	Render_Particle(pGraphicDev);
	Render_NonAlphaUI(pGraphicDev);
	Render_AlphaUI(pGraphicDev);

	PostRender(pGraphicDev);
}

void CRenderer::DrawFullScreen(LPDIRECT3DDEVICE9& pGraphicDev, LPDIRECT3DTEXTURE9 pTexture, _byte byAlpha)
{
	if (pTexture == nullptr || byAlpha == 0)
		return;

	VTXSCREEN buffer[5];

	_D3DVIEWPORT9 vp;
	pGraphicDev->GetViewport(&vp);
	float fWidth = vp.Width;
	float fHeight = vp.Height;

	D3DXCOLOR dwColor = D3DCOLOR_ARGB(byAlpha, 255, 255, 255);
	buffer[0] = { {0.f				,0.f			,0,1},	dwColor,		{0,0} };
	buffer[1] = { {fWidth			,0.f			,0,1},	dwColor,		{1,0} };
	buffer[2] = { {fWidth			,fHeight		,0,1},	dwColor,		{1,1} };
	buffer[3] = { {0.f				,fHeight		,0,1},	dwColor,		{0,1} };

	dwColor = D3DCOLOR_ARGB(0, 255, 255, 255);
	buffer[4] = { {fWidth * 0.5f	,fHeight * 0.5f	,0,1},	dwColor,		{0.5f,0.5f} };

	INDEX32  indices[4];

	indices[0]._0 = 0;
	indices[0]._1 = 1;
	indices[0]._2 = 4;

	indices[1]._0 = 4;
	indices[1]._1 = 1;
	indices[1]._2 = 2;

	indices[2]._0 = 3;
	indices[2]._1 = 4;
	indices[2]._2 = 2;

	indices[3]._0 = 3;
	indices[3]._1 = 0;
	indices[3]._2 = 4;

	// 텍스처 설정
	pGraphicDev->SetTexture(0, pTexture);
	// 텍스처의 알파값이 아닌 정점의 알파값으로 계산해라

	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	// z쓰기 끄기
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	// 안개 끄기
	pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// FVF설정 및 그리기
	pGraphicDev->SetFVF(FVF_SCREEN);

	pGraphicDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 5, 4,
		indices, D3DFMT_INDEX32, buffer, sizeof(VTXSCREEN));

	// 복원
		// 텍스처 끄기
	pGraphicDev->SetTexture(0, nullptr);
	// 정점의 알파값이 아닌 텍스처의 알파값으로 계산해라
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// z쓰기 켜기
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::PreCull(LPDIRECT3DDEVICE9& pGraphicDev)
{
	DistanceCulling(pGraphicDev);
	FrustumCulling(pGraphicDev);
}

void CRenderer::DistanceCulling(LPDIRECT3DDEVICE9& pGraphicDev)
{
	_vec3 vCamPos;
	_matrix matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	memcpy(&vCamPos, &matView.m[3], sizeof(_vec3));
	vCamPos *= -1;

	float fCullDistance, dist;
	_vec3 vObjPos;
	for (size_t i = 0; i < RENDER_PARTICLE; ++i)
	{
		for (auto it = m_RenderGroup[i].begin(); it != m_RenderGroup[i].end();) {
			if (*it == nullptr) {
				Safe_Release((*it));
				it = m_RenderGroup[i].erase(it);
				continue;
			}

			if (!(*it)->Get_CullEnable()) {
				++it;
				continue;
			}

			fCullDistance = (*it)->Get_CullDistance();
			if (fCullDistance == 0) {
				++it;
				continue;
			}

			(*it)->Get_Transform()->Get_Info(INFO_POS, &vObjPos);
			_vec3 dir = vObjPos - vCamPos;
			dist = D3DXVec3Length(&dir);
			if (dist >= fCullDistance) {
				Safe_Release((*it));
				it = m_RenderGroup[i].erase(it);
			}
			else
				++it;
		}
	}
}

void CRenderer::FrustumCulling(LPDIRECT3DDEVICE9& pGraphicDev)
{
	_matrix matView, matInvView;
	_matrix matProj;
	_matrix matObjWorld;

	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	float m[16];
	memcpy(m, &matProj.m, sizeof(float) * 16);
	DirectX::XMMATRIX xmMatProj(m);

	DirectX::BoundingFrustum tFrustum;
	DirectX::BoundingFrustum::CreateFromMatrix(tFrustum, xmMatProj);

	memcpy(m, &matInvView.m, sizeof(float) * 16);
	DirectX::XMMATRIX xmMatInvView(m);

	tFrustum.Transform(tFrustum, xmMatInvView);

	DirectX::BoundingBox box;
	for (size_t i = 0; i < RENDER_PARTICLE; ++i)
	{
		for (auto it = m_RenderGroup[i].begin(); it != m_RenderGroup[i].end();) {
			if (*it == nullptr) {
				Safe_Release((*it));
				it = m_RenderGroup[i].erase(it);
				continue;
			}

			if (!(*it)->Get_CullEnable()) {
				++it;
				continue;
			}

			CVIBuffer* pBuf = (*it)->Get_Component<CVIBuffer>();
			if (pBuf == nullptr) {
				Safe_Release((*it));
				it = m_RenderGroup[i].erase(it);
				continue;
			}

			matObjWorld = *(*it)->Get_Transform()->Get_World();
			memcpy(m, &matObjWorld.m, sizeof(float) * 16);
			DirectX::XMMATRIX xmMatWorld(m);

			box = *pBuf->GetBoundingBox();
			box.Transform(box, xmMatWorld);

			if (!tFrustum.Intersects(box)) {
				Safe_Release((*it));
				it = m_RenderGroup[i].erase(it);
				continue;
			}
			++it;
		}
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

void CRenderer::SetBlur(bool bBlur)
{
	m_bBlur = bBlur;
	//if (m_bBlur == false)
	//	m_fBlurPower = 0.7f; // 투명도 초기화
}
void CRenderer::OnLostDevice()
{
	for (auto& pair : m_mapRenderTarget)
	{
		RTINFO* pInfo = pair.second;
		Safe_Release(pInfo->pRTTexture);
		Safe_Release(pInfo->pRTSurface);
		Safe_Release(pInfo->pRTDepthStencil);
	}
	Safe_Release(m_pBlurB->pRTTexture);
	Safe_Release(m_pBlurB->pRTSurface);
	Safe_Release(m_pBlurB->pRTDepthStencil);

	Safe_Release(m_pBlurA->pRTTexture);
	Safe_Release(m_pBlurA->pRTSurface);
	Safe_Release(m_pBlurA->pRTDepthStencil);
}
void CRenderer::OnResetDevice(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pair : m_mapRenderTarget)
	{
		RTINFO* pInfo = pair.second;

		pGraphicDev->CreateTexture(
			pInfo->fWidth, pInfo->fHeight, 1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&pInfo->pRTTexture,
			nullptr);

		pInfo->pRTTexture->GetSurfaceLevel(0, &pInfo->pRTSurface);

		pGraphicDev->CreateDepthStencilSurface(
			pInfo->fWidth, pInfo->fHeight,
			D3DFMT_D24S8,
			D3DMULTISAMPLE_NONE, 0,
			TRUE,
			&pInfo->pRTDepthStencil,
			nullptr);
	}
	pGraphicDev->CreateTexture(
		m_pBlurB->fWidth, m_pBlurB->fHeight, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBlurB->pRTTexture,
		nullptr);

	m_pBlurB->pRTTexture->GetSurfaceLevel(0, &m_pBlurB->pRTSurface);

	pGraphicDev->CreateDepthStencilSurface(
		m_pBlurB->fWidth, m_pBlurB->fHeight,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pBlurB->pRTDepthStencil,
		nullptr);


	pGraphicDev->CreateTexture(
		m_pBlurA->fWidth, m_pBlurA->fHeight, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBlurA->pRTTexture,
		nullptr);

	m_pBlurA->pRTTexture->GetSurfaceLevel(0, &m_pBlurA->pRTSurface);

	pGraphicDev->CreateDepthStencilSurface(
		m_pBlurA->fWidth, m_pBlurA->fHeight,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pBlurA->pRTDepthStencil,
		nullptr);
}


void CRenderer::Free()
{
	Clear_RenderGroup();
}

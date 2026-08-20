#include "CSpline.h"

CSpline::CSpline(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
	m_eKind = CK_MESH;
}

CSpline::CSpline(const CSpline& rhs)
	:CVIBuffer(rhs)
{
	m_eKind = CK_MESH;
}

CSpline::~CSpline()
{
}


HRESULT CSpline::Ready_CSplineCom()
{
	
	return S_OK;
}

HRESULT CSpline::Ready_Buffer()
{
	return S_OK;
}

void CSpline::Render_Buffer()
{
	if (m_dwVtxCnt < 3)
		return;
	CVIBuffer::Render_Buffer();
}


_int CSpline::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void CSpline::LateUpdate_Component()
{

}

CSpline* CSpline::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpline* pCom = new CSpline(pGraphicDev);

	if (FAILED(pCom->Ready_CSplineCom()))
	{
		Safe_Release(pCom);
		MSG_BOX("CSpline Create Failed");
		return nullptr;
	}

	return pCom;
}


void CSpline::Free()
{
	CVIBuffer::Free();
}

CComponent* CSpline::Clone()
{
	CSpline* pCom = new CSpline(*this);

	if (FAILED(pCom->Ready_CSplineCom()))
	{
		Safe_Release(pCom);
		MSG_BOX("CSpline Create Failed");
		return nullptr;
	}

	return pCom;
}

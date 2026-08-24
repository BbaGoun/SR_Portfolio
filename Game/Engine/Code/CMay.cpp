#include "CMay.h"

CMay::CMay() : CPskFile()
{
}

CMay::CMay(LPDIRECT3DDEVICE9 pGraphicDev) : CPskFile(pGraphicDev)
{
}

CMay::CMay(const CMay& rhs) : CPskFile(rhs)
{
}

CMay::~CMay()
{
}

HRESULT CMay::Ready_Buffer()
{
	if (FAILED(LoadPsk(L"../Bin/Resource/It_Takes_Two/Character/May/May.pskx")))
		return E_FAIL;

	return S_OK;
}

void CMay::Render_Buffer()
{
	CPskFile::Render_Buffer();
}

CMay* CMay::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMay* pMay = new CMay(pGraphicDev);

	if (FAILED(pMay->Ready_Buffer()))
	{
		Safe_Release(pMay);
		MSG_BOX("CMay Create Failed");
		return nullptr;
	}

	return pMay;
}

CComponent* CMay::Clone()
{
	return new CMay(*this);
}

void CMay::Free()
{
	CPskFile::Free();
}

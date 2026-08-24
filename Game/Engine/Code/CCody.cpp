#include "CCody.h"

CCody::CCody() : CPskFile()
{
}

CCody::CCody(LPDIRECT3DDEVICE9 pGraphicDev) : CPskFile(pGraphicDev)
{
}

CCody::CCody(const CCody& rhs) : CPskFile(rhs)
{
}

CCody::~CCody()
{
}

HRESULT CCody::Ready_Buffer()
{
	if (FAILED(LoadPsk(L"../Bin/Resource/It_Takes_Two/Character/Cody/Cody.psk")))
		return E_FAIL;

	return S_OK;
}

void CCody::Render_Buffer()
{
	CPskFile::Render_Buffer();
}

CCody* CCody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCody* pCody = new CCody(pGraphicDev);

	if (FAILED(pCody->Ready_Buffer()))
	{
		Safe_Release(pCody);
		MSG_BOX("CCody Create Failed");
		return nullptr;
	}

	return pCody;
}

CComponent* CCody::Clone()
{
	return new CCody(*this);
}

void CCody::Free()
{
	CPskFile::Free();
}

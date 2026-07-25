#include "CAirplane.h"

CAirplane::CAirplane() : CObj()
{
}

CAirplane::CAirplane(LPDIRECT3DDEVICE9 pGraphicDev) : CObj(pGraphicDev)
{
}

CAirplane::CAirplane(const CAirplane& rhs) : CObj(rhs)
{
}

CAirplane::~CAirplane()
{
}

HRESULT CAirplane::Ready_Obj()
{
	if (FAILED(LoadObj(L"../Bin/Resource/Plane01/Plane01.obj", {0, -90, 0})))
		return E_FAIL;

	return S_OK;
}

void CAirplane::Render_Obj()
{
	CObj::Render_Obj();
}

CAirplane* CAirplane::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAirplane* pAirplane = new CAirplane(pGraphicDev);

	if (FAILED(pAirplane->Ready_Obj()))
	{
		Safe_Release(pAirplane);
		MSG_BOX("Airplane Create Failed");
		return nullptr;
	}

	return pAirplane;
}

CComponent* CAirplane::Clone()
{
	return new CAirplane(*this);
}

void CAirplane::Free()
{
	CObj::Free();
}

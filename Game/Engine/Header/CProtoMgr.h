#pragma once
#include "CComponent.h"
#include "CBase.h"

#include "CQuadrangularPrism.h"
#include "CTriangularPrism.h"

#include "CCube_Collider.h"
#include "CSphere_Collider.h"

#include "CCartBodyCol.h"
#include "CCartWheelCol.h"
#include "CRcTex.h"
#include "CPyramid.h"
#include "CInnerBox.h"
#include "CAirplane.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CTexture.h"
#include "CTerrain.h"
#include "CTerrain2.h"
#include "CTerrain3.h"
#include "CCody.h"
#include "CMay.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit	CProtoMgr();
	virtual		~CProtoMgr();

public:
	HRESULT		Ready_Prototype(const WCHAR* tag, CComponent* comp);
	CComponent* Get_CloneComponent(const WCHAR* pComponentTag);

private:
	map<const WCHAR*, CComponent*>  m_mapComponent;

	// CBase을(를) 통해 상속됨
	void Free() override;
};

END
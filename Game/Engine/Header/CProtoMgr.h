#pragma once
#include "CComponent.h"
#include "CBase.h"

// 오브젝트
#include "CEmpty.h"
#include "CAlphaEmpty.h"

// 콜라이더
#include "CCube_Collider.h"
#include "CSphere_Collider.h"

// 컴포넌트
#include "CTransform.h"
#include "CTexture.h"

// 버퍼
#include "CCartBodyCol.h"
#include "CCartWheelCol.h"
#include "CRcTex.h"
#include "CPyramid.h"
#include "CInnerBox.h"
#include "CTerrain2.h"
#include "CTerrain3.h"
#include "CCube.h"
#include "CQuadrangularPrism.h"
#include "CTriangularPrism.h"
#include "CArrowTex.h"
#include "CSpline.h"
#include "CHeightMap.h"
#include "CSphere.h"

BEGIN(Engine)



struct ProtoRecord {
	WCHAR		tag[256];
	WCHAR		name[256];
	bool		addable;
	CComponent* proto;
};

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit	CProtoMgr();
	virtual		~CProtoMgr();

public:
	HRESULT		Ready_Prototype(const WCHAR* tag, CComponent* comp, bool addable = false, const WCHAR* displayName = L"");
	map<const WCHAR*, ProtoRecord>& Get_Prototypes() { return m_mapComponent; }
	CComponent* Get_CloneComponent(const WCHAR* pComponentTag);

private:
	map<const WCHAR*, ProtoRecord>  m_mapComponent;


	// CBase을(를) 통해 상속됨
	void Free() override;
};

END
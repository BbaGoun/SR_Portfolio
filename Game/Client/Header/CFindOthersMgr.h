#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CFindOthersMgr :
	public CBase
{
	DECLARE_SINGLETON(CFindOthersMgr)

private:
	explicit CFindOthersMgr();
	~CFindOthersMgr() override;

public:
	vector<CGameObject*>	GetOtherCart(CGameObject* pObj);
	vector<CGameObject*>	GetOtherCartBody(CGameObject* pObj);

private:
	void Free() override;

};



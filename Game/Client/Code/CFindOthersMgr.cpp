#include "pch.h"
#include "CFindOthersMgr.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"

IMPLEMENT_SINGLETON(CFindOthersMgr)
CFindOthersMgr::CFindOthersMgr()
{
}


CFindOthersMgr::~CFindOthersMgr()
{
	Free();
}

vector<CGameObject*> CFindOthersMgr::GetOtherCart(CGameObject* pObj)
{
	vector<CGameObject*> vecRacers;

	vecRacers.push_back(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"));
	for (auto& pBot : CManagement::GetInstance()->Find_GameObjectsByTag(L"GameLogic", L"Obj_CartBot"))
	{
		vecRacers.push_back(pBot);
	}

	vector<CGameObject*> vecOthers;
	for (auto& Obj : vecRacers)
	{
		if (Obj != pObj)
			vecOthers.push_back(Obj);
	}
	return vecOthers;
}

vector<CGameObject*> CFindOthersMgr::GetOtherCartBody(CGameObject* pObj)
{
	vector<CGameObject*> vecRacers;

	vecRacers.push_back(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody"));
	for (auto& pBot : CManagement::GetInstance()->Find_GameObjectsByTag(L"GameLogic", L"Obj_CartBotBody"))
	{
		vecRacers.push_back(pBot);
	}

	vector<CGameObject*> vecOthers;
	for (auto& Obj : vecRacers)
	{
		if (Obj != pObj)
			vecOthers.push_back(Obj);
	}
	return vecOthers;
}



void CFindOthersMgr::Free()
{
}
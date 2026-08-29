#include "pch.h"
#include "Engine_Define.h"
#include "CLoadMgr.h"
#include "CCart.h"
#include "CCartBody.h"
#include "CWheel.h"

IMPLEMENT_SINGLETON(CLoadMgr)

CLoadMgr::CLoadMgr()
{
}

CLoadMgr::~CLoadMgr()
{
	Free();
}

void CLoadMgr::ReadyCreateMap() {
	m_createMap[L""] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CEmpty::Create(pGraphicDev); };
	m_createMap[L"Alpha"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CAlphaEmpty::Create(pGraphicDev); };
	m_createMap[L"Obj_Cart"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CCart::Create(pGraphicDev); };
	m_createMap[L"Obj_CartBody"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CCartBody::Create(pGraphicDev); };
	
	m_createMap[L"Obj_Player"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CEmpty::Create(pGraphicDev); };
	m_createMap[L"Obj_PlayerHead"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CEmpty::Create(pGraphicDev); };
	m_createMap[L"Obj_PlayerArm"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CEmpty::Create(pGraphicDev); };
	
	m_createMap[L"CWheelFL"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CWheel::Create(pGraphicDev,WHEEL_FL); };
	m_createMap[L"CWheelFR"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CWheel::Create(pGraphicDev,WHEEL_FR); };
	m_createMap[L"CWheelBL"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CWheel::Create(pGraphicDev,WHEEL_BL); };
	m_createMap[L"CWheelBR"] = [](LPDIRECT3DDEVICE9 pGraphicDev) {return CWheel::Create(pGraphicDev,WHEEL_BR); };
}

CGameObject* CLoadMgr::CreateByType(const _tchar* type, LPDIRECT3DDEVICE9 pGraphicDev) {
	auto it = find_if(m_createMap.begin(), m_createMap.end(), CTag_Finder(type));

	if (it == m_createMap.end()) {
		wstring s = type;
		s += L" Type Not Set";
		MSG_BOXF(s.c_str());
		return nullptr;
	}

	return it->second(pGraphicDev);
}

CGameObject* CLoadMgr::LoadGameObjectClient(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev,
	CGameObject* pParent, CScene* pScene)
{
	CGameObject* pObj = nullptr;

	wchar_t* t = nullptr;
	while (st.Next(t))
	{
		if (!wcscmp(t, L"ENDOBJECT"))
			break;

		if (!wcscmp(t, L"OBJECT"))
		{
			LoadGameObjectClient(st, pGraphicDev, pObj, pScene);
			continue;
		}

		if (StartsWith(t, L"type=")) {
			pObj = CreateByType(t + 5, pGraphicDev);

			if (pObj == nullptr)
				return nullptr;

			const uint32_t guid = pScene->GenerateGuid();
			pObj->SetGuid(guid);
			if (pParent)
				pParent->Set_Child(pObj);
		}
		else if (StartsWith(t, L"tag=")) {
			pScene->Add_GameObject(L"GameLogic", t + 4, pObj);
		}
		else if (StartsWith(t, L"collisionLayer="))
			pObj->Set_CollisionLayer((COLLISION_LAYER)_wtoi(t + 15));
		else if (StartsWith(t, L"cullDistance="))
			pObj->Set_CullDistance(_wtoi(t + 13));
		else if (StartsWith(t, L"COMP "))
		{
			wchar_t kind[64] = {};
			wchar_t proto[256] = {};
			wchar_t mapTag[256] = {};
			ExtractCompField(t, L"kind", kind, 64);
			ExtractCompField(t, L"proto", proto, 256);
			ExtractCompField(t, L"mapTag", mapTag, 256);

			CComponent* pCom = nullptr;
			if (!wcscmp(kind, L"Transform"))
			{
				pCom = pObj->Get_Transform();
			}
			else
			{
				const WCHAR* interned = InternProtoTag(proto);
				if (interned == nullptr)
					interned = InternProtoTag(mapTag);
				if (interned)
				{
					pCom = pObj->Add_Component(interned, interned);
				}
			}

			ApplyCompProperties(st, pObj, pCom);
		}
	}

	return pObj;
}

void CLoadMgr::Free()
{
	m_createMap.clear();
}

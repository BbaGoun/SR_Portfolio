#include "CEditorLoader.h"
#include "CManagement.h"
#include "CProtoMgr.h"

IMPLEMENT_SINGLETON(CEditorLoader)

CEditorLoader::CEditorLoader() {

}

CEditorLoader::~CEditorLoader() {

}

void CEditorLoader::LoadEditorSceneFile(const _tchar* path, LPDIRECT3DDEVICE9 pGraphicDev)
{
	FILE* fp = nullptr;
	if (_wfopen_s(&fp, path, L"r, ccs=UTF-8") != 0 || !fp)
		return;

	FileReadState st;
	st.fp = fp;

	wchar_t* t = nullptr;
	CGameObject* pRoot = nullptr;
	while (true) {
		if (st.Next(t) && !wcscmp(t, L"OBJECT"))
			pRoot = LoadEditorGameObject(st, pGraphicDev, nullptr, true);
		else
			break;
	}

	fclose(fp);
}

CGameObject* CEditorLoader::LoadEditorGameObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent, bool bRoot)
{
	CGameObject* pObj = CEmpty::Create(pGraphicDev);
	if (pObj == nullptr)
		return nullptr;

	const uint32_t guid = CManagement::GetInstance()->GenerateGuid();
	pObj->SetGuid(guid);

	wstring key = to_wstring(guid);
	CManagement::GetInstance()->Add_GameObject(L"Default", key.c_str(), pObj);
	if (pParent)
		pParent->Set_Child(pObj);

	wchar_t* t = nullptr;
	while (st.Next(t))
	{
		if (!wcscmp(t, L"ENDOBJECT"))
			break;

		if (!wcscmp(t, L"OBJECT"))
		{
			LoadEditorGameObject(st, pGraphicDev, pObj, false);
			continue;
		}

		if (StartsWith(t, L"name="))
			pObj->SetName(t + 5);
		else if (StartsWith(t, L"type="))
			pObj->SetType(t + 5);
		else if (StartsWith(t, L"tag="))
			pObj->SetTag(t + 4);
		else if (StartsWith(t, L"belong="))
			pObj->Set_Belong(_wtoi(t + 7) != 0);
		else if (StartsWith(t, L"prefabPath="))
			pObj->Set_PrefabPath(t + 11);
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
					pObj->Add_Component(interned, interned);
					for (auto& pairCom : pObj->Get_ComponentMap())
					{
						if (!lstrcmp(pairCom.first, interned))
						{
							pCom = pairCom.second;
							break;
						}
					}
				}
			}

			ApplyCompProperties(st, pObj, pCom);
		}
	}

	return pObj;
}

void CEditorLoader::Free()
{
}

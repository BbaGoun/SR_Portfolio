#include "CLayer.h"

CLayer::CLayer()
{
	m_vecDelete.reserve(100);
}

CLayer::~CLayer()
{
}

CGameObject* CLayer::Find_GameObjectByTag(const _tchar* pObjTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(),
		CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second.front();
}

const vector<CGameObject*>& CLayer::Find_GameObjectsByTag(const _tchar* pObjTag)
{
	static vector<CGameObject*> s_empty;

	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(),
		CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return s_empty;

	return iter->second;
}

CComponent* CLayer::Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), 
		CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second.front()->Get_Component(eID, pComponentTag);
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(),
		CTag_Finder(pObjTag));

	if (iter == m_mapObject.end()) {
		_tchar* pTag = new _tchar[64];
		swprintf_s(pTag, 64, L"%s", pObjTag);
		vector<CGameObject*> vec;
		vec.push_back(pGameObject);
		pGameObject->SetTag(pTag);
		m_mapObject.insert({ pTag, vec });
	}
	else {
		iter->second.push_back(pGameObject);
		pGameObject->SetTag(iter->first);
	}

	if (pGameObject->Get_Parent() == nullptr)
		Attach_Root(pGameObject);

	return S_OK;
}

bool CLayer::Contains(CGameObject* _pObj) const
{
	if (!_pObj)
		return false;
	for (auto& vec : m_mapObject)
	{
		for (auto& p : vec.second) {
			if (p == _pObj)
				return true;
		}
	}
	return false;
}

void CLayer::Attach_Root(CGameObject* _pObj)
{
	if (!_pObj)
		return;
	// 이미 루트인 경우 중복되는 것을 방지
	Detach_Root(_pObj);

	if (_pObj->Get_Parent() == nullptr)
		m_vecRoots.push_back(_pObj);
}

void CLayer::Detach_Root(CGameObject* _pObj)
{
	if (!_pObj)
		return;
	m_vecRoots.erase(std::remove(m_vecRoots.begin(), m_vecRoots.end(), _pObj), m_vecRoots.end());
}

void CLayer::Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc)
{
	if (!_pDst || !_pSrc || _pDst == _pSrc)
		return;

	// 일단 루트로 불러오기
	_pSrc->To_Root();

	auto itSrc = std::find(m_vecRoots.begin(), m_vecRoots.end(), _pSrc);
	auto itDst = std::find(m_vecRoots.begin(), m_vecRoots.end(), _pDst);
	if (itSrc == m_vecRoots.end() || itDst == m_vecRoots.end())
		return;

	int iSrc = (int)(itSrc - m_vecRoots.begin());
	int iDst = (int)(itDst - m_vecRoots.begin());
	// 잠깐 삭제
	m_vecRoots.erase(m_vecRoots.begin() + iSrc);
	if (iDst > iSrc)
		--iDst;
	// 이전으로 삽입
	m_vecRoots.insert(m_vecRoots.begin() + iDst, _pSrc);
}

void CLayer::Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc)
{
	if (!_pDst || !_pSrc || _pDst == _pSrc)
		return;

	// 일단 루트로 불러오기
	_pSrc->To_Root();

	auto itSrc = std::find(m_vecRoots.begin(), m_vecRoots.end(), _pSrc);
	auto itDst = std::find(m_vecRoots.begin(), m_vecRoots.end(), _pDst);
	if (itSrc == m_vecRoots.end() || itDst == m_vecRoots.end())
		return;

	int iSrc = (int)(itSrc - m_vecRoots.begin());
	int iDst = (int)(itDst - m_vecRoots.begin()) + 1;
	//잠깐 삭제
	m_vecRoots.erase(m_vecRoots.begin() + iSrc);
	if (iDst > iSrc)
		--iDst;
	// 이후로 삽입
	m_vecRoots.insert(m_vecRoots.begin() + iDst, _pSrc);
}


HRESULT CLayer::Delete_GameObject(CGameObject* _pObj)
{
	if (!_pObj)
		return E_FAIL;

	_tchar key[256];
	wcscpy_s(key, 256, _pObj->GetTag());
	auto it = find_if(m_mapObject.begin(), m_mapObject.end(),
		CTag_Finder(key));

	if (it == m_mapObject.end()) {
		wcscpy_s(key, 256, to_wstring(_pObj->GetGuid()).c_str());
		it = find_if(m_mapObject.begin(), m_mapObject.end(), [&](pair<const _tchar*, vector<CGameObject*>> p)->bool {
			return !wcscmp(p.first, key);
			});
		
		if(it == m_mapObject.end())
			return E_FAIL;
	}

	// 자신 삭제 등록
	m_vecDelete.push_back({ wstring(key), _pObj });

	// 자식 삭제 등록
	Delete_Children(_pObj);

	// 부모의 자식 목록에서 자신을 삭제
	if(_pObj->Get_Parent())
		_pObj->Get_Parent()->Delete_Child(_pObj);
	else
		Detach_Root(_pObj);
	return S_OK;
}

void CLayer::Delete_Children(CGameObject* _pObj)
{
	auto& children = _pObj->Get_Children();
	for (auto& pObj : children) {
		_tchar key[256];
		wcscpy_s(key, 256, pObj->GetTag());
		auto it = find_if(m_mapObject.begin(), m_mapObject.end(),
			CTag_Finder(key));

		if (it == m_mapObject.end()) {
			wcscpy_s(key, 256, to_wstring(pObj->GetGuid()).c_str());
			it = find_if(m_mapObject.begin(), m_mapObject.end(), [&](pair<const _tchar*, vector<CGameObject*>> p)->bool {
				return !wcscmp(p.first, key);
				});

			if (it == m_mapObject.end())
				return;
		}
		m_vecDelete.push_back({wstring(key), pObj});
		Delete_Children(pObj);
	}
}

HRESULT CLayer::PostProcess_Delete()
{
	for (auto& p : m_vecDelete) {
		auto it = find_if(m_mapObject.begin(), m_mapObject.end(),
			CTag_Finder(p.first.c_str()));
		if (it == m_mapObject.end())
			continue;

		CGameObject* pObj = p.second;
		// 오브젝트를 삭제
		vector<CGameObject*>& vec = it->second;
		vec.erase(remove(vec.begin(), vec.end(), pObj), vec.end());
		Detach_Root(pObj);
		Safe_Release(pObj);
		
		if (vec.empty()) // vector가 비었으면
		{
			// map의 키는 const라 바로 delete[]를 할 수 없음.
			// 주소를 미리 받아오고 나중에 삭제
			_tchar* pKey = const_cast<_tchar*>(it->first);
			m_mapObject.erase(it);
			Safe_Delete_Array(pKey);
		}
	}

	m_vecDelete.clear();

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

void CLayer::FixedUpdate_Layer(const _float& fFixedDeltaTime)
{
	for (auto& p : m_mapObject)
	{
		for (auto& pObj : p.second) {
			pObj->FixedUpdate_GameObject(fFixedDeltaTime);
		}
	}	

	PostProcess_Delete();
}

_int CLayer::Update_Layer(const _float& fDeltaTime)
{
	_int	iResult(0);

	for (auto& p : m_mapObject)
	{
		for (auto& pObj : p.second) {
			iResult = pObj->Update_GameObject(fDeltaTime);

			if (iResult & 0x80000000) {
				PostProcess_Delete();
				return iResult;
			}
		}
	}

	PostProcess_Delete();

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fDeltaTime)
{
	for (auto& p : m_mapObject)
	{
		for (auto& pObj : p.second) {
			pObj->LateUpdate_GameObject(fDeltaTime);
		}
	}

	PostProcess_Delete();
}

void CLayer::Render_Layer()
{
	for (auto& p : m_mapObject)
	{
		for(auto& pObj : p.second)
			pObj->Render_GameObject();
	}
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		MSG_BOX("Layer Create Failed");
		Safe_Release(pLayer);
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	// vector 안의 오브젝트를 모두 삭제
	for_each(m_mapObject.begin(), m_mapObject.end(), [](pair<const _tchar*, vector<CGameObject*>> p)->void {
		for_each(p.second.begin(), p.second.end(), [](CGameObject* pObj)->void {
			Safe_Release(pObj);
			});
		});
	for_each(m_mapObject.begin(), m_mapObject.end(), [](pair<const _tchar*, vector<CGameObject*>> p)->void {
		// p가 복사본이라서 delete[]가 가능
		Safe_Delete_Array(p.first);
		});
	m_mapObject.clear();
}

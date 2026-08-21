#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CGameObject* Find_GameObjectByTag(const _tchar* pObjTag);
	const vector<CGameObject*>& Find_GameObjectsByTag(const _tchar* pObjTag);

	CComponent*		Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag);
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	const map<const _tchar*, vector<CGameObject*>>& Get_GameObjects() { return m_mapObject; }
	const vector<CGameObject*>& Get_Roots() { return m_vecRoots; }
	void			Attach_Root(CGameObject* _pObj);
	void			Detach_Root(CGameObject* _pObj);
	void			Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc);
	void			Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc);
	bool			Contains(CGameObject* _pObj) const;
	HRESULT			Delete_GameObject(CGameObject* _pObj, bool bEditor);
	void			Delete_Children(CGameObject* _pObj, bool bEditor);
	HRESULT			PostProcess_Delete();

public:
	HRESULT			Ready_Layer();
	void			FixedUpdate_Layer(const _float& fFixedDeltaTime);
	_int			Update_Layer(const _float& fDeltaTime);
	void			LateUpdate_Layer(const _float& fDeltaTime);
	void			Render_Layer();

private:
	map<const _tchar*, vector<CGameObject*>>		m_mapObject;
	vector<CGameObject*>							m_vecRoots;
	vector<pair<wstring, CGameObject*>>			m_vecDelete;

public:
	static CLayer* Create();

private:
	virtual void	Free();
};

END
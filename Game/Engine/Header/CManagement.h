#pragma once

#include	"CBase.h"
#include	"CScene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement();
	virtual		~CManagement();

public:
	CGameObject* Find_GameObjectByTag(const _tchar* pLayerTag, const _tchar* pObjTag);
	const vector<CGameObject*>& Find_GameObjectsByTag(const _tchar* pLayerTag, const _tchar* pObjTag);

	CComponent* Get_Component(COMPONENTID eID,
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		const _tchar* pComponentTag);

	void		Add_GameObject(
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		CGameObject*  pGameObject
	);
	const map<const _tchar*, vector<CGameObject*>>& Get_GameObjects(const _tchar* pLayerTag);
	void			Delete_GameObject(const _tchar* pLayerTag, CGameObject* _pObj, bool bEditor = false);

	// 에디터 용
	const vector<CGameObject*>& Get_Roots(const _tchar* pLayerTag);
	void			Attach_Root(CGameObject* _pObj);
	void			Detach_Root(CGameObject* _pObj);
	void			Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc);
	void			Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc);
	uint32_t		GenerateGuid() { return m_pScene->GenerateGuid(); }
	void			OnLostDevice();
	void			OnResetDevice();

	void			Set_ScenePath(const _tchar* _path) { m_pScene->Set_Path(_path); }
	const _tchar*	Get_ScenePath() { return m_pScene->Get_Path(); }
	void			Set_SceneDirty(bool _b) { m_pScene->Set_Dirty(_b); }
	bool			Get_SceneDirty() { return m_pScene->Get_Dirty(); }


public:
	HRESULT			Set_Scene(CScene* pScene);
	void			Request_Scene(CScene* pScene);
	void			Change_NextScene();

	void			FixedUpdate_Scene(const _float& fFixedDeltaTime);
	_int			Update_Scene(const _float& fDeltaTime);
	void			LateUpdate_Scene(const _float& fDeltaTime);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*					m_pScene;
	CScene*					m_pNextScene;

public:
	virtual void			Free();
};

END
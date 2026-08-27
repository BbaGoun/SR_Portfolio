#pragma once

#include "CBase.h"
#include "CLayer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CGameObject* Find_GameObjectByTag(const _tchar* pLayerTag, const _tchar* pObjTag);
	const vector<CGameObject*>& Find_GameObjectsByTag(const _tchar* pLayerTag, const _tchar* pObjTag);

	CComponent* Get_Component(COMPONENTID eID,
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		const _tchar* pComponentTag);

	HRESULT		Add_GameObject(
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		CGameObject*  pGameObject
	);

	const map<const _tchar*, vector<CGameObject*>>& Get_GameObjects(const _tchar* pLayerTag);
	HRESULT			Delete_GameObject(const _tchar* pLayerTag, CGameObject* _pObj, bool bEditor=false);

	// 에디터 용
	const vector<CGameObject*>& Get_Roots(const _tchar* pLayerTag);
	void			Attach_Root(CGameObject* _pObj);
	void			Detach_Root(CGameObject* _pObj);
	void			Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc);
	void			Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc);
	uint32_t		GenerateGuid() {
		return m_uNextGuid++;
	}
	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();

	void			Set_Path(const _tchar* _path) { wcscpy_s(m_scenePath, MAX_PATH, _path); }
	const _tchar*	Get_Path() { return m_scenePath; }
	void			Set_Dirty(bool _b) { m_bDirty = _b; }
	bool			Get_Dirty() { return m_bDirty; }

private:
	CLayer* Find_Layer_Of(CGameObject* pObj);

public:
	virtual			HRESULT		Ready_Scene();
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime);
	virtual			_int		Update_Scene(const _float& fDeltaTime);
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime);
	virtual			void		Render_Scene();

	void			Process_Collision(const vector<CGameObject*>& vecObjects);

	void			Set_CollisionMatrix(COLLISION_LAYER srcLayer, COLLISION_LAYER dstLayer, bool bCollision);
	bool			Get_CollisionMatrix(COLLISION_LAYER srcLayer, COLLISION_LAYER dstLayer);

protected:
	_tchar								m_scenePath[MAX_PATH] = L"\0";
	map<const _tchar*, CLayer*>			m_mapLayer;
	uint32_t							m_CollisionMatrix[32];
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	uint32_t							m_uNextGuid = 1;
	bool								m_bDirty = true;

protected:
	virtual void			Free();

};

END

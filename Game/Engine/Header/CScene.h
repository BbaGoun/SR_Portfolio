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
	const vector<CGameObject*>& Get_Roots(const _tchar* pLayerTag);
	void			Attach_Root(CGameObject* _pObj);
	void			Detach_Root(CGameObject* _pObj);
	void			Insert_Root_Before(CGameObject* _pDst, CGameObject* _pSrc);
	void			Insert_Root_After(CGameObject* _pDst, CGameObject* _pSrc);
	HRESULT			Delete_GameObject(const _tchar* pLayerTag, CGameObject* _pObj);
	uint64_t		GenerateGuid() {
		return m_uNextGuid++;
	}
	virtual			void		InvalidateDeviceObjects() {};

private:
	CLayer* Find_Layer_Of(CGameObject* pObj);

public:
	virtual			HRESULT		Ready_Scene();
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime);
	virtual			_int		Update_Scene(const _float& fDeltaTime);
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime);
	virtual			void		Render_Scene();


	void			Set_CollisionMatrix(COLLISION_LAYER srcLayer, COLLISION_LAYER dstLayer, bool bCollision);

protected:
	map<const _tchar*, CLayer*>			m_mapLayer;
	bitset<528>							m_CollisionMatrix;
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	uint64_t							m_uNextGuid = 1;

protected:
	virtual void			Free();

};

END
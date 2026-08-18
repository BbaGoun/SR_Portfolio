#pragma once
#include "CBase.h"
#include "CComponent.h"
#include "CTransform.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CCollider;
class CLayer;
class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<CComponent, T>>>
	T* Get_Component();
	template<typename T, typename = std::enable_if_t<std::is_base_of_v<CComponent, T>>>
	vector<T*> Get_Components();
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pComponentTag);
	const map<const _tchar*, CComponent*>& Get_ComponentMap() { return m_mapComponent; }

	CTransform* Get_Transform() { return m_pTransformCom; }
	void	Add_Component(const WCHAR* pProtoTag, const WCHAR* pComponentTag);
	void	Remove_Component(CComponent* _pCom);

	void	Set_Child(CGameObject* _pGO);
	void	Insert_Child(CGameObject* _pGO, int _iIndex);
	void	Insert_Before(CGameObject* _pGO);
	void	Insert_After(CGameObject* _pGO);
	const vector<CGameObject*>& Get_Children() { return m_vecChildren; }
	void	Delete_Child(CGameObject* _pObj);
	void	To_Root();
	CGameObject* Get_Parent() { return m_pParent; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) {};
	virtual			_int		Update_GameObject(const _float& fDeltaTime);
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime);
	virtual			void		Render_GameObject();

	virtual			void		PreCull_GameObject() {};
	virtual			void		PreRender_GameObject() {};
	virtual			void		PostRender_GameObject() {};

	virtual			void		CollisionEnter(CCollider* pOtherCollider) {};
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) {};
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

public:
	void			Set_CollisionLayer(COLLISION_LAYER eID);
	COLLISION_LAYER	Get_CollisionLayer() { return m_eCollisionLayer; }
	uint32_t		Get_CollisionLayerBit() { return m_uCollisionLayerBit; }

	_vec3			Get_Force() { return m_vForce; }
	void			Set_Force(_vec3 _newForce) { m_vForce = _newForce; }
	void			Add_Force(_vec3 _addedForce) { m_vForce += _addedForce; }

	_vec3			Get_Rotation() { return m_vRotation; }
	void			Set_Rotation(_vec3 _newRotation) { m_vRotation = _newRotation; }
	void			Add_Rotation(_vec3 _addedRotation) { m_vRotation += _addedRotation; }

	_float			Get_Speed() { return m_fSpeed; }

	_float			Get_ViewZ() { return m_fViewZ; }
	void			Compute_ViewZ(const _vec3* pPos);


	void			Set_PrefabPath(const _tchar* _path) { wcscpy_s(m_prefabPath, MAX_PATH, _path); }
	const _tchar*	Get_PrefabPath() { return m_prefabPath; }
	void			Set_Belong(bool _b) { m_bBelongPrefab = _b; }
	bool			Get_Belong() { return m_bBelongPrefab; }

	void			SetGuid(uint32_t _guid) { m_uGuid = _guid; }
	uint32_t		GetGuid() { return m_uGuid; }
	void			SetType(const WCHAR* _type) { wcscpy_s(m_wType, 256, _type); }
	const WCHAR*	GetType() { return m_wType; }
	void			SetName(const WCHAR* _name) { wcscpy_s(m_wName, 256, _name); }
	const WCHAR*	GetName() { return m_wName; }
	void			SetTag(const WCHAR* _tag) { wcscpy_s(m_wTag, 256, _tag); }
	const WCHAR*	GetTag() { return m_wTag; }

	void			Set_CullDistance(uint32_t _uCullDistance) {m_uCullDistance = _uCullDistance;}
	uint32_t		Get_CullDistance() {return m_uCullDistance;}

	void			SetLayer(CLayer* pLayer) { m_pLayer = pLayer; }
	CLayer*			GetLayer() { return m_pLayer; }

protected:
	_tchar									m_prefabPath[MAX_PATH] = L"\0";
	bool									m_bBelongPrefab = false;

	uint32_t								m_uGuid;
	WCHAR									m_wType[256] = L"\0";
	WCHAR									m_wName[256] = L"\0";
	WCHAR									m_wTag[256] = L"\0";

	map<const _tchar*, CComponent*>			m_mapComponent;
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	
	vector<CGameObject*>					m_vecChildren;
	CGameObject*							m_pParent;
	CTransform*								m_pTransformCom;
	
	COLLISION_LAYER							m_eCollisionLayer;
	uint32_t								m_uCollisionLayerBit;

	uint32_t								m_uCullDistance;

	_float									m_fSpeed;
	_vec3									m_vForce;
	_vec3									m_vRotation;


	_float									m_fViewZ;

	CLayer*									m_pLayer;

private:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

protected:
	virtual		void		Free();
};

END

template<typename T, typename>
inline T* CGameObject::Get_Component()
{
	for (auto& p : m_mapComponent) {
		if (T* pCom = dynamic_cast<T*>(p.second))
			return pCom;
	}
	return nullptr;
}

template<typename T, typename>
inline vector<T*> CGameObject::Get_Components()
{
	vector<T*> vec;
	for (auto& p : m_mapComponent) {
		if (T* pCom = dynamic_cast<T*>(p.second))
			vec.push_back(pCom);
	}
	return vec;
}

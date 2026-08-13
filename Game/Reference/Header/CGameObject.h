#pragma once
#include "CBase.h"
#include "CComponent.h"
#include "CTransform.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pComponentTag);
	CTransform* Get_Transform() { return m_pTransformCom; }
	void	Set_Child(CGameObject* _pGO);
	void	Insert_Child(CGameObject* _pGO, int _iIndex);
	void	Insert_Before(CGameObject* _pGO);
	void	Insert_After(CGameObject* _pGO);
	const vector<CGameObject*>& Get_Children() { return m_vecChildren; }
	CGameObject* Get_Parent() { return m_pParent; }
	void	Delete_Child(CGameObject* _pObj);
	void	To_Root();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) {};
	virtual			_int		Update_GameObject(const _float& fDeltaTime);
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime);
	virtual			void		Render_GameObject();

	virtual			void		PreCull_GameObject() {};
	virtual			void		PreRender_GameObject() {};
	virtual			void		PostRender_GameObject() {};

	virtual			void		CollisionEnter() {};
	virtual			void		CollisionExit() {};
	virtual			void		CollisionStay() {};

	virtual			void		TriggerEnter() {};
	virtual			void		TriggerExit() {};
	virtual			void		TriggerStay() {};

public:
	void			Set_CollisionLayer(COLLISION_LAYER eID);

	_vec3			Get_Force() { return m_vForce; }
	void			Set_Force(_vec3 _newForce) { m_vForce = _newForce; }
	void			Add_Force(_vec3 _addedForce) { m_vForce += _addedForce; }

	_vec3			Get_Rotation() { return m_vRotation; }
	void			Set_Rotation(_vec3 _newRotation) { m_vRotation = _newRotation; }
	void			Add_Rotation(_vec3 _addedRotation) { m_vRotation += _addedRotation; }

	_float			Get_Speed() { return m_fSpeed; }

	void			SetGuid(uint64_t _guid) { m_uGuid = _guid; }
	uint64_t		GetGuid() { return m_uGuid; }
	void			SetName(const WCHAR* _name) { wcscpy_s(m_wName, 128, _name); }
	const WCHAR*	GetName() { return m_wName; }
	void			SetTag(const WCHAR* _tag) { wcscpy_s(m_wTag, 128, _tag); }
	const WCHAR*	GetTag() { return m_wTag; }

protected:
	uint64_t								m_uGuid;
	WCHAR									m_wName[128];
	WCHAR									m_wTag[128];

	map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	
	vector<CGameObject*>					m_vecChildren;
	CGameObject*							m_pParent;
	CTransform*								m_pTransformCom;
	uint32_t								m_iCollisionLayer;

	uint32_t								m_iCullDistance;

	_float									m_fSpeed;
	_vec3									m_vForce;
	_vec3									m_vRotation;

private:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

protected:
	virtual		void		Free();
};

END
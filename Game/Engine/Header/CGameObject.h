#pragma once
#include "CBase.h"
#include "CComponent.h"
#include "CTransform.h"

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
	void	Set_Child(CGameObject* _pGO){
		m_vecChildren.push_back(_pGO);
		_pGO->m_pParent = this;
		_pGO->Get_Transform()->Set_Dirty();
	}
	const vector<CGameObject*>& Get_Children() { return m_vecChildren; }
	CGameObject* Get_Parent() { return m_pParent; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) {};
	virtual			_int		Update_GameObject(const _float& fDeltaTime);
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime);
	virtual			void		Render_GameObject();
	
	virtual			void		CollisionEnter() {};
	virtual			void		CollisionExit() {};
	virtual			void		CollisionStay() {};
	
	virtual			void		TriggerEnter() {};
	virtual			void		TriggerExit() {};
	virtual			void		TriggerStay() {};

public:
	void			Set_CollisionLayer(COLLISION_LAYER eID);

protected:
	map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	
	vector<CGameObject*>					m_vecChildren;
	CGameObject*							m_pParent;
	CTransform*								m_pTransformCom;
	uint32_t								m_iCollisionLayer;

private:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

protected:
	virtual		void		Free();
};

END
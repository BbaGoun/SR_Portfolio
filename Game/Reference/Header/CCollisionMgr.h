#pragma once
#include "CCollider.h"
#include "CTransform.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	virtual void FixedUpdate_Component();
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component();

public:
	static	void	Collision(CCollider* pDst, CCollider* pSrc);
	static	bool	CubeVsCube(CCollider* pDst, CCollider* pSrc);
	static	bool	SphereVsSphere(CCollider* pDst, CCollider* pSrc);
	static	bool	CubeVsSphere(CCollider* pDst, CCollider* pSrc);
	static	bool	SphereVsCube(CCollider* pDst, CCollider* pSrc);

//void	AddCollider(OBJID eID, CCollider* pCollider);
//private:
//	list<CCollider*>	m_ColliderList[OBJ_END];
};

END
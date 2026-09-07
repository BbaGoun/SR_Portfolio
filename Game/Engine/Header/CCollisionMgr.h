#pragma once
#include "CCollider.h"
#include "CTransform.h"
#include "CGameObject.h"

BEGIN(Engine)
class CCube_Collider;
class CSphere_Collider;

class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	static	void	Collision(CCollider* pDst, CCollider* pSrc);
	static	_vec3	GetMTVCubevsCube(CCube_Collider* pDst, CCube_Collider* pSrc);
	static	_vec3	GetMTVSphereVsSphere(CSphere_Collider* pDst, CSphere_Collider* pSrc);
	//static	bool	CubeVsSphere(CCollider* pDst, CCollider* pSrc);
	//static	bool	SphereVsCube(CCollider* pDst, CCollider* pSrc);

//void	AddCollider(OBJID eID, CCollider* pCollider);
//private:
//	list<CCollider*>	m_ColliderList[OBJ_END];
};

END
#pragma once
#include "CObj.h"

BEGIN(Engine)

// CObj 를 상속해 특정 모델(Plane01)을 로드하는 파생 컴포넌트.
// CPlayerMesh 가 CMesh 를 상속하는 구조와 동일하다.
class ENGINE_DLL CAirplane :
	public CObj
{
protected:
	explicit CAirplane();
	explicit CAirplane(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAirplane(const CAirplane& rhs);
	virtual ~CAirplane() override;

public:
	virtual HRESULT		Ready_Obj() override;
	virtual void		Render_Obj() override;

public:
	static	CAirplane*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

protected:
	virtual void		Free() override;
};

END

#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CTerrain3.h"

class CLand3 :
	public CGameObject
{
private:
	explicit CLand3(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLand3(const CGameObject& rhs);
	~CLand3() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			bool		CheckInTerrain(_vec3 vPos);

private:
	CTerrain3* m_pBufferCom;

public:
	static CLand3* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

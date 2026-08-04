#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CTerrain.h"

class CLand2 :
	public CGameObject
{
private:
	explicit CLand2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLand2(const CGameObject& rhs);
	~CLand2() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CTerrain* m_pBufferCom;

public:
	static CLand2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

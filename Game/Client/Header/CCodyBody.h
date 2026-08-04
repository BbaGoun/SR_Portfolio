#pragma once
#include "CGameObject.h"
#include "CCody.h"

class CCodyBody :
	public CGameObject
{
private:
	explicit CCodyBody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCodyBody(const CGameObject& rhs);
	~CCodyBody() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CCody* m_pBufferCom;

public:
	static CCodyBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

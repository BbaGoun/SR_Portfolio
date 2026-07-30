#pragma once
#include "CGameObject.h"
class CHeadPivot :
    public CGameObject
{
private:
	explicit CHeadPivot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHeadPivot(const CGameObject& rhs);
	~CHeadPivot() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override {};

private:
	void	Mouse_Input(const _float& fTimeDelta);

private:
	_float	m_fPitch;

public:
	static CHeadPivot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


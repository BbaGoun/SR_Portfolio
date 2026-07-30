#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CCody.h"
#include "CCamera.h"

class CGOCody :
	public CGameObject
{
private:
	explicit CGOCody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGOCody(const CGameObject& rhs);
	~CGOCody() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

private:
	void	Key_Input(const _float& fTimeDelta);
	void	Mouse_Input(const _float& fTimeDelta);
	void	Mouse_Fix();
	void	AdjustPosY(_vec3 pos);

private:
	_float	m_fSpeed;
	_vec3	m_vForce = { 0, 0, 0 };
	_bool	m_bJump = false;
	_float	m_fJumpTime;

	_bool		m_bFix;
	_bool		m_bCheck;

public:
	static CGOCody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

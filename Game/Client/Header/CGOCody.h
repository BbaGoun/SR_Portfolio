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
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override {};

public:
	void	Set_Target(_vec3 pos) { m_bTarget = true; m_vTargetPos = pos; }

private:
	void	Key_Input();
	void	Mouse_Input();
	void	Mouse_Fix();
	void	AdjustPosY(_vec3 pos);
	void	FollowTarget();

private:
	_float	m_fSpeed;
	_vec3	m_vForce = { 0, 0, 0 };
	_bool	m_bJump = false;
	_float	m_fJumpTime;
	_vec3	m_vTargetPos;
	_bool	m_bTarget;

	_bool		m_bFix;
	_bool		m_bCheck;

public:
	static CGOCody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

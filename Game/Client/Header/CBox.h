#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CCartBodyCol;
	class CCube_Collider;
}
class CBox : public CGameObject
{
private:
	explicit CBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBox(const CGameObject& rhs);
	virtual ~CBox() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override ;

	void		KeyInput(const _float& fDeltaTime);
public:
	static CBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CCartBodyCol*		m_pBufferCom;
	Engine::CCube_Collider*		m_pColliderCom;
			_float				m_fMaxSpeed;

protected:
	virtual		void		Free() override;
};


#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CCube_Collider;
	class CSphere_Collider;
}
class CCartBody : public CGameObject
{
private:
	explicit CCartBody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCartBody(const CGameObject& rhs);
	virtual ~CCartBody() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void			BananaSpin(const _float& fDeltaTime);
	bool			m_bBananaSpinState;
	float			m_fSpinSpeed;

public:
	static CCartBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


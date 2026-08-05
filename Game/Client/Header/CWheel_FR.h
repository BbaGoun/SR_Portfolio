#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CCartWheelCol;
}
class CWheel_FR : public CGameObject
{
private:
	explicit CWheel_FR(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWheel_FR(const CGameObject& rhs);
	virtual ~CWheel_FR() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CWheel_FR* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CCartWheelCol* m_pBufferCom;

protected:
	virtual		void		Free() override;
};


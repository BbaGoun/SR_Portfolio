#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CCartWheelCol;
}
class CWheel : public CGameObject
{
private:
	explicit CWheel(LPDIRECT3DDEVICE9 pGraphicDev, WHEEL_TYPE eType);
	explicit CWheel(const CGameObject& rhs);
	virtual ~CWheel() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			SetWheelType(WHEEL_TYPE eType) { m_eWheelType = eType; }
	WHEEL_TYPE		GetWheelType() { return m_eWheelType; }
	void			KeyInput(const _float& fDeltaTime);

	void			CreateSkidMark();
public:
	static CWheel* Create(LPDIRECT3DDEVICE9 pGraphicDev, WHEEL_TYPE eType);

private:
	Engine::CCartWheelCol*	m_pBufferCom;
	WHEEL_TYPE				m_eWheelType;

protected:
	virtual		void		Free() override;
};


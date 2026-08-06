#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTriangularPrism;
}
class CCartBody2 : public CGameObject
{
private:
	explicit CCartBody2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCartBody2(const CGameObject& rhs);
	virtual ~CCartBody2() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CCartBody2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTriangularPrism* m_pBufferCom;

protected:
	virtual		void		Free() override;
};


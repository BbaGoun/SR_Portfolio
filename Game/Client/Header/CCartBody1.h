#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CQuadrangularPrism;
}
class CCartBody1 : public CGameObject
{
private:
	explicit CCartBody1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCartBody1(const CGameObject& rhs);
	virtual ~CCartBody1() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CCartBody1* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CQuadrangularPrism* m_pBufferCom;

protected:
	virtual		void		Free() override;
};


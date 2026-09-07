#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTexture;
	class CRcTex;
}
class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CGameObject& rhs);
	virtual ~CPlayer() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	float				m_fFrame = 0;

	bool				m_bBoost;

protected:
	virtual		void		Free() override;
};


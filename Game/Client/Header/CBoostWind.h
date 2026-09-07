#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTexture;
	class CRcTex;
}
class CBoostWind : public CGameObject
{
private:
	explicit CBoostWind(LPDIRECT3DDEVICE9 pGraphicDev, BOOSTER_TYPE eID);
	explicit CBoostWind(const CGameObject& rhs);
	virtual ~CBoostWind() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CBoostWind* Create(LPDIRECT3DDEVICE9 pGraphicDev, BOOSTER_TYPE eID);

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;

	float				m_fFrame = 0;

	BOOSTER_TYPE		m_eBoosterID;

protected:
	virtual		void		Free() override;
};


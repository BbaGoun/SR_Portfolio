#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTexture;
	class CRcTex;
}
class CDriftSpark : public CGameObject
{
private:
	explicit CDriftSpark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDriftSpark(const CGameObject& rhs);
	virtual ~CDriftSpark() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CDriftSpark* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

	float					m_fFrame = 0;

	DRIFT_SPARK_TYPE		m_eSparkID;

protected:
	virtual		void		Free() override;
};


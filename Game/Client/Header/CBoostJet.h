#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTexture;
	class CRcTex;
}
class CBoostJet : public CGameObject
{
private:
	explicit CBoostJet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoostJet(const CGameObject& rhs);
	virtual ~CBoostJet() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CBoostJet* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

	float				m_fFrame = 0;

protected:
	virtual		void		Free() override;
};


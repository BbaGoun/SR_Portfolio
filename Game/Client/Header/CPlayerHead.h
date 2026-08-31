#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTexture;
	class CRcTex;
}
class CPlayerHead : public CGameObject
{
private:
	explicit CPlayerHead(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerHead(const CGameObject& rhs);
	virtual ~CPlayerHead() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void		SetBoost(bool bBoost) { m_bBoost = bBoost; }
	void		SetCartDirType(DIRECTION_TYPE eCartDirType) { m_eCartDirType = eCartDirType; }
	void		KeyInput(const _float& fDeltaTime);
public:
	static CPlayerHead* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	float				m_fFrame = 0;

	bool				m_bBoost;
	DIRECTION_TYPE		m_eCartDirType;

protected:
	virtual		void		Free() override;
};


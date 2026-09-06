#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CTexture;
	class CRcTex;
}
class CPlayerArm : public CGameObject
{
private:
	explicit CPlayerArm(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerArm(const CGameObject& rhs);
	virtual ~CPlayerArm() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void		SetBoost(bool bBoost) { m_bBoost = bBoost; }
	void		SetCartDirType(DIRECTION_TYPE eCartDirType) { m_eCartDirType = eCartDirType; }
	void		KeyInput(const _float& fDeltaTime);
	void		SetKeyInput(bool bKeyInput) { m_bKeyInput = bKeyInput; }

public:
	static CPlayerArm* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	float				m_fFrame = 0;

	bool				m_bBoost;
	DIRECTION_TYPE		m_eCartDirType;
	bool				m_bKeyInput = true;

protected:
	virtual		void		Free() override;
};


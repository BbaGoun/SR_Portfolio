#pragma once
#include "PSystem.h"
BEGIN(Engine)
class ENGINE_DLL CItemGainParticle : public PSystem
{
private:
	explicit CItemGainParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemGainParticle() override;

public:
	virtual		HRESULT		Ready_PSystem();
	virtual		void		ResetParticle(Attribute* attribute) override;
	virtual		void		Update_PSystme(float timeDelta) override;

	virtual		void SetOrigin(const _vec3& vOrigin) { m_vOrigin = vOrigin; }
	virtual		void ResetAll();
public:
	static CItemGainParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();

public:
	virtual void Free() override;
};
END
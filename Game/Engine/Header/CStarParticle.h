#pragma once
#include "PSystem.h"
BEGIN(Engine)
class ENGINE_DLL CStarParticle : public PSystem
{
private:
	explicit CStarParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStarParticle() override;

public:
	virtual		HRESULT		Ready_PSystem();
	virtual		void		ResetParticle(Attribute* attribute) override;
	virtual		void		Update_PSystme(float timeDelta) override;

	void SetOrigin(const _vec3& vOrigin) { m_vOrigin = vOrigin; }
	void ResetAll();
public:
	static CStarParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();

public:
	virtual void Free() override;
};
END
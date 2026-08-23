#pragma once
#include "PSystem.h"
BEGIN(Engine)
class ENGINE_DLL CSmoke : public PSystem
{
private:
	explicit CSmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSmoke() override;

public:
	virtual		HRESULT		Ready_PSystem();
	virtual		void		ResetParticle(Attribute* attribute) override;
	virtual		void		Update_PSystme(float timeDelta) override;

	void SetOrigin(const _vec3& vOrigin) { m_vOrigin = vOrigin; }
	void SetBackDir(const _vec3& vBackDir) { m_vBackDir = vBackDir; }

public:
	static CSmoke* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_vec3 m_vBackDir;   // 카트 뒤 방향

public:
	virtual void Free() override;
};
END
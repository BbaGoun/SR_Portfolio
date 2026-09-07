#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
}
class CLeftMirror :
	public CGameObject
{
private:
	explicit CLeftMirror(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLeftMirror(const CLeftMirror& rhs);
	virtual ~CLeftMirror();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	Engine::CRcTex* m_pVIBufferCom;

public:
	static CLeftMirror* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;

};
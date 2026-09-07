#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CCube_Collider.h"

namespace Engine
{
	class CSphere;
	class CRcTex;
}

class CWaterBombBubble : public CGameObject
{
private:
	explicit CWaterBombBubble(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWaterBombBubble(const CGameObject& rhs);
	~CWaterBombBubble() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void			SetShow(bool bShow) { m_bShow = bShow; }

private:
	CSphere*	m_pBufferCom;
	CTexture*	m_pTextureCom;

	_float		m_fTimer;
	_bool		m_bShow = false;

public:
	static CWaterBombBubble* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
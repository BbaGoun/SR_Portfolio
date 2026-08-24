#pragma once
#include "CGameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CUI_Laps :
	public CGameObject
{
private:
	explicit CUI_Laps(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Laps(const CUI_Laps& rhs);
	virtual ~CUI_Laps();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	Engine::CRcTex* m_pVIBufferCom;
	Engine::CTexture* m_pTextureCom;

	_float		m_fFrame_Cur;
	_float		m_fFrame_Entire;


public:
	static CUI_Laps* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};
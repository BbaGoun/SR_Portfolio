#pragma once
#include "CGameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CUI_Timer :
	public CGameObject
{
private:
	explicit CUI_Timer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Timer(const CUI_Timer& rhs);
	virtual ~CUI_Timer();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			Set_Timer(float _fPlayTime);

private:
	Engine::CRcTex* m_pVIBufferCom;
	Engine::CTexture* m_pTextureCom;

	int			m_iMilli_1;
	int			m_iMilli_10;
	int			m_iSecond_1;
	int			m_iSecond_10;
	int			m_iMin_1;
	int			m_iMin_10;

public:
	static CUI_Timer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;

};
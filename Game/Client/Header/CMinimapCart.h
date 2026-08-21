#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CArrowTex;
	class CTexture;
}
class CMinimapCart :
	public CGameObject
{
private:
	explicit CMinimapCart(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMinimapCart(const CMinimapCart& rhs);
	virtual ~CMinimapCart();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


private:
	CArrowTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;
public:
	static CMinimapCart* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};
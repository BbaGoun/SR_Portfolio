#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CMinimapGround :
	public CGameObject
{
private:
	explicit CMinimapGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMinimapGround(const CMinimapGround& rhs);
	virtual ~CMinimapGround();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


private:
	CRcTex*		m_pVIBufferCom;
	CTexture*	m_pTextureCom;
public:
	static CMinimapGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};
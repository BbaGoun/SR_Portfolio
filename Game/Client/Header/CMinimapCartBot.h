#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCircleCol;
	class CTexture;
}
class CMinimapCartBot :
	public CGameObject
{
private:
	explicit CMinimapCartBot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMinimapCartBot(const CMinimapCartBot& rhs);
	virtual ~CMinimapCartBot();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


private:
	CCircleCol*		m_pVIBufferCom;
	CTexture*		m_pTextureCom;

	CGameObject*	m_pCartBot = nullptr;
public:
	static CMinimapCartBot* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	SetCartBot(CGameObject* pCartBot) { m_pCartBot = pCartBot; }

protected:
	virtual		void		Free() override;

};
#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
}
class CUI_Minimap :
	public CGameObject
{
private:
	explicit CUI_Minimap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Minimap(const CUI_Minimap& rhs);
	virtual ~CUI_Minimap();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


private:
	Engine::CRcTex* m_pVIBufferCom;

public:
	static CUI_Minimap* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};
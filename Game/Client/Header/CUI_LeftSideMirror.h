#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
}
class CUI_LeftSideMirror :
	public CGameObject
{
private:
	explicit CUI_LeftSideMirror(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_LeftSideMirror(const CUI_LeftSideMirror& rhs);
	virtual ~CUI_LeftSideMirror();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void	Set_Show(bool bShow) { m_bShow = bShow; };
	bool	Get_Show() { return m_bShow; }

private:
	Engine::CRcTex* m_pVIBufferCom;



public:
	static CUI_LeftSideMirror* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	_vec3			m_vPos;
	_vec3			m_vScale;

	bool			m_bShow = true;

protected:
	virtual		void		Free() override;

};
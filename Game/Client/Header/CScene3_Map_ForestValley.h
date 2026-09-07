
#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CScene3_Map_ForestValley :
	public CGameObject
{
private:
	explicit CScene3_Map_ForestValley(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene3_Map_ForestValley(const CGameObject& rhs);
	~CScene3_Map_ForestValley() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;
public:
	static CScene3_Map_ForestValley* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



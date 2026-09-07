
#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_NexonLogo :
	public CGameObject
{
private:
	explicit CUI_NexonLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_NexonLogo(const CGameObject& rhs);
	~CUI_NexonLogo() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void						SetFinished(bool bFinished) const { bFinished = m_bVideoFinished; }
	bool						GetFinished() {return m_bVideoFinished ; }

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;
	
	bool	m_bVideoFinished;
	float	m_fFrame;
	float	m_fMaxFrame = 106.f;


	_vec3	m_vPos;
	_vec3	m_vScale;
public:
	static CUI_NexonLogo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};



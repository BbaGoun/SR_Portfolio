#pragma once
#include "CGameObject.h"
#include "CTexture.h"
#include "CMeshRibbon.h"

class CWheel;

class CSkidMark : public CGameObject
{
private:
	explicit CSkidMark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkidMark(const CGameObject& rhs);
	~CSkidMark() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;
	virtual			void		Compute_ViewZ() override;

public:
	void		Append_Point(_vec3 vPos);

private:
	CTexture* m_pTextureCom;
	CMeshRibbon* m_pMeshRibbonCom;

	float		m_fRemoveTimer = 0.f;
	float		m_fWidthTimer = 0.f;
	float		m_fNextWidthTimer = 0.05f;

	CWheel* m_pWheel;

public:
	static CSkidMark* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CWheel* pWheel);

	virtual			void		OnLostDevice() override;
	virtual			void		OnResetDevice() override;

protected:
	virtual		void		Free() override;
};



#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSpline : 
	public CVIBuffer
{
protected:
	explicit CSpline(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpline(const CSpline& rhs);
	virtual ~CSpline();

public:
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void LateUpdate_Component();
	virtual void Render_Component(D3DXCOLOR color) {}

public:
	HRESULT			Ready_CSplineCom();
	HRESULT			Ready_Buffer() override;
	void			Render_Buffer() override;

	static			CSpline* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			CComponent* Clone(); //PURE·Î ¼³Á¤

public:
	void	Set_SampleUnit(float _fSampleUnit) { m_fSampleUnit = _fSampleUnit; }
	float	Get_SampleUnit() { return m_fSampleUnit; }
	void	Set_TextureUnit(float _fTextureUnit) { m_fTextureUnit = _fTextureUnit; }
	float	Get_TextureUnit() { return m_fTextureUnit; }

protected:
	vector<ControlPoint> m_vecControlPoint;
	bool	m_bAddDelDirty = false;
	bool	m_bEditDirty = false;
	float	m_fSampleUnit = 10.f;
	float	m_fTextureUnit = 10.f;

protected:
	virtual		void		Free() override;
};

END

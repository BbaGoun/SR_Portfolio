#pragma once
#include "CVIBuffer.h"
#include "CTexture.h"

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
	void			Add_Point();
	void			Del_Point(ControlPoint* pCp);
	void			Render_Points();

	ControlPoint*	Get_ControlPoint(uint32_t cpId);

	static			CSpline* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			CComponent* Clone(); //PURE·Î ¼³Á¤

private:
	void			PreRender_Points();
	void			PostRender_Points();

public:
	vector<ControlPoint>& Get_ControlPoints() { return m_vecControlPoint; }

	void	Set_Edit(bool _b) { m_bEdit = _b; }
	bool	Get_Edit() { return m_bEdit; }
	void	Set_AddDelDirty(bool _b) { m_bAddDelDirty = _b; }
	bool	Get_AddDelDirty() { return m_bAddDelDirty; }
	void	Set_PointDirty(bool _b) { m_bPointDirty = _b; }
	bool	Get_PointDirty() { return m_bPointDirty; }

	void	Set_SampleUnit(float _fSampleUnit) { m_fSampleUnit = _fSampleUnit; }
	float	Get_SampleUnit() { return m_fSampleUnit; }
	void	Set_TextureUnit(float _fTextureUnit) { m_fTextureUnit = _fTextureUnit; }
	float	Get_TextureUnit() { return m_fTextureUnit; }

	uint32_t GenerateId() { return m_uGenerateId++; }

protected:
	uint32_t m_uGenerateId = 1;
	CTexture* m_pTexNormal;
	CTexture* m_pTexEdit;

	vector<ControlPoint> m_vecControlPoint;

	bool	m_bEdit = false;
	bool	m_bAddDelDirty = false;
	bool	m_bPointDirty = false;
	float	m_fSampleUnit = 10.f;
	float	m_fTextureUnit = 10.f;

protected:
	virtual		void		Free() override;
};

END

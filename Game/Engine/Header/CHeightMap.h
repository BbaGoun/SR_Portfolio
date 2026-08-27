#pragma once
#include "CVIBuffer.h"
#include "CTexture.h"

BEGIN(Engine)

class CHeightMap :
    public CVIBuffer
{
protected:
	explicit CHeightMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHeightMap(const CHeightMap& rhs);
	virtual ~CHeightMap();

public:
	virtual _int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component() {}

public:
	HRESULT			Ready_CHeightMap();
	HRESULT			Ready_Buffer() override;
	void			Render_Buffer() override;

	void			Render_Points();

private:
	void			PreRender_Points();
	void			PostRender_Points();

public:
	static			CHeightMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			CComponent* Clone(); //PURE·Î ¼³Á¤

public:
	vector<VTXTEX>& GetVertices() { return m_vecVertices; }
	vector<FACE32>& GetFaces() { return m_vecFaces; }

	void	Set_Edit(bool _b) { m_bEdit = _b; }
	bool	Get_Edit() { return m_bEdit; }
	void	Set_EditRadius(float _fRadius) { m_fEditRadius = _fRadius; }
	bool	Get_EditRadius() { return m_fEditRadius; }

	void	Set_CntX(int _iCntX) { m_iCntX = _iCntX; }
	float	Get_CntX() { return m_iCntX; }
	void	Set_CntZ(float _iCntZ) { m_iCntZ = _iCntZ; }
	float	Get_CntZ() { return m_iCntZ; }

	void	Set_ItvX(float _fItvX) { m_fItvX = _fItvX; }
	float	Get_ItvX() { return m_fItvX; }
	void	Set_ItvZ(float _fItvZ) { m_fItvZ = _fItvZ; }
	float	Get_ItvZ() { return m_fItvZ; }

	uint32_t GenerateId() { return m_uGenerateId++; }

protected:
	uint32_t m_uGenerateId = 1;
	CTexture* m_pTexNormal;
	CTexture* m_pTexEdit;

	vector<VTXTEX>			m_vecVertices;
	vector<FACE32>			m_vecFaces;

	bool	m_bEdit = false;
	float	m_fEditRadius = 1;

	int		m_iCntX = 10;
	int		m_iCntZ = 10;

	float	m_fItvX = 1;
	float	m_fItvZ = 1;

protected:
	virtual		void		Free() override;
};

END

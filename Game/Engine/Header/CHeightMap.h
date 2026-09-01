#pragma once
#include "CVIBuffer.h"
#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CHeightMap:
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
	void			Adjust_Itv();
	void			HeightMap_Edit(_vec3 _pickPos, bool bShift);
	HRESULT			Ready_BufferByVec();

	void			Render_Buffer() override;


	void			Render_Points();
	void			Render_Brush(_vec3 _pickPos);

private:
	_vec3			GetHeightFromXZ(_vec3 pos);
	void			Adjust_Edit();

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
	void	Set_EditStrength(float _fStrength) { m_fEditStrength = _fStrength; }
	float	Get_EditStrength() { return m_fEditStrength; }
	void	Set_EditRadius(float _fRadius) { m_fEditRadius = _fRadius; }
	float	Get_EditRadius() { return m_fEditRadius; }

	void	Set_CntX(int _iCntX) { m_iCntX = _iCntX; }
	int		Get_CntX() { return m_iCntX; }
	void	Set_CntZ(float _iCntZ) { m_iCntZ = _iCntZ; }
	int		Get_CntZ() { return m_iCntZ; }

	void	Set_Itv(float _fItv) { m_fItv = _fItv; }
	float	Get_Itv() { return m_fItv; }

	uint32_t GenerateId() { return m_uGenerateId++; }

	virtual			void		OnLostDevice() override;
	virtual			void		OnResetDevice() override;

protected:
	uint32_t m_uGenerateId = 1;
	CTexture* m_pTexNormal;
	CTexture* m_pTexEdit;

	vector<VTXTEX>			m_vecVertices;
	vector<FACE32>			m_vecFaces;

	bool	m_bEdit = false;
	float	m_fEditStrength = 1;
	float	m_fEditRadius = 5;

	int		m_iCntX = 10;
	int		m_iCntZ = 10;

	float	m_fItv = 1;

protected:
	virtual		void		Free() override;
};

END

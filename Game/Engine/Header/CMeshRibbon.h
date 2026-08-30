#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMeshRibbon :
    public CVIBuffer
{
protected:
	explicit CMeshRibbon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMeshRibbon(const CMeshRibbon& rhs);
	virtual ~CMeshRibbon();

public:
	virtual _int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component() {}

	HRESULT			Ready_Buffer() override;
	void			Render_Buffer() override;

	virtual			void		OnLostDevice() override;
	virtual			void		OnResetDevice() override;

public:
	void			Append_Point(const ControlPoint& _cp);
	
	void			Update_Wave();

	void			Delete_Tail();

private:
	void			Append_MeshSegment();
	void			Append_Line();
	void			Append_Quad();
	
	void			Delete_Line();
	void			Delete_Quad();

public:
	deque<ControlPoint>& Get_ControlPoints() { return m_deqControlPoint; }
	void			Set_RibbonType(RIBBONTYPE _eID) { m_eRibbonType = _eID; }
	void			Set_QuadCopy(bool _b) { m_bQuadCopy = _b; }

protected:
	deque<ControlPoint>		m_deqControlPoint;
	deque<VTXTEX>			m_deqVertices;
	deque<FACE32>			m_deqFaces;
	
	int				m_iReserveSize = 2000;
	RIBBONTYPE		m_eRibbonType;
	bool			m_bQuadCopy = true;

public:
	static			CMeshRibbon* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			CComponent* Clone(); //PURE로 설정

protected:
	virtual		void		Free() override;
};

END
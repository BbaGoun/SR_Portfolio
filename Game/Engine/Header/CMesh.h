#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CMesh :
	public CComponent
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	HRESULT LoadMesh(const WCHAR* path);

public:
	virtual HRESULT		Ready_Mesh();
	virtual void		Render_Mesh();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

	DWORD						m_dwVtxSize;
	DWORD						m_dwVtxCnt;
	DWORD						m_dwTriCnt;
	DWORD						m_dwFVF;

	LPDIRECT3DVERTEXDECLARATION9 m_pVtxDecl;
	
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	DWORD						m_dwIdxCnt;
	D3DFORMAT					m_IdxFmt;

	LPDIRECT3DTEXTURE9 m_pBaseColorMap;

public:
	virtual CComponent* Clone() override;

protected:
	virtual void		Free() override;
};

END


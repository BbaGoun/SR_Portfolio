#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPskFile :
	public CVIBuffer
{
protected:
	explicit CPskFile();
	explicit CPskFile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPskFile(const CPskFile& rhs);
	virtual ~CPskFile();

public:
	HRESULT LoadPsk(const WCHAR* pPskPath);

public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

	const vector<PSKBone>&		GetBones() const { return m_vecBones; }
	const vector<PSKMaterial>&	GetMaterials() const { return m_vecMaterials; }

public:
	static  CPskFile*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const WCHAR* pPskPath);
	virtual CComponent* Clone() override;

protected:
	map<uint8_t, LPDIRECT3DTEXTURE9>		m_mapTextures;
	vector<PSKSubset>						m_vecSubsets;

	vector<PSKBone>							m_vecBones;
	vector<PSKMaterial>						m_vecMaterials;

protected:
	virtual void		Free() override;
};

END

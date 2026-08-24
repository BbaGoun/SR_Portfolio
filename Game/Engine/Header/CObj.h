#pragma once
#include "CComponent.h"

BEGIN(Engine)

// .obj(Wavefront) 모델을 처리하는 기반 컴포넌트.
// CMesh 와 동일한 역할(정점/인덱스 버퍼 + 텍스처 소유)을 하되,
// 파일 포맷만 .obj/.mtl 을 파싱한다.
class ENGINE_DLL CObj :
	public CComponent
{
protected:
	explicit CObj();
	explicit CObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CObj(const CObj& rhs);
	virtual ~CObj();

public:
	// .obj 를 파싱해 VB/IB 를 만들고, 같은 폴더의 .mtl(map_Kd)로 텍스처까지 로드한다.
	HRESULT				LoadObj(const WCHAR* pObjPath, _vec3 vAngle = { 0, 0, 0 });

public:
	virtual HRESULT		Ready_Obj();
	virtual void		Render_Obj();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	DWORD						m_dwVtxSize;
	DWORD						m_dwVtxCnt;
	DWORD						m_dwTriCnt;
	DWORD						m_dwFVF;

	DWORD						m_dwIdxCnt;
	D3DFORMAT					m_IdxFmt;

	LPDIRECT3DTEXTURE9			m_pBaseColorMap;

private:
	// .obj 가 참조하는 .mtl 을 열어 map_Kd 텍스처를 로드한다.
	HRESULT				LoadMaterialTexture(const string& strDir, const string& strMtlFile);

public:
	virtual CComponent* Clone() override;

protected:
	virtual void		Free() override;
};

END

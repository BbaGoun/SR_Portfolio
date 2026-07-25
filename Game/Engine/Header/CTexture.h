#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTexture :  public CComponent
{
private:
	explicit CTexture();
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT		Ready_Texture(TEXTUREID eID, const _tchar* pPath, const _uint& iCnt);
	void		Set_Texture(const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;

public:
	static		CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev,
								 TEXTUREID eID, 
								 const _tchar* pPath, 
								 const _uint& iCnt = 1);

	virtual		CComponent* Clone();

private:
	virtual		void		Free();
};

END


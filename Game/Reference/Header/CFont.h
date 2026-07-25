#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFont :  public CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFont();

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXSPRITE				m_pSprite;		// 2d 텍스처 출력 담당
	LPD3DXFONT					m_pFont;		// 폰트 출력 담당

public:
	HRESULT			Ready_Font(const _tchar* pFontType,
								const _uint& iWidth,
								const _uint& iHeight,
								const _uint& iWeight);

	void			Render_Font(const _tchar* pString, 
								const _vec2* pPos,
								D3DXCOLOR Color);


public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev,
						const _tchar* pFontType, 
						const _uint& iWidth, 
						const _uint& iHeight, 
						const _uint& iWeight);

private:
	virtual void		Free();

};

END
#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

enum GAME_UI {GAMEUI_SPEEDBAR, GAMEUI_ITEMICON, GAMEUI_USERNAME, GAMEUI_END};

class ENGINE_DLL CGameUI :  public CBase
{

protected:
	explicit CGameUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameUI();

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXSPRITE			m_pSprite;		// 2d 텍스처 출력 담당
	LPD3DXFONT				m_pFont;		// 폰트 출력 담당

private:
	HRESULT					Ready_UI(GAME_UI _eID, CGameUI* _pGameUI);



private:
	void					MakeOrthographicMatrix(_matrix* pOut,
															_float& fWidth,
															_float& fHeight,
															_float& fNear,
															_float& fFar);


public:
	static CGameUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);


private:
	CGameUI* m_GameUIs[GAMEUI_END];


private:
	virtual void	Free();

};
END
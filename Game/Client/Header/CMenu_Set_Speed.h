#pragma once
#include "CScene.h"

#include "Engine_Enum.h"

class CMenu_Set_Speed : public CScene
{
private:
	explicit CMenu_Set_Speed(LPDIRECT3DDEVICE9 pGraphicDev);
	~CMenu_Set_Speed() override;

public:
	virtual			HRESULT		Ready_Scene();
	virtual			HRESULT		PostReady_Scene() override;
	virtual			_int		Update_Scene(const _float& fDeltaTime);
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime);
	virtual			void		Render_Scene();

private:
	HRESULT		LoadSceneFromFile();

	HRESULT		Ready_Prototype();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer();
	HRESULT		Ready_RenderTarget();
public:
	void SetChar(CGameObject* pChar) { m_pChar = pChar; }
	void SetBG(CGameObject* pBG) { m_pBG = pBG; }


public:
	void		Selected_Slot(CGameObject* pSlot) { pSlot = m_pSlots; };
	void		Select_Cart();



private:

	BACKGROUND		m_eBGTexture;
	BACKGROUND		m_eNextBGTexture;

public:
	static CMenu_Set_Speed* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	//vector<CScene3_CharSlot*> m_vSlots;

	CGameObject* m_pChar;
	CGameObject* m_pBG;

	CGameObject* m_pSlots;

private:
	virtual void Free();
};
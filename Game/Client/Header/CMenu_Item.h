#pragma once
#include "CScene.h"

#include "Engine_Enum.h"

class CMenu_Item : public CScene
{
private:
	explicit CMenu_Item(LPDIRECT3DDEVICE9 pGraphicDev);
	~CMenu_Item() override;

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fDeltaTime);
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime);
	virtual			void		Render_Scene();

private:
	HRESULT		Ready_Prototype();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer();
	


private:

	BACKGROUND		m_eBGTexture;
	BACKGROUND		m_eNextBGTexture;

public:
	static CMenu_Item* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};
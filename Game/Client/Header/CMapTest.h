#pragma once

#include "CScene.h"

class CMapTest : public CScene
{
private:
	explicit CMapTest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMapTest();

public:
	virtual HRESULT Ready_Scene();
	virtual void FixedUpdate_Scene(const _float& fFixedDeltaTime);
	virtual _int Update_Scene(const _float& fDeltaTime);
	virtual void LateUpdate_Scene(const _float& fDeltaTime);
	virtual void Render_Scene();

private:
	HRESULT Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT Ready_UI_Layer(const _tchar* pLayerTag);

private:
	HRESULT Ready_Prototype();

public:
	static CMapTest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};
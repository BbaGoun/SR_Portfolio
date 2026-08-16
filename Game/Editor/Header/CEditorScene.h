#pragma once
#include "CScene.h"
#include "CWindow.h"

class CEditorScene :
	public CScene
{
private:
	explicit CEditorScene(LPDIRECT3DDEVICE9 pGraphicDev);
	~CEditorScene() override;

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_Scene(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime) override;
	virtual			void		Render_Scene() override;
	virtual			void		InvalidateDeviceObjects() override;

public:
	static CEditorScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT	Ready_Prototype();
	HRESULT Ready_Windows();
	HRESULT Ready_Layers();

private:
	list<CWindow*>			m_windowList;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

protected:
	virtual void			Free() override;
};


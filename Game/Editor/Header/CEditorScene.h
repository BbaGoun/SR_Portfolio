#pragma once
#include "CScene.h"
#include "CWindow.h"
#include "Engine_Parsing.h"

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
	virtual			void		OnLostDevice() override;

public:
	static CEditorScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT	Ready_Prototype();
	HRESULT Ready_Windows();
	HRESULT Ready_Layers();

	void OnNewScene();

	void OnLoad();
	bool OpenLoadSceneDialog(_tchar* outPath, DWORD outChars);
	void LoadSceneFile(const _tchar* path, CScene* pScene);
	CGameObject* LoadGameObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev,
		CGameObject* pParent, bool bRoot, CScene* pScene);
	
	void OnSave(bool bSaveAs);
	bool OpenSaveSceneDialog(_tchar* outPath, DWORD outChars);
	void SaveSceneFile(const _tchar* path);
	void SaveGameObject(FILE* pf, CGameObject* pObj, int depth, bool bRoot);

	void InputShortCut();
	void DoUndo();
	void DoRedo();

	void SetupDockLayout();

private:
	list<CWindow*>			m_windowList;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
protected:
	virtual void			Free() override;
};


#pragma once
#include "Engine_Define.h"
#include "Engine_Parsing.h"
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CEditorLoader : public CBase
{
	DECLARE_SINGLETON(CEditorLoader)

private:
	explicit CEditorLoader();
	virtual ~CEditorLoader();

public:
	void LoadEditorSceneFile(const _tchar* path, LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CGameObject* LoadEditorGameObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pParent, bool bRoot);

	void Free() override;
};

END
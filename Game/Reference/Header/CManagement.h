#pragma once

#include	"CBase.h"
#include	"CScene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement();
	virtual		~CManagement();

public:
	CComponent* Get_Component(COMPONENTID eID,
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		const _tchar* pComponentTag);

	void		Add_GameObject(
		const _tchar* pLayerTag,
		const _tchar* pObjTag,
		CGameObject*  pGameObject
	);

public:
	HRESULT			Set_Scene(CScene* pScene);
	void			FixedUpdate_Scene(const _float& fFixedDeltaTime);
	_int			Update_Scene(const _float& fDeltaTime);
	void			LateUpdate_Scene(const _float& fDeltaTime);
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*					m_pScene;

public:
	virtual void			Free();
};

END
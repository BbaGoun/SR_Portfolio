#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*		Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag);
	HRESULT			Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	HRESULT			Delete_GameObject(const _tchar* pObjTag);

public:
	HRESULT			Ready_Layer();
	void			FixedUpdate_Layer(const _float& fFixedDeltaTime);
	_int			Update_Layer(const _float& fDeltaTime);
	void			LateUpdate_Layer(const _float& fDeltaTime);
	void			Render_Layer();

private:
	map<const _tchar*, CGameObject*>			m_mapObject;

public:
	static CLayer* Create();

private:
	virtual void	Free();
};

END
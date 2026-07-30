#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CMay.h"
#include "CTexture.h"
#include "CCamera.h"

class CGOMay :
	public CGameObject
{
private:
	explicit CGOMay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGOMay(const CGameObject& rhs);
	~CGOMay() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

private:
	void	Follow(const _float& fTimeDelta);

private:
	CMay* m_pBufferCom;
	CCamera* m_pCameraCom;

public:
	static CGOMay* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

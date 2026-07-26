#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CCody.h"
#include "CCamera.h"

class CGOCody :
	public CGameObject
{
private:
	explicit CGOCody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGOCody(const CGameObject& rhs);
	~CGOCody() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

private:
	void	Key_Input(const _float& fTimeDelta);

private:
	CTransform* m_pTransformCom;
	CCody* m_pBufferCom;
	CCamera* m_pCameraCom;

	_float m_fSpeed;

public:
	static CGOCody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

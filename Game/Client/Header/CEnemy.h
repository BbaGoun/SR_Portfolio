#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CPyramid.h"
#include "CTexture.h"

class CEnemy :
	public CGameObject
{
private:
	explicit CEnemy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnemy(const CGameObject& rhs);
	~CEnemy() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

private:
	void	Follow(const _float& fTimeDelta);

private:
	CTransform* m_pTransformComp;
	CPyramid* m_pBufferCom;
	CTexture* m_pTextureCom;

public:
	static CEnemy* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

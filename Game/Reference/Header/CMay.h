#pragma once
#include "CPskFile.h"

BEGIN(Engine)

class ENGINE_DLL CMay :
	public CPskFile
{
protected:
	explicit CMay();
	explicit CMay(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMay(const CMay& rhs);
	virtual ~CMay() override;

public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

public:
	static	CMay* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

protected:
	virtual void		Free() override;
};

END

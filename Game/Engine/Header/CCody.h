#pragma once
#include "CPskFile.h"

BEGIN(Engine)

class ENGINE_DLL CCody :
    public CPskFile
{
protected:
	explicit CCody();
	explicit CCody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCody(const CCody& rhs);
	virtual ~CCody() override;

public:
	virtual HRESULT		Ready_Buffer() override;
	virtual void		Render_Buffer() override;

public:
	static	CCody* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

protected:
	virtual void		Free() override;
};

END

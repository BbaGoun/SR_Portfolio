#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMissileTex : public CVIBuffer
{
protected:
	explicit CMissileTex();
	explicit CMissileTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMissileTex(const CMissileTex& rhs);
	virtual ~CMissileTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	static CMissileTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void	Free();
};

END

#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMagnetTex : public CVIBuffer
{
protected:
	explicit CMagnetTex();
	explicit CMagnetTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMagnetTex(const CMagnetTex& rhs);
	virtual ~CMagnetTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	static CMagnetTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void	Free();
};

END

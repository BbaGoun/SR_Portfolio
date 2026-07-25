#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	explicit	CKeyMgr();
	virtual		~CKeyMgr();

public:
	bool KeyPressing(int iKey);
	bool KeyDown(int iKey);
	bool KeyUp(int iKey);
	void Key_Update();
	void Key_LateUpdate();

private:
	static const int VK_MAX = 0xff;
	bool m_bGetAsyncKey[VK_MAX];
	bool m_bKeyState[VK_MAX];
	bool m_bKeyNextState[VK_MAX];

	// CBase을(를) 통해 상속됨
	void Free() override;
};

END
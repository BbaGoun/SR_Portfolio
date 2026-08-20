#include "pch.h"
#include "CWindow.h"

CWindow::CWindow()
	:m_pGraphicDev(nullptr), m_bClone(false)
{
}

CWindow::CWindow(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

CWindow::CWindow(const CWindow& rhs)
	:m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true)
{
	m_pGraphicDev->AddRef();
}

CWindow::~CWindow()
{
}

void CWindow::Free() 
{
	Safe_Release(m_pGraphicDev);
}
#include "pch.h"
#include "CGOCody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CKeyMgr.h"
#include "CCameraMgr.h"

CGOCody::CGOCody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CGOCody::CGOCody(const CGameObject& rhs) : CGameObject(rhs)
{
}

CGOCody::~CGOCody()
{
}

HRESULT CGOCody::Ready_GameObject()
{
	m_fSpeed = 10;

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CCody*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Cody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = static_cast<CTransform*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Transform"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 0.05f, 0.05f, 0.05f };

	pComponent = m_pCameraCom = static_cast<CCamera*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Camera"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Camera", pComponent });
	CCameraMgr::GetInstance()->AddCamera();
	m_pCameraCom->SetCameraID(CAM_CODY);

	return S_OK;
}

_int CGOCody::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	return CGameObject::Update_GameObject(fTimeDelta);
}

void CGOCody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pCameraCom->SetCamera_BeforeRender();
}

void CGOCody::Render_GameObject()
{
	D3DXMATRIX matWorld;

	matWorld = m_pTransformCom->m_matWorld;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	for (int i = 0; i < CAM_GLOBAL; ++i)
	{
		switch (i) {
		case 0:
			m_pGraphicDev->SetViewport(&g_LeftView);
			break;
		case 1:
			m_pGraphicDev->SetViewport(&g_RightView);
			break;
		}
		CameraInfo camInfo = CCameraMgr::GetInstance()->GetCameraInfo(i);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &camInfo.matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &camInfo.matProj);
		m_pBufferCom->Render_Buffer();
	}
}

void CGOCody::Key_Input(const _float& fTimeDelta)
{
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	if (GetAsyncKeyState('A')) {
		m_pTransformCom->Rotate(QUATER_ROLL, 180 * fTimeDelta);
	}
	if (GetAsyncKeyState('D')) {
		m_pTransformCom->Rotate(QUATER_ROLL, -180 * fTimeDelta);
	}

	if (GetAsyncKeyState('Q')) {
		m_pTransformCom->Rotate(QUATER_YAW, -180 * fTimeDelta);
	}
	if (GetAsyncKeyState('E')) {
		m_pTransformCom->Rotate(QUATER_YAW, 180 * fTimeDelta);
	}

	if (GetAsyncKeyState('W')) {
		m_pTransformCom->Rotate(QUATER_PITCH, -180 * fTimeDelta);
	}
	if (GetAsyncKeyState('S')) {
		m_pTransformCom->Rotate(QUATER_PITCH, 180 * fTimeDelta);
	}

	if (GetAsyncKeyState(VK_UP)) {
		m_pCameraCom->Rotate(QUATER_PITCH, -180 * fTimeDelta);
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		m_pCameraCom->Rotate(QUATER_PITCH, 180 * fTimeDelta);
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		m_pCameraCom->Rotate(QUATER_YAW, -180 * fTimeDelta);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		m_pCameraCom->Rotate(QUATER_YAW, 180 * fTimeDelta);
	}

	if (GetAsyncKeyState(VK_SPACE)) {
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), m_fSpeed, fTimeDelta);
	}

	if (CKeyMgr::GetInstance()->KeyPressing(VK_LSHIFT)) {
		m_fSpeed = 20;
		float fov = m_pCameraCom->m_fFov;
		m_pCameraCom->m_fFov = min(90, fov + (90 - fov) * 0.1);
	}
	if (!CKeyMgr::GetInstance()->KeyPressing(VK_LSHIFT)) {
		m_fSpeed = 10;
		float fov = m_pCameraCom->m_fFov;
		m_pCameraCom->m_fFov = max(60, fov - (fov - 60) * 0.1);
	}
}

CGOCody* CGOCody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGOCody* pObj = new CGOCody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CGOCody::Free()
{
	CGameObject::Free();
}

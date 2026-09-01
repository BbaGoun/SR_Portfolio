#include "pch.h"
#include "CMinimapGround.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CMinimapGround::CMinimapGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMinimapGround::CMinimapGround(const CMinimapGround& rhs)
	:CGameObject(rhs)
{
}

CMinimapGround::~CMinimapGround()
{
}

HRESULT CMinimapGround::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Pos({ -630,0,-340 });
	m_pTransformCom->Set_Scale({ 2300,2625,1 }); // 일단 스카이박스 크기로 설정
	
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(180), D3DXToRadian(90), 0);
	m_pTransformCom->Set_Quaternion(&q);

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MinimapGround"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });


	return S_OK;
}

void CMinimapGround::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CMinimapGround::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderTargetGroup(L"Minimap", this);
	_vec3 vPos, vScale;
	vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (GetAsyncKeyState('W'))
		vPos.z += 10.f;
	if (GetAsyncKeyState('A'))
		vPos.x -= 10.f;
	if (GetAsyncKeyState('S'))
		vPos.z -= 10.f;
	if (GetAsyncKeyState('D'))
		vPos.x += 10.f;

	if (GetAsyncKeyState('Z'))
		vScale.x += 10.f;
	if (GetAsyncKeyState('X'))
		vScale.x -= 10.f;
	if (GetAsyncKeyState('C'))
		vScale.y += 10.f;
	if (GetAsyncKeyState('V'))
		vScale.y -= 10.f;

	
	m_pTransformCom->Set_Pos(vPos);
	m_pTransformCom->Set_Scale(vScale);

	//cout << "x: " << vPos.x << "\ty: " << vPos.y << "\tz: " << vPos.z << endl;
	//cout << "Scale.x: " << vScale.x << "\tScale.y: " << vScale.y << endl;
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMinimapGround::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMinimapGround::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture(0);
	m_pVIBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

CMinimapGround* CMinimapGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMinimapGround* pObj = new CMinimapGround(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMinimapGround Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CMinimapGround::Free()
{
	CGameObject::Free();
}
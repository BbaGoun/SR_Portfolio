#include "pch.h"
#include "CSkyBox.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CSkyBox::CSkyBox(const CGameObject& rhs) : CGameObject(rhs)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 500, 500, 500 });

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CInnerBox*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_InnerBox"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SkyBoxTexture"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	return CGameObject::Update_GameObject(fTimeDelta);
}

void CSkyBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CCamera* p_Camera = CCameraMgr::GetInstance()->GetMainCamera();
	_vec3 pos;
	p_Camera->Get_Transform()->Get_Info(INFO_POS, &pos);
	m_pTransformCom->Set_Pos(pos);
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSkyBox::Render_GameObject()
{
	D3DXMATRIX* matWorld;

	matWorld = m_pTransformCom->Get_World();

	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pBufferCom->Render_Buffer();
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox* pObj = new CSkyBox(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CSkyBox::Free()
{
	CGameObject::Free();
}

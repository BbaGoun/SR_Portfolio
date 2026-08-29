#include "pch.h"
#include "CWheel.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CCart.h"
#include "CLand3.h"
#include "CSkidMark.h"

CWheel::CWheel(LPDIRECT3DDEVICE9 pGraphicDev, WHEEL_TYPE eType)
	:CGameObject(pGraphicDev),m_eWheelType(eType)
{
}

CWheel::CWheel(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CWheel::~CWheel()
{
}

HRESULT CWheel::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	switch (m_eWheelType)
	{
	case Engine::WHEEL_FL:
		m_pTransformCom->Set_Pos({-2.5f ,1, 6 });
		break;
	case Engine::WHEEL_FR:
		m_pTransformCom->Set_Pos({ 2.5f,1, 6 });
		break;
	case Engine::WHEEL_BL:
		m_pTransformCom->Set_Pos({ -2.5f,1,0 });
		break;								
	case Engine::WHEEL_BR:					
		m_pTransformCom->Set_Pos({ 2.5f ,1,0 });
		break;
	case Engine::WHEEL_END:
		break;
	default:
		break;
	}

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartWheelCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartWheelCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartWheelTex");
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Tex", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	
	m_vColliderSize = { 0.4f,1.f,1.f };
	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents(m_vColliderSize);
	m_mapComponent.insert({ L"Com_Collider", pComponent });


	m_fScale = 1.f;
	m_fRayMinDist = 0.99f;

	return S_OK;
}

void CWheel::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	m_pColliderCom->Set_Extents(m_vColliderSize * m_fScale);

	CCart* pCart = dynamic_cast<CCart*>(m_pParent->Get_Parent());
	_vec3 vParentForce = pCart->Get_Force();
	float fParentForceLen = D3DXVec3Length(&vParentForce);

	_vec3 vPlayerLook;
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vPlayerLook);

	if (D3DXVec3Dot(&vPlayerLook, &vParentForce) >= 0)
		m_vRotation.x += fParentForceLen * fFixedDeltaTime;
	else
		m_vRotation.x -= fParentForceLen * fFixedDeltaTime;

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
	m_pTransformCom->Set_Quaternion(&q);

	if (m_eWheelType < WHEEL_BL)
		return;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	_vec3 originPos = vPos;
	if (pCart->GetDrift())
	{
		_vec3 vDeltaPos;
		vDeltaPos = vPos - m_vPrePos;
		m_fDistSum += D3DXVec3Length(&vDeltaPos);
		if (m_fDistSum >= 0.01f && CheckInTerrain())
		{
			CreateSkidMark();
			m_fDistSum = 0;
		}
	}
	else
	{
		m_fDistSum = 0;
	}
	m_vPrePos = vPos;
}

_int CWheel::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	KeyInput(fDeltaTime);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CWheel::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CWheel::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

void CWheel::KeyInput(const _float& fDeltaTime)
{
	_vec3 vParentForce = m_pParent->Get_Parent()->Get_Force();
	float fParentForceLen = D3DXVec3Length(&vParentForce);
	if (fParentForceLen > 5.0f)
	{
		m_vRotation.y = 0;
		return;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT)&& m_eWheelType < WHEEL_BL)
	{
		m_vRotation.y = -45;
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT) && m_eWheelType < WHEEL_BL)
	{
		m_vRotation.y = 45;
	}
	else
	{
		m_vRotation.y = 0;
	}
}

void CWheel::ResetPrePos()
{
	m_pTransformCom->Get_Info(INFO_POS, &m_vPrePos);
}

void CWheel::CreateSkidMark()
{
	CGameObject* pGameObject = CSkidMark::Create(m_pGraphicDev);
	
	if (nullptr == pGameObject)
		return;

	CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"Proto_SkidMark", pGameObject);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y -= m_fRayMinDist - 0.01f;

	pGameObject->Get_Transform()->Set_Pos(vPos);

	//CCartBody의 WorldQuaternion을 가져옴
	D3DXQUATERNION q = m_pParent->Get_Parent()->Get_Transform()->Get_WorldQuaternion();
	pGameObject->Get_Transform()->Multiple_Quaternion(&q);
}

bool CWheel::CheckInTerrain()
{
	auto& tracks = CManagement::GetInstance()->Find_GameObjectsByTag(L"GameLogic", L"Track");
	if (tracks.empty())
		return false;
	// for문 밖에 생성
	_vec3 vWheelWorldCenter = ToVec3(m_pColliderCom->Get_Info().Center);
	_vec3 vWheelModelCenter;

	m_fRayMinDist = FLT_MAX;
	bool  bFind = false;
	// 지형들 중 어떤 지형과 충돌했는지 확인 후 fGroundY, m_vTerrainNormal값이 구해짐
	for (auto& track : tracks) {
		CSpline* pSpline = track->Get_Component<CSpline>();
		DirectX::BoundingBox box = *pSpline->GetBoundingBox();

		// spline의 모델 스페이스로 보내기 위한 역행렬
		_matrix matTrack, matInvTrack;
		matTrack = *track->Get_Transform()->Get_World();
		D3DXMatrixInverse(&matInvTrack, 0, &matTrack);

		// 플레이어의 박스 콜라이더를 spline의 모델 스페이스로 보낸다.
		// 박스 콜라이더의 center를 변환해서 다시 넣는 방식
		D3DXVec3TransformCoord(&vWheelModelCenter, &vWheelWorldCenter, &matInvTrack);
		m_pColliderCom->Set_Center(vWheelModelCenter);

		// 트랙의 boundingbox와 플레이어의 콜라이더가 닿는지 검사
		bool bCheckCollision = box.Intersects(m_pColliderCom->Get_Info());
		m_pColliderCom->Set_Center(vWheelWorldCenter);
		if (bCheckCollision == false)
			continue;
		// 충돌한 지형을 찾았다면 이제 spline이 갖고 있는 면에 대해서 raycast로 지형에있는 평면 하나 찾기
		vector<VTXTEX> vecVertices = pSpline->GetVertices();
		vector<FACE32> vecFaces = pSpline->GetFaces();

		D3DXVECTOR3 vRayPos = { vWheelModelCenter.x, vWheelModelCenter.y, vWheelModelCenter.z };

		D3DXVECTOR3 vRayDir = { 0.f, -1.f, 0.f };
		D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matInvTrack);

		//cout << vRayDir.x << "\t" << vRayDir.y << "\t" << vRayDir.z << endl;
		int a = 0;
		for (int i = 0; i < vecFaces.size(); ++i)
		{
			_vec3 p0 = vecVertices[vecFaces[i].indices._0].vPosition;
			_vec3 p1 = vecVertices[vecFaces[i].indices._1].vPosition;
			_vec3 p2 = vecVertices[vecFaces[i].indices._2].vPosition;

			float u, v, fDist;
			if (!D3DXIntersectTri(&p0, &p1, &p2, &vRayPos, &vRayDir, &u, &v, &fDist))
			{
				continue;
			}

			if (fDist >= m_fRayMinDist)
				continue;
			m_fRayMinDist = fDist;
			bFind = true;
		}
	}
	if (bFind)
		return true;
	else
		return false;
}

CWheel* CWheel::Create(LPDIRECT3DDEVICE9 pGraphicDev,WHEEL_TYPE eType)
{
	CWheel* pObj = new CWheel(pGraphicDev, eType);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWheel_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CWheel::Free()
{
	CGameObject::Free();
}

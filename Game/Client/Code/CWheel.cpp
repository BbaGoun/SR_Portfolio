#include "pch.h"
#include "CWheel.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CCart.h"
#include "CLand3.h"
#include "CSkidMark.h"
#include "CCartBot.h"

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

	m_fCartForceLen = 0.f;
	m_eCartDirection = DIR_FORWARD;
	m_eWheelTurn = TURN_END;
	m_fDistSum = 0.f;

	Set_CollisionLayer(CL_CART_WHEEL);

	return S_OK;
}

void CWheel::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	m_pColliderCom->Set_Extents(m_vColliderSize * m_fScale);
  
	if (CCart* pCart = dynamic_cast<CCart*>(m_pParent->Get_Parent())) {
    if (m_eCartDirection == DIR_FORWARD)
      m_vRotation.x += m_fCartForceLen * fFixedDeltaTime;
    else
		  m_vRotation.x -= m_fCartForceLen * fFixedDeltaTime;

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
			if (m_fDistSum >= 0.5f && CheckInTerrain())
			{
				m_fDistSum = 0;

				CreateSkidMark();
				CreateDriftTrail();
			}
		}
		else
		{
			m_pSkidMark = nullptr;
			m_pDriftTrail = nullptr;
			m_fDistSum = 0;
		}
		m_vPrePos = vPos;
	}
	
	else if (CCartBot* pCartBot = dynamic_cast<CCartBot*>(m_pParent->Get_Parent())) {
    if (m_eCartDirection == DIR_FORWARD)
      m_vRotation.x += m_fCartForceLen * fFixedDeltaTime;
    else
		  m_vRotation.x -= m_fCartForceLen * fFixedDeltaTime;

		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		if (m_eWheelType < WHEEL_BL)
			return;

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		_vec3 originPos = vPos;
		if (pCartBot->GetDrift())
		{
			_vec3 vDeltaPos;
			vDeltaPos = vPos - m_vPrePos;
			m_fDistSum += D3DXVec3Length(&vDeltaPos);
			if (m_fDistSum >= 0.5f && CheckInTerrain())
			{
				m_fDistSum = 0;

				CreateSkidMark();
				CreateDriftTrail();
			}
		}
		else
		{
			m_pSkidMark = nullptr;
			m_pDriftTrail = nullptr;
			m_fDistSum = 0;
		}
		m_vPrePos = vPos;
	}
}

_int CWheel::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	UpdateWheelRot(fDeltaTime);
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

void CWheel::UpdateWheelRot(const _float& fDeltaTime)
{
	if (m_fCartForceLen > 5.0f)
	{
		m_vRotation.y = 0;
		return;
	}
	if (m_eWheelTurn == TURN_LEFT && m_eWheelType < WHEEL_BL)
		m_vRotation.y = D3DXToRadian(-45);
	else if (m_eWheelTurn == TURN_RIGHT && m_eWheelType < WHEEL_BL)
		m_vRotation.y = D3DXToRadian(45);
	else
		m_vRotation.y = 0;
}

void CWheel::ResetPrePos()
{
	m_pTransformCom->Get_Info(INFO_POS, &m_vPrePos);
}

void CWheel::CreateSkidMark()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y -= m_fRayMinDist - 0.01f;

	if (!m_pSkidMark) {
		m_pSkidMark = CSkidMark::Create(m_pGraphicDev, vPos, this);
		if (m_pSkidMark != nullptr)
			CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"SkidMark", m_pSkidMark);
	}
	else {
		m_pSkidMark->Append_Point(vPos);
	}
}

void CWheel::CreateDriftTrail()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y -= m_fRayMinDist - 0.02f;

	if (!m_pDriftTrail) {
		m_pDriftTrail = CDriftTrail::Create(m_pGraphicDev, vPos, this);
		if (m_pDriftTrail != nullptr)
			CManagement::GetInstance()->Add_GameObject(L"GameLogic", L"Drift_Trail", m_pDriftTrail);
	}
	else {
		m_pDriftTrail->Append_Point(vPos);
	}
}

bool CWheel::CheckInTerrain()
{
	auto& tracks = CManagement::GetInstance()->Find_GameObjectsByTag(L"GameLogic", L"Track");
	if (tracks.empty())
		return false;

	DirectX::BoundingOrientedBox OBB = m_pColliderCom->Get_Info();

	// 계산에 쓰기 위해 벡터 준비
	_vec3 vWheelWorldCenter = ToVec3(OBB.Center);
	_quaternion qWheelWorld = ToQuaternion(OBB.Orientation);

	// 변환될 결과를 담을 벡터
	_vec3 vWheelModelCenter;
	_quaternion qWheelModel;

	bool	bFind = false;
	float	fMinRayDist = FLT_MAX;

	// 지형들 중 어떤 지형과 충돌했는지 확인 후 fGroundY, m_vTerrainNormal값이 구해짐
	for (auto& track : tracks) {
		CSpline* pSpline = track->Get_Component<CSpline>();
		DirectX::BoundingBox box = *pSpline->GetBoundingBox();

		// spline의 모델 스페이스로 보내기 위한 역행렬
		_matrix matTrack, matInvTrack;
		matTrack = *track->Get_Transform()->Get_World();
		D3DXMatrixInverse(&matInvTrack, 0, &matTrack);

		// OBB의 회전을 spline의 모델 스페이스로 보내기 위한 역 쿼터니언
		_quaternion qTrack, qInvTrack;
		qTrack = track->Get_Transform()->Get_WorldQuaternion();
		D3DXQuaternionInverse(&qInvTrack, &qTrack);

		// 플레이어의 박스 콜라이더를 spline의 모델 스페이스로 보낸다.
		// 박스 콜라이더의 Center/Orientation를 변환해서 다시 넣는 방식
		D3DXVec3TransformCoord(&vWheelModelCenter, &vWheelWorldCenter, &matInvTrack);
		qWheelModel = qWheelWorld * qInvTrack;
		OBB.Center = ToXMFLOAT3(vWheelModelCenter);
		OBB.Orientation = ToXMFLOAT4(qWheelModel);
		
		// 트랙의 boundingbox와 플레이어의 콜라이더가 닿는지 검사
		bool bCheckCollision = box.Intersects(OBB);
		if (bCheckCollision == false)
			continue;
		
		// 충돌한 지형을 찾았다면 이제 spline이 갖고 있는 면에 대해서 raycast로 지형에있는 평면 하나 찾기
		vector<VTXTEX> vecVertices = pSpline->GetVertices();
		vector<FACE32> vecFaces = pSpline->GetFaces();

		D3DXVECTOR3 vRayPos = { vWheelWorldCenter.x, vWheelWorldCenter.y, vWheelWorldCenter.z };

		D3DXVECTOR3 vRayDir = { 0.f, -1.f, 0.f };

		//cout << vRayDir.x << "\t" << vRayDir.y << "\t" << vRayDir.z << endl;
		for (int i = 0; i < vecFaces.size(); ++i)
		{
			_vec3 p0 = vecVertices[vecFaces[i].indices._0].vPosition;
			_vec3 p1 = vecVertices[vecFaces[i].indices._1].vPosition;
			_vec3 p2 = vecVertices[vecFaces[i].indices._2].vPosition;

			D3DXVec3TransformCoord(&p0, &p0, &matTrack);
			D3DXVec3TransformCoord(&p1, &p1, &matTrack);
			D3DXVec3TransformCoord(&p2, &p2, &matTrack);

			float u, v, fDist;
			if (!D3DXIntersectTri(&p0, &p1, &p2, &vRayPos, &vRayDir, &u, &v, &fDist))
				continue;

			if (fDist >= fMinRayDist)
				continue;
			fMinRayDist = fDist;
			bFind = true;
		}
	}
	if (bFind) {
		m_fRayMinDist = fMinRayDist;
		return true;
	}
	else
		return false;
}

void CWheel::ForgetDriftTrail(CDriftTrail* pDriftTrail)
{
	if (m_pDriftTrail == pDriftTrail)
		m_pDriftTrail = nullptr;
}

void CWheel::ForgetSkidMark(CSkidMark* pSkidMark)
{
	if (m_pSkidMark == pSkidMark)
		m_pSkidMark = nullptr;
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

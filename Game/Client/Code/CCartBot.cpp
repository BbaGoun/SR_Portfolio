#include "pch.h"
#include "CCartBot.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CRainBow_Cloud.h"
#include "CManagement.h"
#include "CBanana.h"
#include "CCollisionMgr.h"
#include "CMissile.h"
#include "CMissileBody.h"
#include "CLand3.h"
#include "CThunderCloud.h"
#include "CCartBody.h"
#include "CMagnetBody.h"
#include "CWaterBomb.h"
#include "CWaterBombBody.h"
#include "CWaterBombThrow.h"
#include "CWaterBombBubble.h"
#include "CWaterFly.h"
#include "CWaterFLyBody.h"
#include "CDustLandingEffect.h"
#include "SoundMgr.h"
#include "CUI_StartCountDown.h"
#include "CUI_EndCountDown.h"
#include "CShield1.h"
#include "CShield2.h"
#include "CPlayTimeMgr.h"
#include "CTrackMgr.h"
#include "CCalculator.h"
#include "CWheel.h"
#include "CFindOthersMgr.h"
#include "CBarricade.h"
#include "CMagnet.h"

CCartBot::CCartBot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_bDrift(false)
{
}

CCartBot::CCartBot(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCartBot::~CCartBot()
{
}

HRESULT CCartBot::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_vForce = { 0,0,0 };

	m_fSpeed = 1.f;
	m_fMaxSpeed = 3.f;

	m_bDrift = false;
	m_fLookForceAngle = 0.f;

	m_fBoostTurnAngle = 0.5f;
	m_fNormalTurnAngle = 0.8f;
	m_fDriftTurnAngle = 2.0f;

	m_bRainbowUI = false;
	// m_bBubbleUI				= false;
	m_bBanana = false;
	m_bThunder = false;
	m_bMagnet = false;
	m_bUseItem = false;

	m_fMagnetTimer = 0.f;
	m_fBananaTimer = 0.f;

	m_fCurGage = 0.f;
	m_fGainGage = 0.f;

	m_fBoostItemCnt = 0.f;

	m_fShortBoosterTimer = 0.f;

	m_eCartState = CART_STATE_GROUND;
	m_eBoostState = BOOST_STATE_NORMAL;
	m_vTerrainNormal = { 0,1,0 };
	m_fAirTime = 0.f;

	m_eFirstSlot = ITEM_END;
	m_eSecondSlot = ITEM_END;

	m_eDirection = DIR_FORWARD;
	m_iFlatFrameCnt = 0;
	m_vBananaSpinStartLook = { 0,0,0 };

	m_bCanShortBoost = true;
	m_bShortBoosterTimerOnOff = false;
	m_bPlaying = false;

	m_fPlayTimer = 0.f;
	m_fPreTimer = 0.f;

	m_pPlayerHead = nullptr;
	m_PreQuaternion = { 0,0,0,1 };

	m_PreQuaternion = { 0, 0, 0, 1 };

	m_bUpKey = false;

	return S_OK;
}

void CCartBot::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (m_bMissileHit == true)
		return;

	UpdateBubble(fFixedDeltaTime);
	if (m_bBubble == true)
		return;
	
	if (!CPlayTimeMgr::GetInstance()->GetPlaying()) {
		m_iCollisionTick = 0;
		return;
	}

	m_fOffsetTimer += fFixedDeltaTime;
	if (m_fOffsetTimer >= m_fOffsetTimerEnd) {
		m_fOffsetTimer = 0.f;
		m_fOffsetTimerEnd = 1.f + CCalculator::RandInt() / 99.f * 3.f;
		
		m_fLateralOffsetTarget = -0.5f + CCalculator::RandInt() / 99.f;
		m_fLateralOffsetTarget = clampT(m_fLateralOffsetTarget, -0.35f, 0.35f);
	}

	m_fLateralOffset = Lerp(fFixedDeltaTime, m_fLateralOffset, m_fLateralOffsetTarget);

	float lookAhead = clampT(D3DXVec3Length(&m_vForce) * m_fSpeed, 10.f, 50.f);

	TrackPose TP = CTrackMgr::GetInstance()->Compute_TargetPose(this, lookAhead, true);

	_vec3 vPos, vLook;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	if (TP.bValid && m_bActive) {
		if(TP.bDodge)
			TP.position += m_fLateralOffset * TP.R * TP.halfW;
		TP.position.y += 0.5f; // 카트가 박히지 않도록

		_vec3 dir = TP.position - vPos;
		D3DXVec3Normalize(&dir, &dir);

		_vec3 flatLook = vLook;
		_vec3 flatDir = dir;
		flatLook.y = 0;
		flatDir.y = 0;
		D3DXVec3Normalize(&flatLook, &flatLook);
		D3DXVec3Normalize(&flatDir, &flatDir);
		float dot = D3DXVec3Dot(&flatLook, &flatDir);

		_vec3 cross;
		D3DXVec3Cross(&cross, &flatLook, &flatDir);

		float yawError = atan2f(cross.y, dot);

		float maxYawSpeed = D3DXToRadian(180.f); // 초당 90도
		float maxYawStep = maxYawSpeed * fFixedDeltaTime;

		float yawStep =
			clampT(yawError, -maxYawStep, maxYawStep);

		if (fabsf(yawStep) >= D3DXToRadian(15.f) * fFixedDeltaTime) {
			if (yawStep >= 0) {
				m_vRotation.z = Lerp(fFixedDeltaTime, m_vRotation.z, D3DXToRadian(15.f));
				m_vRotation.z = clampT(m_vRotation.z, D3DXToRadian(-15.f), D3DXToRadian(15.f));
			}
			else {
				m_vRotation.z = Lerp(fFixedDeltaTime, m_vRotation.z, D3DXToRadian(-15.f));
				m_vRotation.z = clampT(m_vRotation.z, D3DXToRadian(-15.f), D3DXToRadian(15.f));
			}

			m_bDrift = true;
		}
		else {
			m_vRotation.z = Lerp(fFixedDeltaTime, m_vRotation.z, 0.f);
			m_vRotation.z = clampT(m_vRotation.z, D3DXToRadian(-15.f), D3DXToRadian(15.f));
			m_bDrift = false;
		}

		m_vRotation.y += yawStep;

		float acceleration = TP.speed - D3DXVec3Length(&m_vForce);

		if (acceleration > 0)
		{
			if (acceleration >= 20.f) {
				m_eBoostState = BOOST_STATE_LONG_BOOST;
				if (m_pPlayerHead)
					m_pPlayerHead->SetBoost(true);
			}
			else {
				m_eBoostState = BOOST_STATE_NORMAL;
				if (m_pPlayerHead)
					m_pPlayerHead->SetBoost(false);
			}

			m_vForce += dir * acceleration * fFixedDeltaTime;
		}
		else {
			m_eBoostState = BOOST_STATE_NORMAL;
			if (m_pPlayerHead)
				m_pPlayerHead->SetBoost(false);

			m_vForce *= clampT(1.f + acceleration * fFixedDeltaTime, 0.1f, 1.f);
		}

		_matrix matRotY;
		_vec3 vUp;
		m_pTransformCom->Get_Info(INFO_UP, &vUp);
		D3DXMatrixRotationAxis(&matRotY, &vUp, yawStep);
		D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRotY);
	}
	else {
		m_vForce *= 0.98f;
	}

	UpdateGravity();

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	m_pTransformCom->Set_Quaternion(&q);

	float fForceLen = D3DXVec3Length(&m_vForce);
	if (fForceLen >= 120.f)
		m_vForce = m_vForce / fForceLen * 120.f;
	SetWheelForceLen();

	for (int i = 0; i < 2; ++i) {
		m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed / 2.f, fFixedDeltaTime);
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		AdjustPosY_Slope(vPos, fFixedDeltaTime);
		if (!m_bCollisionWall)
			CollisionWall();
	}

	m_bCollisionWall = false;

	//CGameObject::FixedUpdate_GameObject(fFixedDeltaTime);
}

_int CCartBot::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	m_bPlaying = CPlayTimeMgr::GetInstance()->GetPlaying();

	if (!m_bActive) {
		m_bDrift = false;
		m_bUpKey = false;
		m_eBoostState = BOOST_STATE_NORMAL;
		return 0;
	}

	m_fItemTimer += fDeltaTime;
	if (m_fItemTimer > m_fItemTimerEnd) {
		m_fItemTimer = 0;
		m_fItemTimerEnd = CCalculator::RandInt() / 99.f * 3.f;
		UseItem();
	}

	UpdateMagnet(fDeltaTime);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCartBot::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

CCartBot* CCartBot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartBot* pObj = new CCartBot(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Cart Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCartBot::CreateCloudObject()
{
	CGameObject* pGameObject = CRainbow_Cloud::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Rainbow_Cloud", pGameObject)))
		return;

	TrackPose TP = CTrackMgr::GetInstance()->Compute_TargetPose(this, -10, false);

	if (!TP.bValid) {
		pGameObject->GetLayer()->Delete_GameObject(pGameObject);
		return;
	}

	TP.position.y += 10.f;

	pGameObject->Get_Transform()->Set_Pos(TP.position);

	_matrix	matRot;
	D3DXMatrixIdentity(&matRot);
	memcpy(&matRot.m[0], &TP.R, sizeof(_vec3));
	memcpy(&matRot.m[1], &TP.U, sizeof(_vec3));
	memcpy(&matRot.m[2], &TP.T, sizeof(_vec3));

	_quaternion q;
	D3DXQuaternionRotationMatrix(&q, &matRot);
	pGameObject->Get_Transform()->Set_Quaternion(&q);
}

void CCartBot::CreateBananaObject()
{
	CGameObject* pGameObject = CBanana::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Banana", pGameObject)))
		return;

	_vec3 vPos, vLook, vUp;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	vPos -= vLook * 10 - vUp;
	pGameObject->Get_Transform()->Set_Pos(vPos);
	CTrackMgr::GetInstance()->Register_Hazard(pGameObject, ITEM_BANANA);
}

void CCartBot::CreateThunderCloudObject()
{
	SoundMgr::GetInstance().PlaySound(L"Effect/Item_thunderbolt/ThunderCloud.ogg", SOUND_THUNDERCLOUD, 0.4f);
	vector<CGameObject*> vecOthers = CFindOthersMgr::GetInstance()->GetOtherCart(this);

	for (auto& pOther : vecOthers)
	{
		CGameObject* pGameObject = CThunderCloud::Create(m_pGraphicDev, pOther);

		if (nullptr == pGameObject)
			return;

		if (FAILED(m_pLayer->Add_GameObject(L"Obj_ThunderCloud", pGameObject)))
			return;

		_vec3 vRight, vLook, vPos;
		m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos += +vRight * 10 + _vec3({ 0,13,0 }) - vLook * 10;
		pGameObject->Get_Transform()->Set_Pos(vPos);

		pGameObject->SetLayer(m_pLayer);
	}
}

void CCartBot::AdjustPosY_Slope(_vec3 pos, const float fDeltaTime)
{
	auto& tracks = CManagement::GetInstance()->Find_GameObjectsByTag(L"GameLogic", L"Track");
	if (tracks.empty())
		return;

	CCube_Collider* pCol = Get_ComponentSpread<CCube_Collider>();

	// Reference가 아니라 값 복사 (변환을 하다보니 값 복사)
	DirectX::BoundingOrientedBox OBB = pCol->Get_Info();

	// 계산에 쓰기 위해 벡터 준비
	_vec3 vCartOldCenter = ToVec3(OBB.Center);
	_quaternion qCart = ToQuaternion(OBB.Orientation);
	_vec3 extends = ToVec3(OBB.Extents);

	// 변환될 결과를 담을 벡터
	_vec3 vCartModelCenter;
	_quaternion qCartModel;

	float fGroundY = 0.f;
	float fMinRayDist = FLT_MAX;
	bool bFind = false;
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
		D3DXVec3TransformCoord(&vCartModelCenter, &vCartOldCenter, &matInvTrack);
		qCartModel = qCart * qInvTrack;
		OBB.Center = ToXMFLOAT3(vCartModelCenter);
		OBB.Orientation = ToXMFLOAT4(qCartModel);

		// 트랙의 boundingbox와 플레이어의 콜라이더가 닿는지 검사
		bool bCheckCollision = box.Intersects(OBB);
		if (bCheckCollision == false)
			continue;

		// 충돌한 지형을 찾았다면 이제 spline이 갖고 있는 면에 대해서 raycast로 지형에있는 평면 하나 찾기
		vector<VTXTEX> vecVertices = pSpline->GetVertices();
		vector<FACE32> vecFaces = pSpline->GetFaces();

		D3DXVECTOR3 vRayPos = { vCartModelCenter.x, vCartModelCenter.y + 5.f, vCartModelCenter.z };
		D3DXVECTOR3 vRayDir = { 0.f, -1.f, 0.f };

		for (int i = 0; i < vecFaces.size(); ++i)
		{
			_vec3 p0 = vecVertices[vecFaces[i].indices._0].vPosition;
			_vec3 p1 = vecVertices[vecFaces[i].indices._1].vPosition;
			_vec3 p2 = vecVertices[vecFaces[i].indices._2].vPosition;

			float u, v, fDist;
			if (!D3DXIntersectTri(&p0, &p1, &p2, &vRayPos, &vRayDir, &u, &v, &fDist))
				continue;

			if (fDist >= fMinRayDist)
				continue;

			bFind = true;
			D3DXPLANE plane;
			D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

			float fLocalY = -(plane.a * vCartModelCenter.x + plane.c * vCartModelCenter.z + plane.d) / plane.b;

			_vec3 vLocalPos = { vCartModelCenter.x, fLocalY, vCartModelCenter.z };
			_vec3 vWorldPos;
			D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, &matTrack);
			fGroundY = vWorldPos.y;

			// 법선 구하기
			_vec3 vLocalNormal = { plane.a, plane.b, plane.c };
			_matrix matNormal;
			D3DXMatrixTranspose(&matNormal, &matInvTrack);
			D3DXVec3TransformNormal(&vLocalNormal, &vLocalNormal, &matNormal);
			D3DXVec3Normalize(&vLocalNormal, &vLocalNormal);
			m_vTerrainNormal = vLocalNormal;
			if (m_vTerrainNormal.y < 0)
				m_vTerrainNormal *= -1;
		}
	}
	// for문이 끝나면 fGroundY, m_vTerrainNormal값이 구해짐
	// 이후부터는 CartState갱신
	_vec3 vCartPos;
	m_pTransformCom->Get_Info(INFO_POS, &vCartPos);
	if (bFind)
	{
		m_bCollisionWall = true;
		float fDeltaY = vCartPos.y - fGroundY;
		// m_eCart_State 업데이트
		if (m_eCartState == CART_STATE_GROUND) // Ground 유지
		{
			if (fDeltaY < 0.09f)
			{
				m_fAirTime = 0.f;
				m_eCartState = CART_STATE_GROUND;
				m_pTransformCom->Set_Pos({ vCartPos.x, fGroundY, vCartPos.z });

				// 경사면에 맞게 카트 몸체 회전
				_vec3 vCartUp;
				m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
				float fRadian = acosf(clampT(D3DXVec3Dot(&vCartUp, &m_vTerrainNormal), -1.f, 1.f));

				_vec3 vAxis;
				D3DXVec3Cross(&vAxis, &vCartUp, &m_vTerrainNormal);

				D3DXQUATERNION q = { 0, 0, 0, 1 };

				if (D3DXVec3LengthSq(&vAxis) > FLT_EPSILON) {
					D3DXVec3Normalize(&vAxis, &vAxis);
					D3DXQuaternionRotationAxis(&q, &vAxis, fRadian);
				}
				int a;
				if (fabsf(m_vTerrainNormal.y) >= 0.999f)
				{
					++m_iFlatFrameCnt;
					if (m_iFlatFrameCnt > 3)
						m_PreQuaternion = { 0,0,0,1 };
				}
				else
				{
					m_PreQuaternion = q;
					m_iFlatFrameCnt = 0;
				}
			}
			else // 점프 시작 
			{
				m_eCartState = CART_STATE_AIR;
				m_fAirTime += fDeltaTime;
			}
		}
		else if (m_eCartState == CART_STATE_AIR) // 착지
		{
			if (fDeltaY <= 0.1f)
			{
				if (m_fAirTime > 0.3f)//공중에 떠있는 시간
					m_pDustLandingEffect->ResetParticle();

				m_fAirTime = 0.f;
				m_eCartState = CART_STATE_GROUND;
				m_pTransformCom->Set_Pos({ vCartPos.x, fGroundY, vCartPos.z });

				// 경사면에 맞게 카트 몸체 회전
				_vec3 vCartUp;
				m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
				float fRadian = acosf(D3DXVec3Dot(&vCartUp, &m_vTerrainNormal));

				_vec3 vAxis;
				D3DXVec3Cross(&vAxis, &vCartUp, &m_vTerrainNormal);

				D3DXQUATERNION q;
				D3DXQuaternionRotationAxis(&q, &vAxis, fRadian);

				if (fabsf(m_vTerrainNormal.y) >= 0.999f)
					m_PreQuaternion = { 0,0,0,1 };
				else
					m_PreQuaternion = q;
			}
			else // 점프 유지
			{
				m_eCartState = CART_STATE_AIR;
				m_fAirTime += fDeltaTime;
			}
		}
	}
	else //맵 전체를 지형으로 덮으면 else 부분은 필요 없을듯?
	{
		if (vCartOldCenter.y <= 0.f)
		{
			m_eCartState = CART_STATE_GROUND;
			m_pTransformCom->Set_Pos({ vCartOldCenter.x,0,vCartOldCenter.z });
			m_vTerrainNormal = { 0,1,0 };
			m_PreQuaternion = { 0,0,0,1 };
		}
		else
		{
			m_eCartState = CART_STATE_AIR;
			m_fAirTime += fDeltaTime;
			m_iFlatFrameCnt = 0;
		}
	}

	m_pTransformCom->Multiple_Quaternion(&m_PreQuaternion);
	if (m_eCartState == CART_STATE_AIR)
	{
		_vec3 vCartUp, vUp = { 0,1,0 };
		m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
		float fRadian = acosf(D3DXVec3Dot(&vCartUp, &vUp));

		_vec3 vAxis;
		D3DXVec3Cross(&vAxis, &vCartUp, &vUp);

		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &vAxis, fRadian * m_fAirTime);
		m_pTransformCom->Multiple_Quaternion(&q);
	}
}

void CCartBot::CollisionWall()
{
	auto& walls = CManagement::GetInstance()->Find_GameObjectsByTag(L"GameLogic", L"Wall");
	if (walls.empty())
		return;

	// 플레이어의 정보
	_vec3 vRight, vUp, vLook, vPos;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CCube_Collider* pCol = Get_ComponentSpread<CCube_Collider>();

	// Reference가 아니라 값 복사 (변환을 하다보니 값 복사)
	DirectX::BoundingOrientedBox OBB = pCol->Get_Info();

	// 계산에 쓰기 위해 벡터 준비
	_vec3 vCartOldCenter = ToVec3(OBB.Center);
	_quaternion qCart = ToQuaternion(OBB.Orientation);
	_vec3 extends = ToVec3(OBB.Extents);

	// 변환될 결과를 담을 벡터
	_vec3 vCartModelCenter;
	_quaternion qCartModel;

	// 벽들 중 어떤 벽과 충돌했는지 확인
	for (auto& wall : walls) {
		CSpline* pSpline = wall->Get_Component<CSpline>();
		DirectX::BoundingBox box = *pSpline->GetBoundingBox();

		// OBB의 중심을 spline의 모델 스페이스로 보내기 위한 역행렬
		_matrix matTrack, matInvTrack;
		matTrack = *wall->Get_Transform()->Get_World();
		D3DXMatrixInverse(&matInvTrack, 0, &matTrack);

		// OBB의 회전을 spline의 모델 스페이스로 보내기 위한 역 쿼터니언
		_quaternion qTrack, qInvTrack;
		qTrack = wall->Get_Transform()->Get_WorldQuaternion();
		D3DXQuaternionInverse(&qInvTrack, &qTrack);

		// 플레이어의 박스 콜라이더를 spline의 모델 스페이스로 보낸다.
		// 박스 콜라이더의 Center/Orientation를 변환해서 다시 넣는 방식
		D3DXVec3TransformCoord(&vCartModelCenter, &vCartOldCenter, &matInvTrack);
		qCartModel = qCart * qInvTrack;
		OBB.Center = ToXMFLOAT3(vCartModelCenter);
		OBB.Orientation = ToXMFLOAT4(qCartModel);

		// 벽의 boundingbox와 플레이어의 콜라이더가 닿는지 검사
		bool bCheckCollision = box.Intersects(OBB);
		if (bCheckCollision == false)
			continue;

		// 충돌한 벽을 찾았다면 이제 spline이 갖고 있는 삼각형(면)에 대해서 intersect로 충돌한 평면 하나 찾기
		vector<VTXTEX> vecVertices = pSpline->GetVertices();
		vector<FACE32> vecFaces = pSpline->GetFaces();

		bool bCollision = false;
		_vec3 MTV;
		float closestDist = FLT_MAX;

		for (int i = 0; i < vecFaces.size(); ++i)
		{
			// 로컬에서의 정점
			_vec3 p0 = vecVertices[vecFaces[i].indices._0].vPosition;
			_vec3 p1 = vecVertices[vecFaces[i].indices._1].vPosition;
			_vec3 p2 = vecVertices[vecFaces[i].indices._2].vPosition;

			// 지형의 양 끝의 경우, 삼각형이 너무 작아서 외적이 불가능한 경우가 생김
			// 이러한 삼각형은 Intersects 시 에러가 발생하며, 법선을 계산할 수 없으므로 스킵
			_vec3 e1 = p1 - p0;
			_vec3 e2 = p2 - p0;
			_vec3 n;
			D3DXVec3Cross(&n, &e1, &e2);
			if (D3DXVec3LengthSq(&n) < 1e-12f)
				continue;

			if (!OBB.Intersects(ToXMVec(p0), ToXMVec(p1), ToXMVec(p2)))
				continue;

			// 충돌시
			// 1. 법선벡터 생성(카트를 바라보는 방향의 법선벡터)
			// 로컬 -> 월드로 변환
			D3DXVec3TransformCoord(&p0, &p0, &matTrack);
			D3DXVec3TransformCoord(&p1, &p1, &matTrack);
			D3DXVec3TransformCoord(&p2, &p2, &matTrack);
			D3DXPLANE plane;
			D3DXPlaneFromPoints(&plane, &p0, &p1, &p2);

			_vec3 normal = { plane.a,plane.b,plane.c };

			float r = extends.x * fabsf(D3DXVec3Dot(&normal, &vRight))
				+ extends.y * fabsf(D3DXVec3Dot(&normal, &vUp))
				+ extends.z * fabsf(D3DXVec3Dot(&normal, &vLook));

			float s = fabsf(plane.a * vCartOldCenter.x
				+ plane.b * vCartOldCenter.y
				+ plane.c * vCartOldCenter.z + plane.d);

			// 충돌을 하지 않음
			if (s > r)
				continue;

			// 최단거리 평면과의 MTV를 구함
			if (s < closestDist) {
				bCollision = true;
				closestDist = s;
				MTV = (r - s) * normal;
				//법선벡터에 -1을 곱하냐 마냐를 결정함
				if (plane.a * vCartOldCenter.x
					+ plane.b * vCartOldCenter.y
					+ plane.c * vCartOldCenter.z
					+ plane.d < 0)
					MTV *= -1;
			}
		}
		if (bCollision) {
			m_bCollisionWall = true;
			SoundMgr::GetInstance().PlaySound(L"Effect/cart/crash.ogg", COLLISION_EFFECT, 0.4f);
			m_pTransformCom->Set_Pos(vPos + MTV);

			// 2. 가속도에서 벽 쪽으로 들어가는 속도 성분을 제거
			_vec3 MTV_n;
			D3DXVec3Normalize(&MTV_n, &MTV);
			float inward = D3DXVec3Dot(&m_vForce, &MTV_n);
			// MTV가 벽 밖으로 나가는 방향 
			if (inward < 0)
				m_vForce -= MTV_n * inward;

			// 3. 조금 튕겨나가도록
			m_vForce += MTV_n * 10.f;

			// 4. 힘 약화
			m_vForce *= 0.98f;

			// 5. Gage, Drift 초기화
			m_fGainGage = 0;
			m_bDrift = false;
		}
	}
}

void CCartBot::UpdateGravity()
{
	/*
	중력 -> 지면의 -Look , -Up 성분으로 분해(투영으로 분해)
			  지면의  -Look = (지면 법선 x 지면의 Right)
			  지면의 - Up    = -Normal
	*/

	_vec3 vGravity = { 0,-0.98f,0 };
	_vec3 vCartUp, vPlaneRight, vPlaneLook;
	float fSize;

	switch (m_eCartState)
	{
	case CART_STATE_GROUND:
		if (m_vTerrainNormal != _vec3({ 0,1,0 }))
		{
			// 평면의 Right벡터
			m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
			D3DXVec3Cross(&vPlaneRight, &m_vTerrainNormal, &vCartUp);

			// 평면의 Look
			D3DXVec3Cross(&vPlaneLook, &vPlaneRight, &m_vTerrainNormal);

			// 중력의 성분 중에 -Look 방향의 성분만 받기
			// -Look벡터에 Gravity 투영해서 -Look 방향의 크기 구하기
			D3DXVec3Normalize(&vPlaneLook, &vPlaneLook);
			vPlaneLook *= -1;
			fSize = D3DXVec3Dot(&vPlaneLook, &vGravity);

			// 구한 크기에 -Look 방향벡터 곱해서 vForce에 적용
			m_vForce += fSize * vPlaneLook;
		}
		else
		{
			m_vForce.y = 0;
		}
		break;
	case CART_STATE_AIR:
		// 중력 전부 다 받기
		m_vForce += vGravity;
		break;
	case CART_STATE_LANDING:
		break;
	case CART_STATE_END:
		break;
	default:
		break;
	}
}

void CCartBot::UpdateMagnet(const _float& fDeltaTime)
{
	if (m_bMagnet == true)
	{
		CGameObject* pTarget = m_pMagnetTarget;
		if (pTarget == nullptr)
		{
			m_bMagnet = false;
			m_fMagnetTimer = 0.f;
			return;
		}

		if (m_fMagnetTimer < 1.5f)
			if (pTarget == CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"))
				SoundMgr::GetInstance().PlaySound(L"Effect/Item_magnet/using.ogg", SOUND_MAGNET, 0.4f);

		_vec3 vPos, vLook, vTargetPos, vDir;

		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		pTarget->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);

		vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) <= 0.001f)
			return;

		D3DXVec3Normalize(&vDir, &vDir);

		_float fDirection = D3DXVec3Dot(&vLook, &vDir);

		if (fDirection < -0.5 || fDirection > 0.5)		// 그 방향이 카트 기준 앞/뒤 방향인지 확인
		{
			m_vForce += vDir * 2.f;
			//vDir += m_vForce * 2.f;
		}

		m_fMagnetTimer += fDeltaTime;					// 3.5초 지나면 m_bMagnet = false로 종료

		if (m_fMagnetTimer > 3.5f)
		{
			m_bMagnet = false;
			m_fMagnetTimer = 0.f;
			m_pMagnetTarget = nullptr;
		}
	}
}

void CCartBot::UpdateBubble(const _float& fDeltaTime)
{
	if (m_bBubble == false)
		return;
	static_cast<CWaterBombBubble*>(m_pBubble)->SetShow(true);
	m_fBubbleTimer += fDeltaTime;


	if (m_fBubbleTimer <= 1.f)
		m_vForce = _vec3({ 0,1,0 }) * m_fBubbleTimer * 15;
	else if (m_fBubbleTimer <= 1.5f)
		m_vForce = { 0,0,0 };
	else if (m_fBubbleTimer <= 2.5f)
		m_vForce = _vec3({ 0,-1,0 }) * (m_fBubbleTimer - 1.5f) * 15;
	else
	{
		m_vForce = { 0,0,0 };
		m_bBubble = false;
		m_fBubbleTimer = 0.f;
		m_vRotation.z = 0.f;
		static_cast<CWaterBombBubble*>(m_pBubble)->SetShow(false);
	}
	m_pTransformCom->Move_Pos(&m_vForce, 1, fDeltaTime);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y += 3.f;
	m_pBubble->Get_Transform()->Set_Pos(vPos);

	m_vRotation.z = 0.1f;
	m_pTransformCom->Rotate(QUATER_ROLL, m_vRotation.z);
	
}
void CCartBot::OutputCarState()
{
	switch (m_eCartState)
	{
	case CART_STATE_GROUND:
		cout << "CART_STATE_GROUND" << endl;
		break;
	case CART_STATE_AIR:
		cout << "CART_STATE_AIR" << endl;
		break;
	case CART_STATE_LANDING:
		cout << "CART_STATE_LANDING" << endl;
		break;
	case CART_STATE_END:
		cout << "CART_STATE_GROUND" << endl;
		break;
	default:
		break;
	}
}

void CCartBot::AddWheel()
{
	for (auto& pFirstChild : m_vecChildren)
	{
		if (dynamic_cast<CCartBody*>(pFirstChild) != nullptr)
		{
			for (auto& pSecondChild : pFirstChild->Get_Children())
			{
				if (dynamic_cast<CWheel*>(pSecondChild) != nullptr)
				{
					m_vecWheel.push_back(pSecondChild);
				}
			}
			return;
		}
	}
}

void CCartBot::SetWheelForceLen()
{
	for (auto& pWheel : m_vecWheel)
		static_cast<CWheel*>(pWheel)->SetCartForceLen(D3DXVec3Length(&m_vForce) * m_fSpeed);
}

void CCartBot::SetWheelDir()
{
	for (auto& pWheel : m_vecWheel)
		static_cast<CWheel*>(pWheel)->SetCartDir(m_eDirection);
}

void CCartBot::SetWheelTurn(WHEEL_TURN eTurn)
{
	for (auto& pWheel : m_vecWheel)
		static_cast<CWheel*>(pWheel)->SetWheelTurn(eTurn);
}

void CCartBot::CreateMissileObject(CGameObject* pTarget)
{
	CGameObject* pMissile = CMissile::Create(m_pGraphicDev);

	if (pMissile == nullptr)
		return;
	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Missile", pMissile)))
		return;
	static_cast<CMissile*>(pMissile)->SetTarget(pTarget);

	if (pTarget == CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"))
		SoundMgr::GetInstance().PlaySound(L"Effect/Item_rocket/shooting.ogg", SOUND_MISSILE, 0.4f, true);

	CGameObject* pMissileBody = CMissileBody::Create(m_pGraphicDev);
	if (pMissileBody == nullptr)
		return;
	if (FAILED(m_pLayer->Add_GameObject(L"Obj_MissileBody", pMissileBody)))
		return;

	pMissile->Set_Child(pMissileBody);

	_vec3 vPos, vLook, vTargetPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	pMissile->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	pTarget->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);

	_vec3 vDir = vTargetPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	vPos += vDir * 5.f;

	if (D3DXVec3Length(&vDir) <= 0.001f)
		return;

	D3DXVec3Normalize(&vDir, &vDir);

	_quaternion qRot;

	D3DXQuaternionRotationYawPitchRoll(&qRot, m_vRotation.y, 0.f, 0.f);

	pMissile->Get_Transform()->Set_Pos(vPos);
	pMissile->Get_Transform()->Set_Quaternion(&qRot);
}

void CCartBot::CreateMagnetObject()
{
	CGameObject* pMagnet = CMagnet::Create(m_pGraphicDev, this);

	if (nullptr == pMagnet)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Magnet", pMagnet)))
		return;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y += 5;
	pMagnet->Get_Transform()->Set_Pos(vPos);
	_quaternion q = m_pTransformCom->Get_WorldQuaternion();
	pMagnet->Get_Transform()->Set_Quaternion(&q);

	CMagnetBody* pBody = CMagnetBody::Create(m_pGraphicDev);
	m_pLayer->Add_GameObject(L"Obj_MagnetBody", pBody);
	pMagnet->Set_ChildWithoutTune(pBody);
}

void CCartBot::CreateWaterBombObject()
{
	CGameObject* pWaterBombThrow = CWaterBombThrow::Create(m_pGraphicDev);
	Set_Child(pWaterBombThrow);

	if (pWaterBombThrow == nullptr)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterBombThrow", pWaterBombThrow)))
		return;
}

void CCartBot::CreateWaterFlyObject()
{
	CGameObject* pTarget = CTrackMgr::GetInstance()->Get_Forward(this);
	if (pTarget == nullptr)
		return;

	if(pTarget == CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"))
		SoundMgr::GetInstance().PlaySound(L"Effect/Item_waterbombFly/firing.mp3", SOUND_WATERFLY, 0.4f);

	CGameObject* pWaterFly = CWaterFly::Create(m_pGraphicDev, pTarget);

	if (pWaterFly == nullptr)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterFly", pWaterFly)))
		return;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	pWaterFly->Get_Transform()->Set_Pos(vPos);
}

void CCartBot::CreateShieldObject()
{
	static_cast<CShield1*>(m_pShield1)->SetShow(true);
}

void CCartBot::CreateBarricadeObject()
{
	CGameObject* pBarricade = CBarricade::Create(m_pGraphicDev);

	if (pBarricade == nullptr)
		return;
	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Barricade", pBarricade)))
		return;

	CGameObject* pApex = CTrackMgr::GetInstance()->Get_Apex();
	if (!pApex)
		return;

	TrackPose TP_front = CTrackMgr::GetInstance()->Compute_TargetPose(pApex, 60, false);
	TrackPose TP_back = CTrackMgr::GetInstance()->Compute_TargetPose(pApex, 90, false);
	if (TP_front.bValid && TP_back.bValid) {
		CBarricade* pBar = CBarricade::Create(m_pGraphicDev);
		if (pBar == nullptr)
			return;
		if (FAILED(m_pLayer->Add_GameObject(L"Obj_Barricade", pBar)))
			return;

		pBar->Get_Transform()->Set_Pos(TP_front.position);
		pBar->Set_OriginPos(TP_front.position);

		_matrix	matRot;
		D3DXMatrixIdentity(&matRot);
		memcpy(&matRot.m[0], &TP_front.R, sizeof(_vec3));
		memcpy(&matRot.m[1], &TP_front.U, sizeof(_vec3));
		memcpy(&matRot.m[2], &TP_front.T, sizeof(_vec3));

		_quaternion q;
		D3DXQuaternionRotationMatrix(&q, &matRot);
		pBar->Get_Transform()->Set_Quaternion(&q);

		CTrackMgr::GetInstance()->Register_Hazard(pBar, ITEM_BARRICADE);

		pBar = CBarricade::Create(m_pGraphicDev);
		if (pBar == nullptr)
			return;
		if (FAILED(m_pLayer->Add_GameObject(L"Obj_Barricade", pBar)))
			return;

		_vec3 vPosLeft = TP_back.position + TP_back.R * TP_back.halfW * -0.5f;
		pBar->Get_Transform()->Set_Pos(vPosLeft);
		pBar->Set_OriginPos(vPosLeft);

		D3DXMatrixIdentity(&matRot);
		memcpy(&matRot.m[0], &TP_back.R, sizeof(_vec3));
		memcpy(&matRot.m[1], &TP_back.U, sizeof(_vec3));
		memcpy(&matRot.m[2], &TP_back.T, sizeof(_vec3));

		D3DXQuaternionRotationMatrix(&q, &matRot);
		pBar->Get_Transform()->Set_Quaternion(&q);

		CTrackMgr::GetInstance()->Register_Hazard(pBar, ITEM_BARRICADE);

		pBar = CBarricade::Create(m_pGraphicDev);
		if (pBar == nullptr)
			return;
		if (FAILED(m_pLayer->Add_GameObject(L"Obj_Barricade", pBar)))
			return;

		_vec3 vPosRight = TP_back.position + TP_back.R * TP_back.halfW * 0.5f;
		pBar->Get_Transform()->Set_Pos(vPosRight);
		pBar->Set_OriginPos(vPosRight);

		D3DXMatrixIdentity(&matRot);
		memcpy(&matRot.m[0], &TP_back.R, sizeof(_vec3));
		memcpy(&matRot.m[1], &TP_back.U, sizeof(_vec3));
		memcpy(&matRot.m[2], &TP_back.T, sizeof(_vec3));

		D3DXQuaternionRotationMatrix(&q, &matRot);
		pBar->Get_Transform()->Set_Quaternion(&q);

		CTrackMgr::GetInstance()->Register_Hazard(pBar, ITEM_BARRICADE);
	}
}

CGameObject* CCartBot::AcquireAimTarget()
{
	return CTrackMgr::GetInstance()->Get_Forward(this);
}

void CCartBot::ConsumeFirstItem()
{
	m_eFirstSlot = m_eSecondSlot;
	m_eSecondSlot = ITEM_END;
}

void CCartBot::GainItem()
{
	if (m_eFirstSlot == ITEM_END)
	{
		m_eFirstSlot = ITEM_TYPE(rand() % ITEM_END);
	}

	else if (m_eFirstSlot != ITEM_END)
	{
		m_eSecondSlot = ITEM_TYPE(rand() % ITEM_END);
	}
}

void CCartBot::GainBoost()
{
	if (m_eFirstSlot == ITEM_END)
	{
		m_eFirstSlot = ITEM_BOOSTER;
	}

	else if (m_eFirstSlot != ITEM_END)
	{
		m_eSecondSlot = ITEM_BOOSTER;
	}
}

void CCartBot::UseItem()
{
	switch (m_eFirstSlot)
	{
	case ITEM_BOOSTER:
		m_eBoostState = BOOST_STATE_LONG_BOOST;
		m_fBoostCal = 1.015f;
		break;
	case ITEM_THUNDER:
		CreateThunderCloudObject();
		break;
	case ITEM_CLOUD:
		CreateCloudObject();
		break;
	case ITEM_WATERFLY:
		CreateWaterFlyObject();
		break;
	case ITEM_BARRICADE:
		CreateBarricadeObject();
		break;
	case ITEM_BANANA:
		if (m_eCartState == CART_STATE_AIR || m_bBubble || m_bMissileHit)
			return;
		CreateBananaObject();
		break;
	case ITEM_WATERBOMB:
		CreateWaterBombObject();
		break;
	case ITEM_SHIELD:
		CreateShieldObject();
		break;
	case ITEM_ROCKET:
	case ITEM_MAGNET:
		UseAimItem();
		return;
	case ITEM_END:
		return;
	default:
		break;
	}

	ConsumeFirstItem();
}

void CCartBot::UseAimItem()
{
	switch (m_eFirstSlot)
	{
	case ITEM_ROCKET:
		UseMissileItem();
		break;

	case ITEM_MAGNET:
		UseMagnetItem();
		break;
	}
}

void CCartBot::UseMissileItem()
{
	CGameObject* pTarget = AcquireAimTarget();
	if (pTarget == nullptr)
		return;

	CreateMissileObject(pTarget);
	ConsumeFirstItem();
}

void CCartBot::UseMagnetItem()
{
	if (m_bMagnet)
		return;

	CGameObject* pTarget = AcquireAimTarget();
	if (pTarget == nullptr)
		return;

	m_bMagnet = true;
	m_fMagnetTimer = 0.f;
	m_pMagnetTarget = pTarget;
	CreateMagnetObject();
	ConsumeFirstItem();
}

void CCartBot::Free()
{
	CGameObject::Free();
}

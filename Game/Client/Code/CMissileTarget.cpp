#include "pch.h"
#include "CMissileTarget.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCollisionMgr.h"
#include "CManagement.h"
#include "CCube_Collider.h"	
#include "CWaterBombBubble.h"
#include "CShield1.h"
#include "CShield2.h"

CMissileTarget::CMissileTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMissileTarget::~CMissileTarget()
{
}

HRESULT CMissileTarget::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_iLast_KeyInput	= 0;	
	m_iAccumulate		= 0;

	m_fSpeed			= 0.f;
	m_fMaxSpeed			= 3.f;
	m_fTimer			= 0.f;
	m_fShieldTimer		= 0.f;

	m_bMissileHit		= false;
	m_bWaterBombHit		= false;
	m_bWaterFlyHit		= false;
	m_bWaterBubble		= false;
	m_bWaterFly			= false;
	m_bBubbling			= false;
	m_bBubbleUI			= false;
	m_bShieldHit		= false;
	m_bShieldTimer		= false;
	m_bShieldActive		= false;
	m_bUfoHit			= false;

	m_pTransformCom->Set_Pos({ -30.f,0.f, 90.f });
	m_pTransformCom->Set_Scale({ 1.5f, 1.5f, 1.f });

	m_pBubble = nullptr;

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 2.5f, 1.5f, 3.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CMissileTarget::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (m_bMissileHit == false && m_bWaterBombHit == false && m_bWaterFlyHit == false && m_bUfoHit == false)
		return;

	if (m_bMissileHit)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vPos.y += m_vForce.y * fFixedDeltaTime;
		m_vForce.y -= 30.f * fFixedDeltaTime;

		m_vRotation.x += D3DXToRadian(350.f) * fFixedDeltaTime;

		if (m_vRotation.x >= D3DXToRadian(1080.f))
		{
			m_vRotation.x = D3DXToRadian(1080.f);
		}

		_quaternion q;
		D3DXQuaternionRotationYawPitchRoll(&q, 0.f, m_vRotation.x, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		if (vPos.y <= 0.f)
		{
			vPos.y = 0.f;
			m_vForce.y = 0.f;
			m_vRotation.x = 0.f;
			m_bMissileHit = false;

			_quaternion qReset;
			D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
			m_pTransformCom->Set_Quaternion(&qReset);
		}

		m_pTransformCom->Set_Pos(vPos);
	}

	if (m_bWaterBombHit)
	{
		m_fTimer += fFixedDeltaTime;

		_vec3 vPos, vBubblePos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		if (m_fTimer < 0.08f)
		{
			vPos.y += m_vForce.y * fFixedDeltaTime;
		}

		//else if (m_fTimer < 0.08f)
		//{
		//	m_vForce.y = 0;
		//}

		else if (m_fTimer > 0.15f && m_fTimer < 0.40f)
		{
			m_vRotation.x -= D3DXToRadian(100.f) * fFixedDeltaTime;
			m_vRotation.y += D3DXToRadian(100.f) * fFixedDeltaTime;

			if (m_vRotation.x <= D3DXToRadian(-25.f))
			{
				m_vRotation.x = D3DXToRadian(-25.f);
			}

			if (m_vRotation.y >= D3DXToRadian(25.f))
			{
				m_vRotation.y = D3DXToRadian(25.f);
			}
		}

		else if (m_fTimer > 1.0f && m_fTimer < 2.f)
		{
			m_vRotation.x += D3DXToRadian(5.f) * fFixedDeltaTime;
			m_vRotation.y -= D3DXToRadian(5.f) * fFixedDeltaTime;

			if (m_vRotation.x >= D3DXToRadian(-20.f))
			{
				m_vRotation.x = D3DXToRadian(-20.f);
			}

			if (m_vRotation.y <= D3DXToRadian(20.f))
			{
				m_vRotation.y = D3DXToRadian(20.f);
			}

		}

		else if (m_fTimer >= 2.f)
		{
			vPos.y -= m_vForce.y * 0.45f * fFixedDeltaTime;
			m_vForce.y -= 5.f * fFixedDeltaTime;
		}

		_quaternion q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		if (vPos.y <= 0.f)
		{
			vPos.y = 0.f;
			m_vForce.y = 0.f;
			m_vRotation.x = 0.f;
			m_vRotation.y = 0.f;
			m_bWaterBombHit = false;
			m_bWaterBubble = false;
			SetBubbleUI(false);
			m_pBubble->GetLayer()->Delete_GameObject(m_pBubble);
			m_fTimer = 0.f;

			_quaternion qReset;
			D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
			m_pTransformCom->Set_Quaternion(&qReset);
		}

		m_pTransformCom->Set_Pos(vPos);
		m_pBubble->Get_Transform()->Set_Pos(vPos);
		//pBubble->GetLayer()->Delete_GameObject(pBubble);
	}

	if (m_bWaterFlyHit)
	{
		m_fTimer += fFixedDeltaTime;

		_vec3 vPos, vFlyPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		if (m_fTimer < 0.08f)
		{
			vPos.y += m_vForce.y * fFixedDeltaTime;
		}

		//else if (m_fTimer < 0.08f)
		//{
		//	m_vForce.y = 0;
		//}

		else if (m_fTimer > 0.15f && m_fTimer < 0.40f)
		{
			m_vRotation.x -= D3DXToRadian(100.f) * fFixedDeltaTime;
			m_vRotation.y += D3DXToRadian(100.f) * fFixedDeltaTime;

			if (m_vRotation.x <= D3DXToRadian(-25.f))
			{
				m_vRotation.x = D3DXToRadian(-25.f);
			}

			if (m_vRotation.y >= D3DXToRadian(25.f))
			{
				m_vRotation.y = D3DXToRadian(25.f);
			}
		}

		else if (m_fTimer > 1.0f && m_fTimer < 2.f)
		{
			m_vRotation.x += D3DXToRadian(5.f) * fFixedDeltaTime;
			m_vRotation.y -= D3DXToRadian(5.f) * fFixedDeltaTime;

			if (m_vRotation.x >= D3DXToRadian(-20.f))
			{
				m_vRotation.x = D3DXToRadian(-20.f);
			}

			if (m_vRotation.y <= D3DXToRadian(20.f))
			{
				m_vRotation.y = D3DXToRadian(20.f);
			}

		}

		else if (m_fTimer >= 2.f)
		{
			vPos.y -= m_vForce.y * 0.45f * fFixedDeltaTime;
			m_vForce.y -= 5.f * fFixedDeltaTime;
		}

		_quaternion q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		if (vPos.y <= 0.f)
		{
			vPos.y = 0.f;
			m_vForce.y = 0.f;
			m_vRotation.x = 0.f;
			m_vRotation.y = 0.f;
			m_bWaterFlyHit = false;
			m_bWaterBubble = false;
			SetBubbleUI(false);
			m_pBubble->GetLayer()->Delete_GameObject(m_pBubble);
			m_fTimer = 0.f;

			_quaternion qReset;
			D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
			m_pTransformCom->Set_Quaternion(&qReset);
		}

		m_pTransformCom->Set_Pos(vPos);
		m_pBubble->Get_Transform()->Set_Pos(vPos);
	}

	// if (m_bUfoHit)
	// {
	//	m_fTimer += fFixedDeltaTime;

	//	_vec3 vPos, vFlyPos;
	//	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	//	if (m_fTimer < 0.08f)
	//	{
	//		vPos.y += m_vForce.y * fFixedDeltaTime;
	//	}

	//	//else if (m_fTimer < 0.08f)
	//	//{
	//	//	m_vForce.y = 0;
	//	//}

	//	else if (m_fTimer > 0.15f && m_fTimer < 0.40f)
	//	{
	//		m_vRotation.x -= D3DXToRadian(100.f) * fFixedDeltaTime;
	//		m_vRotation.y += D3DXToRadian(100.f) * fFixedDeltaTime;

	//		if (m_vRotation.x <= D3DXToRadian(-25.f))
	//		{
	//			m_vRotation.x = D3DXToRadian(-25.f);
	//		}

	//		if (m_vRotation.y >= D3DXToRadian(25.f))
	//		{
	//			m_vRotation.y = D3DXToRadian(25.f);
	//		}
	//	}

	//	else if (m_fTimer > 1.0f && m_fTimer < 2.f)
	//	{
	//		m_vRotation.x += D3DXToRadian(5.f) * fFixedDeltaTime;
	//		m_vRotation.y -= D3DXToRadian(5.f) * fFixedDeltaTime;

	//		if (m_vRotation.x >= D3DXToRadian(-20.f))
	//		{
	//			m_vRotation.x = D3DXToRadian(-20.f);
	//		}

	//		if (m_vRotation.y <= D3DXToRadian(20.f))
	//		{
	//			m_vRotation.y = D3DXToRadian(20.f);
	//		}

	//	}

	//	else if (m_fTimer >= 2.f)
	//	{
	//		vPos.y -= m_vForce.y * 0.45f * fFixedDeltaTime;
	//		m_vForce.y -= 5.f * fFixedDeltaTime;
	//	}

	//	_quaternion q;
	//	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
	//	m_pTransformCom->Set_Quaternion(&q);

	//	if (vPos.y <= 0.f)
	//	{
	//		vPos.y = 0.f;
	//		m_vForce.y = 0.f;
	//		m_vRotation.x = 0.f;
	//		m_vRotation.y = 0.f;
	//		m_bUfoHit	   = false;
	//		m_bWaterBubble = false;
	//		SetBubbleUI(false);
	//		m_pBubble->GetLayer()->Delete_GameObject(m_pBubble);
	//		m_fTimer = 0.f;

	//		_quaternion qReset;
	//		D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
	//		m_pTransformCom->Set_Quaternion(&qReset);
	//	}

	//	m_pTransformCom->Set_Pos(vPos);
	//	m_pBubble->Get_Transform()->Set_Pos(vPos);
	//}
}

_int CMissileTarget::Update_GameObject(const _float& fDeltaTime)
{
	KeyInput(fDeltaTime);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	if (m_bShieldTimer == true && m_bShieldActive == true)
	{
		m_fShieldTimer += fDeltaTime;
		// 2초마다 초기화인데
		// 충돌이 발생하고 2초 지나기 전 그 사이 갭 동안 쉴드를 키면 쉴드 2가 나오는거지
		if (m_fShieldTimer >= 2.f)
		{
			m_bShieldHit	= false;
			m_bShieldActive = false;
			m_bShieldTimer	= false;
			m_fShieldTimer	= 0;
		}
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMissileTarget::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMissileTarget::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetTexture(0, nullptr);

	m_pBufferCom->Render_Buffer();
}

void CMissileTarget::KeyInput(const _float& fDeltaTime)
{
	CGameObject* pWaterBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_WaterBombBody");

	_vec3 vPos, vFlyPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y >= 0.f && m_bWaterBubble == true)
	{
		if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_LEFT))
		{
			if (m_iAccumulate <= 12)
			{

				if (m_iLast_KeyInput == 1)
				{
					m_iAccumulate += 1;
				}

				else if (m_iLast_KeyInput == 2)
				{
					m_iAccumulate += 2;
				}
				m_iLast_KeyInput = 1;

			}

		}

		if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_RIGHT))
		{
			if (m_iAccumulate <= 12)
			{

				if (m_iLast_KeyInput == 2)
				{
					m_iAccumulate += 1;
				}

				else if (m_iLast_KeyInput == 1)
				{
					m_iAccumulate += 2;
				}

				m_iLast_KeyInput = 2;
			}
		}

		if (m_iAccumulate >= 12)
		{
			vPos.y -= m_vForce.y * 0.45f * fDeltaTime;	// 원래는 픽시드
			m_vForce.y -= 5.f * fDeltaTime;

			m_pTransformCom->Set_Pos(vPos);
			m_pBubble->Get_Transform()->Set_Pos(vPos);
		}

		if (vPos.y <= 0.f && m_iAccumulate >= 12)
		{

			if (m_bWaterBombHit)
			{
				if (pWaterBody != nullptr)
					pWaterBody->GetLayer()->Delete_GameObject(pWaterBody);

				m_bWaterBombHit = false;
			}

			else if (m_bWaterFlyHit)
			{
				m_bWaterFlyHit = false;
			}

			vPos.y = 0.f;
			m_vForce.y = 0.f;
			m_vRotation.x = 0.f;
			m_vRotation.y = 0.f;
			m_iAccumulate = 0.f;
			// m_bWaterFlyHit = false;
			// m_bWaterBombHit = false;	
			m_bWaterBubble = false;
			SetBubbleUI(false);
			m_pBubble->GetLayer()->Delete_GameObject(m_pBubble);
			// pWaterBody->GetLayer()->Delete_GameObject(pWaterBody);
			m_fTimer = 0.f;

			_quaternion qReset;
			D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
			m_pTransformCom->Set_Quaternion(&qReset);
		}

	}

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_O))
	{
		CreateShieldObject();
		m_bShieldActive = true;
		m_bShieldTimer	= true;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_5))
	{
		CreateShieldObject();
		m_bShieldActive = true;
		m_bShieldTimer = true;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_1))
	{
		CreateShieldObject();
		m_bShieldActive = true;
		m_bShieldTimer = true;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_3))
	{
		CreateShieldObject();
		m_bShieldActive = true;
		m_bShieldTimer = true;
	}
}

void CMissileTarget::CreateShieldObject()
{
	CGameObject* pTarget = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget");

	CGameObject* pShield1 = CShield1::Create(m_pGraphicDev);

	if (pShield1 == nullptr)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Shield1", pShield1)))
		return;

	//pShield1->SetLayer(m_pLayer);
	pTarget->Set_Child(pShield1);
	//Set_Child(pShield1);

	CGameObject* pShield2 = CShield2::Create(m_pGraphicDev);

	if (pShield2 == nullptr)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_pShield2", pShield2)))
		return;

	//pShield2->SetLayer(m_pLayer);
	pTarget->Set_Child(pShield2);
	//Set_Child(pShield1);
}

void CMissileTarget::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMissileTarget::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
	
	// 그니까 아까 설명 했던 내용이 뭔 말이냐면 TriggerEnter는 충돌하면 그냥 불리는데
	// 이전에 if(m_bShieldHit == false)이면 true로 하는거는 당연히 조준점만 닿아도 불리지
	// 근데 밑에서 보면 미사일인지, 물폭탄인지, 물파리인지 검사하잖아
	// 그럼 그냥 걔네 안에서 쉴드가 활성화되어있는지 보고, 그 안에서 쉴드를 켜면 되지 않느냐는거지
	// 이전의 조건문은 닿은얘가 누구든지 상관없이 작동했으니까
	//if (m_bShieldHit == false && (wcscmp(wOtherTag, L"Obj_TargetAim"))!= 0 )	// 지금은 충돌안하네? ㅈㅈ
	//{
	//	m_bShieldHit = true;	
	//}

	// 가끔 쉴드2가 되는 이유가 뭐냐 <- 쉴드 2가 되는 조건은 m_bShieldHit가 true인거지?
	// 근데 위에 있는 이전 조건문은 targetAim만 아니면 충돌 시 true가 되잖아
	// 근데 이걸 다시 false로 하는 코드는 쉴드가 켜졌을 때만 그런거니까
	// 뭐가 됐든 한번 충돌을 하면 얘가 true상태로 있던거야
	// 그 상태에서 형이 쉴드를 활성화하니까 바로 쉴드 2가 되는거지
	// 결국에는 쉴드의 활성화 상태랑 무관하게 m_bShieldHit를 true로 하던 이전 조건문이 잘못되었다.
	// 난 그냥 타이머 2초지나면  펄스주니까 거기서 되는줄
	//else
	//{
		if (wcsncmp(wOtherTag, L"Obj_Missile", 11) == 0)
		{
			// 내 말은 m_bShieldActive가 쉴드가 켜져있다는 거잖아?
			// 이 문자열 검사를 통과 했다는건 미사일이라는 거잖아?
			// 그럼 이 안에서 이런식으로 하면
			// 미사일일 때, 쉴드가 켜져있으면 막는다
			// 안 켜져있으면 맞는다가 되잖아?
			// 그걸 밑에도 똑같이 하면 되지 않을까?
			if (m_bShieldActive)
				m_bShieldHit = true;
			else if (m_bMissileHit == false)
			{
				m_bMissileHit = true;
				m_vForce.y = 30.f;
				m_vRotation.x += D3DXToRadian(0.f);
			}
		}
		// 이미 미사일에 걸렸다면 물폭탄/물파리 검사는 할 필요 없으니까
		//if (wcsncmp(wOtherTag, L"Obj_WaterBomb", 13) == 0)
		else if (wcscmp(wOtherTag, L"Obj_WaterBomb") == 0)
		{
			if (m_bShieldActive)
				m_bShieldHit = true;
			else if (m_bWaterBombHit == false && m_bWaterBubble == false)
			{
				m_bWaterBombHit = true;
				m_bWaterBubble = true;
				SetBubbleUI(true);
				m_vForce.y = 120.f;
				m_vRotation.x += D3DXToRadian(0.f);
				m_vRotation.y += D3DXToRadian(0.f);

				m_pBubble = CWaterBombBubble::Create(m_pGraphicDev);

				if (m_pBubble == nullptr)
					return;

				if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterBombBubble", m_pBubble)))
					return;

				m_pBubble->SetLayer(m_pLayer);

				_vec3 vPos;

				m_pTransformCom->Get_Info(INFO_POS, &vPos);
				m_pBubble->Get_Transform()->Set_Pos(vPos);
			}
		}

		else if (wcscmp(wOtherTag, L"Obj_WaterFly") == 0)
		{
			if (m_bShieldActive)
				m_bShieldHit = true;
			else if (m_bWaterFlyHit == false && m_bWaterBubble == false)
			{
				m_bWaterFlyHit = true;
				m_bWaterBubble = true;
				SetBubbleUI(true);
				m_vForce.y = 120.f;
				m_vRotation.x += D3DXToRadian(0.f);
				m_vRotation.y += D3DXToRadian(0.f);

				m_pBubble = CWaterBombBubble::Create(m_pGraphicDev);

				if (m_pBubble == nullptr)
					return;

				if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterBombBubble", m_pBubble)))
					return;

				m_pBubble->SetLayer(m_pLayer);

				_vec3 vPos;

				m_pTransformCom->Get_Info(INFO_POS, &vPos);
				m_pBubble->Get_Transform()->Set_Pos(vPos);
			}
		}

		//else if (wcscmp(wOtherTag, L"Obj_Ufo") == 0)	// 머리위로 충돌하고 몇 초 뒤 속도 느리게 아이템 발동 서서히 느려지게 -> 몇초 뒤 복원
		//{
		//	if (m_bShieldActive)
		//		m_bShieldHit = true;
		//	else if (m_bUfoHit == false && m_bWaterBubble == false)	// 수정하기
		//	{
		//		m_bUfoHit = true;
		//		m_bWaterBubble = true;
		//		SetBubbleUI(true);
		//		m_vForce.y = 120.f;
		//		m_vRotation.x += D3DXToRadian(0.f);
		//		m_vRotation.y += D3DXToRadian(0.f);

		//		m_pBubble = CWaterBombBubble::Create(m_pGraphicDev);

		//		if (m_pBubble == nullptr)
		//			return;

		//		if (FAILED(m_pLayer->Add_GameObject(L"Obj_WaterBombBubble", m_pBubble)))
		//			return;

		//		m_pBubble->SetLayer(m_pLayer);

		//		_vec3 vPos;

		//		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		//		m_pBubble->Get_Transform()->Set_Pos(vPos);
		//	}
		//}
	//}
}

CMissileTarget* CMissileTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileTarget* pObj = new CMissileTarget(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMissileTarget_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CMissileTarget::Free()
{
	CGameObject::Free();
}

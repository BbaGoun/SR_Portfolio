#include "CStarParticle.h"

CStarParticle::CStarParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: PSystem(pGraphicDev)
{
}

CStarParticle::~CStarParticle()
{
}

HRESULT CStarParticle::Ready_PSystem()
{

	m_fSize = 1.f;			// 연기 하나의 화면상 크기 (픽셀단위)
	m_dwVbSize = 100;		// 버텍스 버퍼 전체 크기
	m_dwVbOffset = 0;
	m_dwVbBatchSize = 25;   // 한 배치 크기 (100/25 = 4구간)

	// 초기 파티클 풀 생성 (동시에 존재할 수 있는 최대 연기 개수)
	int iParticleCnt = 20;
	for (int i = 0; i < iParticleCnt; ++i)
		AddParticle();

	PSystem::Ready_PSystem();
	for (auto& p : m_Particles)
		p.bIsAlive = false;

	return S_OK;
}

void CStarParticle::ResetParticle(Attribute* attribute)
{
	attribute->bIsAlive = true;

	//_vec3 vRandomPosOffset(
	//	GetRandomFloat(-0.1f, 0.1f),
	//	GetRandomFloat(-0.1f, 0.1f),
	//	GetRandomFloat(-0.1f, 0.1f));

	attribute->vPosition = m_vOrigin;// +vRandomPosOffset;

	_vec3 vRandomSpread(
		GetRandomFloat(-3.f, 3.f)*10,
		30.f,
		GetRandomFloat(-3.f, 3.f) * 10);
	attribute->vVelocity = vRandomSpread;

	attribute->color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = GetRandomFloat(0.2f, 0.3f);
}

void CStarParticle::Update_PSystme(float timeDelta)
{
	if (IsDead())
		return;
	for (auto& p : m_Particles)
	{
		if (!p.bIsAlive)
			continue;

		p.fAge += timeDelta;

		if (p.fAge >= p.fLifeTime)
		{
			//ResetParticle(&p);
			p.bIsAlive = false;
			continue;
		}

		p.vPosition += p.vVelocity * timeDelta;

		float fRatio = p.fAge / p.fLifeTime;
		p.color.a = 0.6f * (1.0f - fRatio);
	}
}


void CStarParticle::ResetAll()
{
	for (auto& p : m_Particles)
		ResetParticle(&p);
}

CStarParticle* CStarParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStarParticle* pParticle = new CStarParticle(pGraphicDev);

	if (FAILED(pParticle->Ready_PSystem()))
	{
		MSG_BOX("CStarParticle Create Failed");
		Safe_Release(pParticle);
		return nullptr;
	}
	return pParticle;
}
void CStarParticle::OnLostDevice()
{
	PSystem::OnLostDevice();
}

void CStarParticle::OnResetDevice()
{
	PSystem::OnResetDevice();
}
void CStarParticle::Free()
{
	PSystem::Free();
}
#include "CItemGainParticle.h"
#include <random>
#include <cmath>
#include "CCalculator.h"

CItemGainParticle::CItemGainParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: PSystem(pGraphicDev)
{
}

CItemGainParticle::~CItemGainParticle()
{
}

HRESULT CItemGainParticle::Ready_PSystem()
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

void CItemGainParticle::ResetParticle(Attribute* attribute)
{
	attribute->bIsAlive = true;

	//_vec3 vRandomPosOffset(
	//	GetRandomFloat(-0.1f, 0.1f),
	//	GetRandomFloat(-0.1f, 0.1f),
	//	GetRandomFloat(-0.1f, 0.1f));

	attribute->vPosition = m_vOrigin;// +vRandomPosOffset;

	_vec3 vRandomSpread(
		GetRandomFloat(-5.f, 5.f) * 10,
		GetRandomFloat(-5.f, 5.f) * 10,
		GetRandomFloat(-5.f, 5.f) * 10);
	attribute->vVelocity = vRandomSpread;

	std::random_device rd;
	std::mt19937 gen(rd());

	// 색상(H)은 0~360도 전 범위 균등하게 생성
	std::uniform_real_distribution<float> disH(0.0f, 360.0f);

	float fixedS = 1.f; // 0.0 ~ 1.0 (선명하게)
	float fixedV = 1.f; // 0.0 ~ 1.0 (밝게)

	float randomH = disH(gen);
	RGB color = CCalculator::HSVtoRGB(randomH, fixedS, fixedV);
	attribute->color = D3DXCOLOR(
		color.r/255.f,
		color.g/255.f,
		color.b/255.f,
		1.0f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = GetRandomFloat(0.2f, 0.3f);
}

void CItemGainParticle::Update_PSystme(float timeDelta)
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


void CItemGainParticle::ResetAll()
{
	for (auto& p : m_Particles)
		ResetParticle(&p);
}

CItemGainParticle* CItemGainParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItemGainParticle* pParticle = new CItemGainParticle(pGraphicDev);

	if (FAILED(pParticle->Ready_PSystem()))
	{
		MSG_BOX("CItemGainParticle Create Failed");
		Safe_Release(pParticle);
		return nullptr;
	}
	return pParticle;
}
void CItemGainParticle::OnLostDevice()
{
	PSystem::OnLostDevice();
}

void CItemGainParticle::OnResetDevice()
{
	PSystem::OnResetDevice();
}
void CItemGainParticle::Free()
{
	PSystem::Free();
}
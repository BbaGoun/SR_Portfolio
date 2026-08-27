#include "CSmoke.h"

CSmoke::CSmoke(LPDIRECT3DDEVICE9 pGraphicDev)
	: PSystem(pGraphicDev)
	, m_vBackDir(0.f, 0.f, -1.f) 
{
}

CSmoke::~CSmoke()
{
}

HRESULT CSmoke::Ready_PSystem()
{

	m_fSize = 0.8f;			// 연기 하나의 화면상 크기 (픽셀단위)
	m_dwVbSize = 100;		// 버텍스 버퍼 전체 크기
	m_dwVbOffset = 0;
	m_dwVbBatchSize = 25;   // 한 배치 크기 (100/25 = 4구간)

	// 초기 파티클 풀 생성 (동시에 존재할 수 있는 최대 연기 개수)
	int iParticleCnt = 60;
	for (int i = 0; i < iParticleCnt; ++i)
		AddParticle();

	PSystem::Ready_PSystem();
	for (auto& p : m_Particles)
		ResetParticle(&p);

	return S_OK;
}

void CSmoke::ResetParticle(Attribute* attribute)
{
	attribute->bIsAlive = true;

	_vec3 vRandomPosOffset(
		GetRandomFloat(-0.1f, 0.1f),
		GetRandomFloat(-0.1f, 0.1f),
		GetRandomFloat(-0.1f, 0.1f));
	attribute->vPosition = m_vOrigin + vRandomPosOffset;

	_vec3 vRandomSpread(
		GetRandomFloat(-0.3f, 0.3f),
		0.0f,
		GetRandomFloat(-0.3f, 0.3f));
	attribute->vVelocity = m_vBackDir * 1.5f + vRandomSpread;

	attribute->color = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.6f);

	attribute->fAge = 0.0f;
	attribute->fLifeTime = GetRandomFloat(0.8f, 1.5f);
}

void CSmoke::Update_PSystme(float timeDelta)
{
	for (auto& p : m_Particles)
	{
		if (!p.bIsAlive)
			continue;

		p.fAge += timeDelta;

		if (p.fAge >= p.fLifeTime)
		{
			ResetParticle(&p);
			continue;
		}

		p.vPosition += p.vVelocity * timeDelta;

		float fRatio = p.fAge / p.fLifeTime;
		p.color.a = 0.6f * (1.0f - fRatio);
	}
}


CSmoke* CSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSmoke* pParticle = new CSmoke(pGraphicDev);

	if (FAILED(pParticle->Ready_PSystem()))
	{
		MSG_BOX("CSmoke Create Failed");
		Safe_Release(pParticle);
		return nullptr;
	}
	return pParticle;
}
void CSmoke::OnLostDevice()
{
	PSystem::OnLostDevice();
}

void CSmoke::OnResetDevice()
{
	PSystem::OnResetDevice();
}
void CSmoke::Free()
{
	PSystem::Free();
}
#include "PSystem.h"
#include "Engine_Struct.h"

PSystem::PSystem(LPDIRECT3DDEVICE9 pGraphicDev) 
	: m_pGraphicDev(pGraphicDev)
	, m_vOrigin(_vec3(0.f, 0.f, 0.f))
	, m_fEmitRate(0.f)
	, m_fSize(0.f)
	, m_pVB(nullptr)
	, m_iMaxParticles(0)
	, m_dwVbSize(0)
	, m_dwVbOffset(0)
	, m_dwVbBatchSize(0)
{
	m_pGraphicDev->AddRef();
}

PSystem::~PSystem()
{
}

HRESULT PSystem::Ready_PSystem()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVbSize * sizeof(PARTICLE),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		nullptr)))
	{
		MSG_BOX("CreatePSystemVertexBuffer FAILED");
		return E_FAIL;
	}

	return S_OK;
}

void PSystem::Reset()
{
	for (auto iter = m_Particles.begin(); iter != m_Particles.end(); ++iter)
	{
		ResetParticle(&(*iter));
	}
}

void PSystem::AddParticle()
{
	Attribute attribute;		//Attribute 생성자 호출(멤버 변수 초기화)

	ResetParticle(&attribute);

	m_Particles.push_back(attribute);
}

void PSystem::PreRender_Particle()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);				// 조명
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);		// 카메라가 점 하나를 사각형 스프라이트로 자동 확장하게함
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);		// 카메라와의 z거리에 따른 크기 조절
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_fSize));		// 파티클 최대 크기
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));	// 파티클 최소 크기

	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));		// 파티클 크기 계산 공식A
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));		// 파티클 크기 계산 공식B
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));		// 파티클 크기 계산 공식C

	// 텍스처에 알파블렌딩 사용
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 텍스처의 알파값을 그대로 픽셀의 알파값으로 사용
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// 알파블랜딩 세팅
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void PSystem::Render_Particle()
{
	if (m_Particles.empty())
		return;

	PreRender_Particle();

	m_pGraphicDev->SetFVF(FVF_PARTICLE);
	// 어떤 버텍스버퍼를 사용할 지, 정점 하나의 크기가 몇 바이트인지 설정
	m_pGraphicDev->SetStreamSource(
		0,					// 스트림 슬롯 번호 (지금은 스트림 하나만 사용함)
		m_pVB,				// 사용할 버텍스 버퍼 
		0,					// 버퍼 안에서 시작할 바이트 위치(처음부터 시작)
		sizeof(PARTICLE)	//정점 하나의 크기
	);

	if (m_dwVbOffset >= m_dwVbSize)
		m_dwVbOffset = 0;

	PARTICLE* v = nullptr;

	m_pVB->Lock(
		m_dwVbOffset * sizeof(PARTICLE),						// 잠그기 시작 위치	(정점 개수 * 정점하나의 크기)
		m_dwVbBatchSize * sizeof(PARTICLE),						// 잠글 메모리 크기	(배치 사이즈(512) * 정점 하나의 크기)
		(void**)&v,												// 메모리를 가리키는 포인터를 받을 곳
		m_dwVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD	// 잠금 방식
	);

	// 배치 한 칸 중 어디까지 그렸는지 세는 변수
	DWORD numParticlesInBatch = 0;

	for (auto iter = m_Particles.begin(); iter != m_Particles.end(); ++iter)
	{
		// 죽은 파티클 제외
		if (!iter->bIsAlive)
			continue;

		// m_Particles(Attribute)의 위치와 색 정보만 빼서 PARTICLE 구조체(버텍스 구조체)에 넣음
		v->vPosition = iter->vPosition;
		v->dwColor = (D3DCOLOR)iter->color;
		++v;

		++numParticlesInBatch;

		// 배치 한 칸의 사이즈를 넘어갔다면 배치 사이즈 만큼 그리고 다시 Lock설정
		if (numParticlesInBatch == m_dwVbBatchSize)
		{
			m_pVB->Unlock();

			m_pGraphicDev->DrawPrimitive(
				D3DPT_POINTLIST,	// 그릴 도형의 종류
				m_dwVbOffset,		// 몇 번째 정점부터 그릴지
				m_dwVbBatchSize		// 도형을 몇 개 그릴 지
			);

			m_dwVbOffset += m_dwVbBatchSize;

			// 배치 마지막 칸을 넘어갔다면 처음으로
			if (m_dwVbOffset >= m_dwVbSize)
				m_dwVbOffset = 0;

			m_pVB->Lock(
				m_dwVbOffset * sizeof(PARTICLE),
				m_dwVbBatchSize * sizeof(PARTICLE),
				(void**)&v,
				m_dwVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

			numParticlesInBatch = 0;
		}
	}

	m_pVB->Unlock();
	// 배치 개수와 리스트의 크기가 딱 맞아 떨어지지 않는 경우
	// 즉 numParticlesInBatch이 아직 남아있다면 그림 그려주기
	if (numParticlesInBatch)
	{
		m_pGraphicDev->DrawPrimitive(
			D3DPT_POINTLIST,
			m_dwVbOffset,
			numParticlesInBatch);
	}
	// 다음 배치 설정
	m_dwVbOffset += m_dwVbBatchSize;

	PostRender_Particle();
}

void PSystem::PostRender_Particle()
{
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, true);				//조명
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);		
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool PSystem::IsDead()
{
	for (auto& p : m_Particles)
		if (p.bIsAlive) return false;
	return true;
}

void PSystem::ResetAll()
{
	for (auto& p : m_Particles)
		ResetParticle(&p);
}
void PSystem::RemoveDeadParticles()
{
	for (auto iter = m_Particles.begin(); iter != m_Particles.end();)
	{
		if (!iter->bIsAlive)
			iter = m_Particles.erase(iter);
		else
			++iter;
	}
}

void PSystem::OnLostDevice()
{
	Safe_Release(m_pVB);   
}

void PSystem::OnResetDevice()
{
	m_pGraphicDev->CreateVertexBuffer(
		m_dwVbSize * sizeof(PARTICLE),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		nullptr);
	m_dwVbOffset = 0;
}

void PSystem::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pGraphicDev);
}
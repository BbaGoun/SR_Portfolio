#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "DirectXCollision.h"

BEGIN(Engine)
class ENGINE_DLL PSystem : public CBase
{
protected:
	explicit			PSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~PSystem();


public:
	// 버텍스 버퍼, 텍스처 초기화
	virtual			HRESULT		Ready_PSystem();		

	// m_Particles리스트를 순회하며 파티클의 값만 초기상태로 되돌림 
	// 죽은 파티클의 메모리를 해제하지 않고, 재활용.
	virtual			void		Reset();									


	// Reset()함수에서 m_Particles를 초기화할 때 사용. 
	// 순수가상함수로 만들어서 PSystem을 상속받는 다른 파티클의 초기 상태를 따로 설정하게만듬
	virtual			void		ResetParticle(Attribute* attribute) = 0;	 

	// ResetParticle()을 호출하여 생성한 파티클 하나를 리스트에 추가(파티클 개수 자체를 늘림)
	virtual			void		AddParticle();

	// 자식클래스에서 몸체 완성
	virtual			void		Update_PSystme(float timeDelta) = 0;

	// 파티클에 필요한 RenderState 설정 ON
	virtual			void		PreRender_Particle();

	// 파티클 Render
	virtual			void		Render_Particle();

	// 파티클에 사용한 RenderState 설정 OFF
	virtual			void		PostRender_Particle();

	//	m_Particles가 비어있는지 확인
	bool IsEmpty() { return m_Particles.empty(); }

	// m_Particles에 들어있는 파티클들이 전부 죽었는지
	bool IsDead();

	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();

	virtual			void		ResetAll();

protected:
	virtual void RemoveDeadParticles();
	virtual void Free();

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_vec3						m_vOrigin;
	DirectX::BoundingBox		m_BoundingBox;
	float						m_fEmitRate;   // 새 파티클이 시스템에 추가되는 속도(비율)
	float						m_fSize;       // 파티클의 크기(화면에 보이는 픽셀 크기)
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	list<Attribute>				m_Particles;
	int							m_iMaxParticles; // max allowed particles system can have


	DWORD m_dwVbSize;      // 버텍스 버퍼의 크기
	DWORD m_dwVbOffset;    // 버텍스 버퍼에서의 Lock 위치
	DWORD m_dwVbBatchSize; // m_dwVbOffset부터 시작해서 잠글 정점의 개수
};

END
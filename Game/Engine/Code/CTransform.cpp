#include "CTransform.h"

CTransform::CTransform() : CComponent()
, m_qRotation({ 0, 0, 0, 1})
, m_vScale({1, 1, 1})
{
	ZeroMemory(&m_vInfo, sizeof(_vec3) * INFO_END);
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
, m_qRotation({ 0, 0, 0, 1 })
, m_vScale({ 1, 1, 1 })
{
	ZeroMemory(&m_vInfo, sizeof(_vec3) * INFO_END);
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs):CComponent(rhs)
, m_qRotation(rhs.m_qRotation)
, m_vScale(rhs.m_vScale)
{
	for (int i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

_int CTransform::Update_Component(const _float& fTimeDelta)
{
	// 1. 월드 행렬의 초기화?
	D3DXMatrixIdentity(&m_matWorld);

	// 2. Right, Up, Look의 초기화
	for (int i = 0; i < INFO_POS; ++i) {
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	// 3. 크기 적용
	for (int i = 0; i < INFO_POS; ++i) {
		m_vInfo[i] *= *(((float*)&m_vScale) + i);
	}

	// 4. 회전 적용
	_matrix matRotQ;
	D3DXMatrixRotationQuaternion(&matRotQ, &m_qRotation);
	
	for (int i = 0; i < INFO_POS; ++i) {
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotQ);
	}

	// 5. 월드 행렬 생성
	// 이동은 이 GameObject에서 변경됨
	for (int i = 0; i < INFO_END; ++i) {
		memcpy(&m_matWorld.m[i][0], m_vInfo[i], sizeof(_vec3));
	}

	return 0;
}

void CTransform::LateUpdate_Component(const _float& fTimeDelta)
{
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (int i = 0; i < INFO_END; ++i) {
		memcpy(m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	return S_OK;
}

void CTransform::FollowObj(_vec3* pPos, _float _fSpeed, _float _fTimeDelta)
{
	_vec3 vFollowDir = *pPos - m_vInfo[INFO_POS];
	
	_float fLength = D3DXVec3Length(&vFollowDir);

	if (fLength < 0.05f)
		return;

	float step = min(fLength, _fSpeed * _fTimeDelta);

	D3DXVec3Normalize(&vFollowDir, &vFollowDir);

	m_vInfo[INFO_POS] += vFollowDir * step;

	_matrix matScale, matRot, matTrans;

	// 크기
	D3DXMatrixScaling(&matScale, 
		m_vScale.x, 
		m_vScale.y, 
		m_vScale.z);

	// 회전
	GetFollowRotation(&vFollowDir, &matRot);

	// 이동
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

_matrix* CTransform::GetFollowRotation(_vec3* pFollowDir, _matrix* _pRot)
{
	// 플레이어를 향하는 방향과 현재 삼각형이 향하는 방향의 외적
	// 현재 삼각형이 향하는 방향에서 플레이어를 향하는 방향으로 바꾸는 축을 알아낸다.
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &m_vInfo[INFO_UP], pFollowDir);

	if (vCross.x == 0 && vCross.y == 0 && vCross.z == 0)
		return D3DXMatrixIdentity(_pRot);

	// 현재 삼각형이 향하는 방향에서 플레이어를 향하는 방향으로 회전할 때, 사잇각을 알아낸다.
	float theta = acosf(D3DXVec3Dot(&m_vInfo[INFO_UP], pFollowDir));

	// 해당 축으로 사잇각 만큼 회전하는 회전 행렬을 구함
	return	D3DXMatrixRotationAxis(_pRot, &vCross, theta);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pTransformComp = new CTransform(pGraphicDev);

	if (FAILED(pTransformComp->Ready_Transform())) {
		Safe_Release(pTransformComp);
		MSG_BOX("CTransform Create Fail");
		return nullptr;
	}

	return pTransformComp;
}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

void CTransform::Free()
{
	CComponent::Free();
}
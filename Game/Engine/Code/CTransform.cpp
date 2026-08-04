#include "CTransform.h"
#include "CGameObject.h"

CTransform::CTransform() : CComponent()
, m_quaternion({ 0, 0, 0, 1 })
, m_vScale({ 1, 1, 1 })
, m_bDirty(true)
{
	ZeroMemory(&m_vInfo, sizeof(_vec3) * INFO_END);
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
, m_quaternion({ 0, 0, 0, 1 })
, m_vScale({ 1, 1, 1 })
, m_bDirty(true)
{
	ZeroMemory(&m_vInfo, sizeof(_vec3) * INFO_END);
	D3DXMatrixIdentity(&m_matWorld);
}

CTransform::CTransform(const CTransform& rhs):CComponent(rhs)
, m_quaternion(rhs.m_quaternion)
, m_vScale(rhs.m_vScale)
, m_bDirty(rhs.m_bDirty)
{
	for (int i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
}

CTransform::~CTransform()
{
}

_int CTransform::Update_Component(const _float& fDeltaTime)
{
	return 0;
}

void CTransform::LateUpdate_Component(const _float& fDeltaTime)
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

_matrix* CTransform::Get_World()
{
	if (!m_bDirty)
		return &m_matWorld;

	// 1. 월드 행렬의 초기화
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
	D3DXMatrixRotationQuaternion(&matRotQ, &m_quaternion);

	for (int i = 0; i < INFO_POS; ++i) {
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotQ);
	}

	// 5. 월드 행렬 생성
	// 이동은 컴포넌트를 수정한 GameObject에서 직접 수행됨
	// 월드 행렬에 이동 상태를 옮길 뿐
	for (int i = 0; i < INFO_END; ++i) {
		memcpy(&m_matWorld.m[i][0], m_vInfo[i], sizeof(_vec3));
	}

	// 6. 부모의 월드 행렬 가져오기
	if (m_pOwner->Get_Parent() != nullptr) {
		_matrix* parentWorld = m_pOwner->Get_Parent()->Get_Transform()->Get_World();

		// 7. 로컬 월드 행렬 * 부모의 월드 행렬 = 실제 월드 행렬 
		m_matWorld *= (*parentWorld);
	}

	m_bDirty = false;

	return &m_matWorld;
}

void CTransform::FollowObj(_vec3* pPos, _float _fSpeed, _float _fDeltaTime)
{
	_vec3 pos;
	Get_Info(INFO_POS, &pos);
	_vec3 vFollowDir = *pPos - pos;
	
	_float fLength = D3DXVec3Length(&vFollowDir);

	float step = min(fLength, _fSpeed * _fDeltaTime);

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

	m_bDirty = false;

	// 서브 트리만 설정 (자신은 방금 계산했으므로)
	vector<CGameObject*> vecChildren = m_pOwner->Get_Children();

	for (auto& child : vecChildren) {
		child->Get_Transform()->Set_Dirty();
	}
}

_matrix* CTransform::GetFollowRotation(_vec3* pFollowDir, _matrix* _pRot)
{
	// 플레이어를 향하는 방향과 현재 삼각형이 향하는 방향의 외적
	// 현재 삼각형이 향하는 방향에서 플레이어를 향하는 방향으로 바꾸는 축을 알아낸다.
	_vec3 vCross;
	D3DXVec3Cross(&vCross, &m_vInfo[INFO_LOOK], pFollowDir);

	if (vCross.x == 0 && vCross.y == 0 && vCross.z == 0)
		return D3DXMatrixIdentity(_pRot);

	// 현재 삼각형이 향하는 방향에서 플레이어를 향하는 방향으로 회전할 때, 사잇각을 알아낸다.
	float theta = acosf(D3DXVec3Dot(&m_vInfo[INFO_LOOK], pFollowDir));

	// 해당 축으로 사잇각 만큼 회전하는 회전 행렬을 구함
	return	D3DXMatrixRotationAxis(_pRot, &vCross, theta);
}

void CTransform::Set_Dirty()
{
	if (!m_bDirty) {
		// 자신 설정
		m_bDirty = true;
		// 서브 트리 설정
		vector<CGameObject*> vecChildren = m_pOwner->Get_Children();

		for (auto& child : vecChildren) {
			child->Get_Transform()->Set_Dirty();
		}
	}
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
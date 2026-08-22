#include "CTransform.h"
#include "CGameObject.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
, m_localQuaternion({ 0, 0, 0, 1 })
, m_worldQuaternion({ 0, 0, 0, 1 })
, m_vScale({ 1, 1, 1 })
, m_bDirty(true)
{
	m_eID = ID_STATIC;
	m_eKind = CK_TRANSFORM;

	ZeroMemory(&m_vInfo, sizeof(_vec3) * INFO_END);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matLocalWorld);
	D3DXMatrixIdentity(&m_matBillboard);	
}

CTransform::CTransform(const CTransform& rhs):CComponent(rhs)
, m_localQuaternion(rhs.m_localQuaternion)
, m_worldQuaternion(rhs.m_worldQuaternion)
, m_vScale(rhs.m_vScale)
, m_bDirty(rhs.m_bDirty)
{
	m_eID = ID_STATIC;
	m_eKind = CK_TRANSFORM;

	for (int i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
	m_matLocalWorld = rhs.m_matLocalWorld;
	m_matBillboard = rhs.m_matBillboard;
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

void CTransform::Set_LocalWorld(_matrix* _MatLocal)
{
	// 크기 분해
	_vec3 vRight, vUp, vLook;
	memcpy(&vRight, &_MatLocal->m[0], sizeof(_vec3));
	memcpy(&vUp, &_MatLocal->m[1], sizeof(_vec3));
	memcpy(&vLook, &_MatLocal->m[2], sizeof(_vec3));
	m_vScale = { D3DXVec3Length(&vRight), D3DXVec3Length(&vUp), D3DXVec3Length(&vLook) };

	// 회전 분해
	_matrix matRot;
	D3DXMatrixIdentity(&matRot);
	memcpy(&matRot.m[0], D3DXVec3Normalize(&vRight, &vRight), sizeof(_vec3));
	memcpy(&matRot.m[1], D3DXVec3Normalize(&vUp, &vUp), sizeof(_vec3));
	memcpy(&matRot.m[2], D3DXVec3Normalize(&vLook, &vLook), sizeof(_vec3));

	D3DXQuaternionRotationMatrix(&m_localQuaternion, &matRot);

	// 이동 분해
	memcpy(&m_vInfo[INFO_POS], &_MatLocal->m[3], sizeof(_vec3));

	Set_Dirty();
}

_matrix* CTransform::Get_World()
{
	if (!m_bDirty)
		return &m_matWorld;

	// 1. 월드 행렬의 초기화
	D3DXMatrixIdentity(&m_matLocalWorld);

	// 2. Right, Up, Look의 초기화
	for (int i = 0; i < INFO_POS; ++i) {
		memcpy(&m_vInfo[i], &m_matLocalWorld.m[i][0], sizeof(_vec3));
	}

	// 3. 크기 적용
	for (int i = 0; i < INFO_POS; ++i) {
		m_vInfo[i] *= *(((float*)&m_vScale) + i);
	}

	// 4. 회전 적용
	_matrix matRotQ;
	D3DXMatrixRotationQuaternion(&matRotQ, &m_localQuaternion);

	for (int i = 0; i < INFO_POS; ++i) {
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotQ);
	}

	// 4-1. 빌보드 행렬 적용(Set_Billboard()함수를 호출하지 않은 경우 빌보드 행렬은 항등행렬)
	for (int i = 0; i < INFO_POS; ++i) {
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &m_matBillboard);
	}

	// 5. 월드 행렬 생성
	// 이동은 컴포넌트를 수정한 GameObject에서 직접 수행됨
	// 월드 행렬에 이동 상태를 옮길 뿐
	for (int i = 0; i < INFO_END; ++i) {
		memcpy(&m_matLocalWorld.m[i][0], m_vInfo[i], sizeof(_vec3));
	}

	CGameObject* pParent = m_pOwner->Get_Parent();
	m_matWorld = m_matLocalWorld;

	if (pParent == nullptr)
	{
		m_worldQuaternion = m_localQuaternion;
		m_bDirty = false;
		return &m_matWorld;
	}

	// 6. 부모의 월드 행렬/쿼터니언 가져오기
	_matrix* parentWorld = pParent->Get_Transform()->Get_World();
	_quaternion qParent = pParent->Get_Transform()->Get_WorldQuaternion();
	m_worldQuaternion = m_localQuaternion * qParent;

	// 7. 로컬 월드 행렬 * 부모의 월드 행렬 = 실제 월드 행렬 
	m_matWorld *= (*parentWorld);

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
	_vec3 vLook;
	Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Cross(&vCross, &vLook, pFollowDir);
	
	if (vCross.x == 0 && vCross.y == 0 && vCross.z == 0)
		return D3DXMatrixIdentity(_pRot);

	//if (D3DXVec3Length(&vCross) <= 0.001f)	// 테스트
	//	return D3DXMatrixIdentity(_pRot);	 





	// 현재 삼각형이 향하는 방향에서 플레이어를 향하는 방향으로 회전할 때, 사잇각을 알아낸다.
	float theta = acosf(D3DXVec3Dot(&m_vInfo[INFO_LOOK], pFollowDir));

	// 해당 축으로 사잇각 만큼 회전하는 회전 행렬을 구함
	return	D3DXMatrixRotationAxis(_pRot, &vCross, theta);
}

_quaternion* CTransform::GetFollowQuaternion(_vec3* _pFollowDir, _quaternion* _pQuater)
{
	_vec3 vFixUp, vFixRight, vFixLook;
	vFixUp = { 0.f, 1.f, 0.f };			

	D3DXVec3Normalize(&vFixLook, _pFollowDir);

	D3DXVec3Cross(&vFixRight, &vFixUp, &vFixLook);
	D3DXVec3Cross(&vFixUp, &vFixLook, &vFixRight);

	D3DXVec3Normalize(&vFixUp, &vFixUp);
	D3DXVec3Normalize(&vFixRight, &vFixRight);

	_matrix matFixRot;
	D3DXMatrixIdentity(&matFixRot);

	matFixRot._11 = vFixRight.x;
	matFixRot._12 = vFixRight.y;
	matFixRot._13 = vFixRight.z;

	matFixRot._21 = vFixUp.x;
	matFixRot._22 = vFixUp.y;
	matFixRot._23 = vFixUp.z;

	matFixRot._31 = vFixLook.x;
	matFixRot._32 = vFixLook.y;
	matFixRot._33 = vFixLook.z;

	return D3DXQuaternionRotationMatrix(_pQuater, &matFixRot);
}

void CTransform::Chase_Target(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	vDir = *pPos - m_vInfo[INFO_POS];

	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	matRot = *Compute_Lookattarget(pPos);

	m_matWorld = matScale * matRot * matTrans;
}

_matrix* CTransform::Compute_Lookattarget(const _vec3* pPos)
{
	_vec3	vDir = *pPos - m_vInfo[INFO_POS];

	D3DXMATRIX	matRot;
	_vec3	vAxis, vUp;
	return D3DXMatrixRotationAxis(&matRot,
								  D3DXVec3Cross(&vAxis, &m_vInfo[INFO_LOOK], &vDir),
								  acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
										D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
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

void CTransform::Set_Billboard(_matrix* pMatView)
{
	D3DXMatrixIdentity(&m_matBillboard);

	m_matBillboard._11 = pMatView->_11;
	m_matBillboard._13 = pMatView->_13;
	m_matBillboard._31 = pMatView->_31;
	m_matBillboard._33 = pMatView->_33;

	D3DXMatrixInverse(&m_matBillboard, nullptr, &m_matBillboard); 
	Set_Dirty();
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
#include "CObj.h"

#include <fstream>
#include <sstream>

CObj::CObj() : CComponent()
, m_pVB(nullptr), m_pIB(nullptr)
, m_dwVtxSize(0), m_dwVtxCnt(0)
, m_dwTriCnt(0), m_dwFVF(0)
, m_dwIdxCnt(0), m_IdxFmt(D3DFMT_INDEX32)
, m_pBaseColorMap(nullptr)
{
}

CObj::CObj(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
, m_pVB(nullptr), m_pIB(nullptr)
, m_dwVtxSize(0), m_dwVtxCnt(0)
, m_dwTriCnt(0), m_dwFVF(0)
, m_dwIdxCnt(0), m_IdxFmt(D3DFMT_INDEX32)
, m_pBaseColorMap(nullptr)
{
}

CObj::CObj(const CObj& rhs) : CComponent(rhs)
, m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
, m_dwVtxSize(rhs.m_dwVtxSize), m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwTriCnt(rhs.m_dwTriCnt), m_dwFVF(rhs.m_dwFVF)
, m_dwIdxCnt(rhs.m_dwIdxCnt), m_IdxFmt(rhs.m_IdxFmt)
, m_pBaseColorMap(rhs.m_pBaseColorMap)
{
	// 원본(프로토타입)의 버퍼/텍스처를 공유하므로 참조 카운트를 올린다.
	if (m_pVB)			m_pVB->AddRef();
	if (m_pIB)			m_pIB->AddRef();
	if (m_pBaseColorMap) m_pBaseColorMap->AddRef();
}

CObj::~CObj()
{
}

namespace
{
	// 유니코드 경로를 ifstream / D3DXCreateTextureFromFileA 용 멀티바이트 문자열로 변환
	string ToMultiByte(const WCHAR* wstr)
	{
		if (nullptr == wstr)
			return string();

		int iLen = WideCharToMultiByte(CP_ACP, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
		if (iLen <= 1)
			return string();

		string result(iLen - 1, '\0');
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, &result[0], iLen, nullptr, nullptr);
		return result;
	}
}

HRESULT CObj::LoadObj(const WCHAR* pObjPath, _vec3 vAngle)
{
	string strObjPath = ToMultiByte(pObjPath);

	ifstream fin(strObjPath.c_str());
	if (false == fin.is_open())
	{
		WCHAR wMessage[256];
		swprintf_s(wMessage, L"LoadObj Open Fail : %s", pObjPath);
		MSG_BOXF(wMessage);
		return E_FAIL;
	}

	// .mtl / .png 를 찾기 위한 디렉터리 경로("../Resource/Plane01/")
	string strDir;
	{
		size_t iPos = strObjPath.find_last_of("/\\");
		if (string::npos != iPos)
			strDir = strObjPath.substr(0, iPos + 1);
	}

	// .obj 원본 데이터
	vector<_vec3>	vecPos;
	vector<_vec2>	vecUV;

	// 재인덱싱 결과(최종 버퍼)
	vector<VTXTEX>	vecVtx;
	vector<DWORD>	vecIdx;
	unordered_map<string, DWORD> mapLookup;	// "v/vt/vn" -> 최종 정점 인덱스

	string strMtlFile;

	string strLine;
	while (getline(fin, strLine))
	{
		if (strLine.empty())
			continue;

		istringstream ss(strLine);
		string strTag;
		ss >> strTag;

		if ("v" == strTag)			// 정점 위치
		{
			_vec3 vPos;
			ss >> vPos.x >> vPos.y >> vPos.z;
			vecPos.push_back(vPos);
		}
		else if ("vt" == strTag)	// 텍스처 좌표
		{
			_vec2 vUV;
			ss >> vUV.x >> vUV.y;
			vecUV.push_back(vUV);
		}
		else if ("mtllib" == strTag)
		{
			ss >> strMtlFile;
		}
		else if ("f" == strTag)		// 면 (정점/텍스처/법선 인덱스)
		{
			// OBJ 는 위치/UV/법선 인덱스가 따로이므로,
			// (v/vt/vn) 조합마다 고유 정점을 만들어 하나의 인덱스로 재구성한다.
			vector<DWORD> vecFace;

			string strCorner;
			while (ss >> strCorner)
			{
				auto iter = mapLookup.find(strCorner);
				if (mapLookup.end() != iter)
				{
					vecFace.push_back(iter->second);
					continue;
				}

				int iPos = 0, iUV = 0, iNormal = 0;
				// 지원 포맷: "v", "v/vt", "v//vn", "v/vt/vn"
				if (string::npos != strCorner.find("//"))
					sscanf_s(strCorner.c_str(), "%d//%d", &iPos, &iNormal);
				else
					sscanf_s(strCorner.c_str(), "%d/%d/%d", &iPos, &iUV, &iNormal);

				VTXTEX vtx{};

				if (iPos > 0 && iPos <= (int)vecPos.size())	// OBJ 인덱스는 1부터 시작 → -1 보정
					vtx.vPosition = vecPos[iPos - 1];

				if (iUV > 0 && iUV <= (int)vecUV.size())
					vtx.vTexUV = { vecUV[iUV - 1].x, 1.0f - vecUV[iUV - 1].y };	// D3D 는 V축 원점이 위쪽
				else
					vtx.vTexUV = { 0.f, 0.f };

				DWORD dwNewIdx = (DWORD)vecVtx.size();
				vecVtx.push_back(vtx);
				mapLookup[strCorner] = dwNewIdx;
				vecFace.push_back(dwNewIdx);
			}

			// 다각형(코너 3개 이상)은 삼각형 팬으로 분할
			for (size_t k = 1; k + 1 < vecFace.size(); ++k)
			{
				vecIdx.push_back(vecFace[0]);
				vecIdx.push_back(vecFace[k]);
				vecIdx.push_back(vecFace[k + 1]);
			}
		}
	}
	fin.close();

	if (vecVtx.empty() || vecIdx.empty())
	{
		MSG_BOX("LoadObj : Geometry Parse Fail");
		return E_FAIL;
	}

	// ── 로드한 매쉬 회전 ───────────────────────────

	if (vAngle.x == 0 && vAngle.y == 0 && vAngle.z == 0);
	else {
		_matrix matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, 
			D3DXToRadian(vAngle.y), 
			D3DXToRadian(vAngle.x),
			D3DXToRadian(vAngle.z));
		for (auto& vtx : vecVtx) {
			D3DXVec3TransformCoord(&vtx.vPosition, &vtx.vPosition, &matRot);
		}
	}

	// ── 버퍼 메타 ──────────────────────────────
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = (DWORD)vecVtx.size();
	m_dwFVF = FVF_TEX;
	m_dwIdxCnt = (DWORD)vecIdx.size();
	m_dwTriCnt = m_dwIdxCnt / 3;
	m_IdxFmt = D3DFMT_INDEX32;	// 정점이 6만개를 넘을 수 있으므로 32비트 인덱스

	// ── 정점 버퍼 ──────────────────────────────
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxCnt * m_dwVtxSize,
		D3DUSAGE_WRITEONLY,
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0)))
		return E_FAIL;

	void* pData = nullptr;
	m_pVB->Lock(0, 0, &pData, 0);
	memcpy(pData, vecVtx.data(), m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();

	// ── 인덱스 버퍼 ────────────────────────────
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxCnt * sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		0)))
		return E_FAIL;

	m_pIB->Lock(0, 0, &pData, 0);
	memcpy(pData, vecIdx.data(), sizeof(DWORD) * m_dwIdxCnt);
	m_pIB->Unlock();

	// ── .mtl 로부터 텍스처 로드 ────────────────
	if (false == strMtlFile.empty())
		LoadMaterialTexture(strDir, strMtlFile);

	return S_OK;
}

HRESULT CObj::LoadMaterialTexture(const string& strDir, const string& strMtlFile)
{
	ifstream fin((strDir + strMtlFile).c_str());
	if (false == fin.is_open())
	{
		MSG_BOX("LoadObj : mtl Open Fail");
		return E_FAIL;
	}

	string strTexFile;
	string strLine;
	while (getline(fin, strLine))
	{
		istringstream ss(strLine);
		string strTag;
		ss >> strTag;

		if ("map_Kd" == strTag)		// 디퓨즈(색) 텍스처 파일명
		{
			ss >> strTexFile;
			break;
		}
	}
	fin.close();

	if (strTexFile.empty())
		return E_FAIL;

	string strTexPath = strDir + strTexFile;

	if (FAILED(D3DXCreateTextureFromFileA(m_pGraphicDev, strTexPath.c_str(), &m_pBaseColorMap)))
	{
		MSG_BOX("LoadObj : map_Kd Texture Load Fail");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CObj::Ready_Obj()
{
	return S_OK;
}

void CObj::Render_Obj()
{
	m_pGraphicDev->SetTexture(0, m_pBaseColorMap);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);

	// MagicaVoxel(오른손 좌표계) 모델은 와인딩이 뒤집혀 안팎이 반대일 수 있으므로 양면 렌더.
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CComponent* CObj::Clone()
{
	return nullptr;
}

void CObj::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pBaseColorMap);

	CComponent::Free();
}

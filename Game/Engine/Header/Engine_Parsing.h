#ifndef Engine_Parsing_h__
#define Engine_Parsing_h__

#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "CProtoMgr.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CScene.h"
#include "CManagement.h"

namespace Engine
{
	inline bool ToAbsPath(const wchar_t* in, wchar_t* out, DWORD outChars)
	{
		return GetFullPathNameW(in, outChars, out, nullptr) != 0;
	}

	inline bool ToRelFromCwd(const wchar_t* absPath, wchar_t* out, DWORD outChars)
	{
		wchar_t cwd[MAX_PATH] = {};
		GetCurrentDirectoryW(MAX_PATH, cwd);
		PathAddBackslashW(cwd);  // 디렉터리로 인식시키려면 필수
		wchar_t rel[MAX_PATH] = {};
		if (!PathRelativePathToW(rel, cwd, FILE_ATTRIBUTE_DIRECTORY,
			absPath, FILE_ATTRIBUTE_NORMAL))
			return false;
		wcscpy_s(out, outChars, rel);
		return true;
	}

	inline void writeIndent(FILE* f, int depth)
	{
		for (int i = 0; i < depth; ++i)
			fwprintf(f, L"    ");
	};

    inline wchar_t* TrimPrefabLine(wchar_t* s)
    {
        if (s == nullptr)
            return s;
        // UTF의 BOM 제거
        if (s[0] == 0xFEFF)
            ++s;
        // 앞쪽 공백 제거
        while (*s == L' ' || *s == L'\t')
            ++s;
        size_t n = wcslen(s);

        // 뒷쪽 공백 및 개행 제거
        while (n > 0 && (s[n - 1] == L'\n' || s[n - 1] == L'\r' || s[n - 1] == L' ' || s[n - 1] == L'\t'))
            s[--n] = 0;
        return s;
    }

    struct FileReadState
    {
        // 파일 포인터, 현재 줄 버퍼, Unget 여부
        FILE* fp = nullptr;
        wchar_t buf[1024] = {};
        bool    pending = false;

        // 다음 줄을 읽어 공백/개행을 제거한 뒤 out에 포인터를 반환
        bool Next(wchar_t*& out)
        {
            if (!pending)
            {
                if (!fgetws(buf, 1024, fp))
                    return false;
            }
            pending = false;
            out = TrimPrefabLine(buf);
            return true;
        }

        // 방금 읽은 줄을 되돌린다. 속성 값에 대한 줄이 아니면
        // 호출하여 상위 루프가 다시 읽게 한다.
        void Unget() { pending = true; }
    };

    // 저장된 proto 태그 문자열이 현재 ProtoMgr에 있는지 확인하고, 
    // 있으면 맵 안의 포인터를 반환
    inline const WCHAR* InternProtoTag(const WCHAR* tag)
    {
        if (tag == nullptr || tag[0] == L'\0')
            return nullptr;

        for (auto& proto : CProtoMgr::GetInstance()->Get_Prototypes())
        {
            if (!lstrcmp(proto.second.tag, tag))
                return proto.second.tag;
        }
		wstring s = tag;
		s += L" proto not set";
		MSG_BOXF(s.c_str());
        return nullptr;
    }

    // kind, proto, mapTag를 분리해낸다.
    inline void ExtractCompField(const wchar_t* line, const wchar_t* key, wchar_t* out, int outChars)
    {
        out[0] = 0;
        wchar_t pat[64] = {};
        swprintf_s(pat, L"%s=", key);
        const wchar_t* p = wcsstr(line, pat);
        if (p == nullptr)
            return;
        p += wcslen(pat);
        int i = 0;
        while (p[i] && p[i] != L' ' && p[i] != L'\t' && i < outChars - 1)
        {
            out[i] = p[i];
            ++i;
        }
        out[i] = 0;
    }

    // COMP 줄 다음 줄들을 읽어 컴포넌트 속성 값을 적용
    // COMP, OBJECT, ENDOBJECT 등을 만나면 Unget()을 호출하고 반환.
    inline void ApplyCompProperties(FileReadState& st, CGameObject* pObj, CComponent* pCom)
    {
        ControlPoint cp = {};
        VTXTEX vtxTex = {};
        FACE32 face = {};
        TrackNode trackNode = {};
        TrackEdge trackEdge = {};
        ControlPoint trackControl = {};

        wchar_t* t = nullptr;
        while (st.Next(t))
        {
            float fX = 0.f, fY = 0.f, fZ = 0.f, fW = 0.f;
            int iX = 0, iY = 0, iZ = 0, iW = 0;
            unsigned int uX = 0, uY = 0, uZ = 0;
            int   trig = 0;

            // Transform
            if (swscanf_s(t, L"pos=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                if (CTransform* pTF = pObj->Get_Transform())
                    pTF->Set_Pos(_vec3(fX, fY, fZ));
            }
            else if (swscanf_s(t, L"rot=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                // 런타임 회전은 quat가 기준. rot는 저장용 미리보기.
            }
            else if (swscanf_s(t, L"quat=%f %f %f %f", &fX, &fY, &fZ, &fW) == 4)
            {
                D3DXQUATERNION q(fX, fY, fZ, fW);
				if (CTransform* pTF = pObj->Get_Transform())
					pTF->Set_DefaultQuaternion(&q);
            }
            else if (swscanf_s(t, L"scale=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                if (CTransform* pTF = pObj->Get_Transform())
                    pTF->Set_Scale(_vec3(fX, fY, fZ));
            }

            // Collider
            else if (swscanf_s(t, L"trigger=%d", &trig) == 1)
            {
                if (CCollider* pCol = dynamic_cast<CCollider*>(pCom))
                    pCol->SetIsTrigger(trig != 0);
            }
            else if (swscanf_s(t, L"offset=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                if (CCube_Collider* pBox = dynamic_cast<CCube_Collider*>(pCom))
                    pBox->Set_Offset(_vec3(fX, fY, fZ));
                else if (CSphere_Collider* pSphere = dynamic_cast<CSphere_Collider*>(pCom))
                    pSphere->Set_Offset(_vec3(fX, fY, fZ));
            }
            else if (swscanf_s(t, L"extents=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                if (CCube_Collider* pBox = dynamic_cast<CCube_Collider*>(pCom))
                    pBox->Set_Extents(_vec3(fX, fY, fZ));
            }
            else if (swscanf_s(t, L"radius=%f", &fX) == 1)
            {
                if (CSphere_Collider* pSphere = dynamic_cast<CSphere_Collider*>(pCom))
                    pSphere->Set_Radius(fX);
            }

            // Spline
			else if (swscanf_s(t, L"SampleUnit=%f", &fX) == 1) {
				if (CSpline* pSpline = dynamic_cast<CSpline*>(pCom))
					pSpline->Set_SampleUnit(fX);
			}
			else if (swscanf_s(t, L"TextureUnit=%f", &fX) == 1) {
				if (CSpline* pSpline = dynamic_cast<CSpline*>(pCom))
					pSpline->Set_TextureUnit(fX);
			}
            else if (!wcscmp(t, L"CONTROL_POINT"))
            {
                cp = {};
                if (CSpline* pSpline = dynamic_cast<CSpline*>(pCom))
                    cp.id = pSpline->GenerateId();
            }
            else if (swscanf_s(t, L"cp_pos=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                cp.position = { fX, fY, fZ };
            }
            else if (swscanf_s(t, L"cp_bank=%f", &fX) == 1)
            {
                cp.bank = fX;
            }
            else if (swscanf_s(t, L"cp_width=%f", &fX) == 1)
            {
                cp.width = fX;
            }
            else if (swscanf_s(t, L"cp_depth=%f", &fX) == 1)
            {
                cp.depth = fX;
            }
            else if (!wcscmp(t, L"END_CONTROL_POINT"))
            {
                if (CSpline* pSpline = dynamic_cast<CSpline*>(pCom)) {
                    auto& vecPoints = pSpline->Get_ControlPoints();
                    vecPoints.push_back(cp);
                }
            }

            // TrackGraph
            else if (swscanf_s(t, L"tg_sample_unit=%f", &fX) == 1)
            {
                if (CTrackGraph* pTrackGraph = dynamic_cast<CTrackGraph*>(pCom))
                    pTrackGraph->Set_SampleUnit(fX);
            }
            else if (!wcscmp(t, L"TRACK_NODE"))
            {
                trackNode = {};
            }
            else if (swscanf_s(t, L"tn_id=%u", &uX) == 1)
            {
                trackNode.id = static_cast<NodeId>(uX);
            }
            else if (swscanf_s(t, L"tn_pos=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                trackNode.position = { fX, fY, fZ };
            }
            else if (swscanf_s(t, L"tn_start=%d", &iX) == 1)
            {
                trackNode.bStart = iX != 0;
            }
            else if (swscanf_s(t, L"tn_finish=%d", &iX) == 1)
            {
                trackNode.bFinish = iX != 0;
            }
            else if (!wcscmp(t, L"END_TRACK_NODE"))
            {
                if (CTrackGraph* pTrackGraph = dynamic_cast<CTrackGraph*>(pCom))
                    pTrackGraph->Get_Nodes().push_back(trackNode);
            }
            else if (!wcscmp(t, L"TRACK_EDGE"))
            {
                trackEdge = {};
            }
            else if (swscanf_s(t, L"te_id=%u", &uX) == 1)
            {
                trackEdge.id = static_cast<EdgeId>(uX);
            }
            else if (swscanf_s(t, L"te_nodes=%u %u", &uX, &uY) == 2)
            {
                trackEdge.fromNode = static_cast<NodeId>(uX);
                trackEdge.toNode = static_cast<NodeId>(uY);
            }
            else if (swscanf_s(t, L"te_default_size=%f %f", &fX, &fY) == 2)
            {
                trackEdge.fWidthDefault = fX;
                trackEdge.fHeightDefault = fY;
            }
            else if (swscanf_s(t, L"te_cost_bias=%f", &fX) == 1)
            {
                trackEdge.fCostBias = fX;
            }
            else if (!wcscmp(t, L"TRACK_CONTROL_POINT"))
            {
                trackControl = {};
            }
            else if (swscanf_s(t, L"tcp_id=%u", &uX) == 1)
            {
                trackControl.id = uX;
            }
            else if (swscanf_s(t, L"tcp_pos=%f %f %f", &fX, &fY, &fZ) == 3)
            {
                trackControl.position = { fX, fY, fZ };
            }
            else if (swscanf_s(t, L"tcp_bank=%f", &fX) == 1)
            {
                trackControl.bank = fX;
            }
            else if (swscanf_s(t, L"tcp_width=%f", &fX) == 1)
            {
                trackControl.width = fX;
            }
            else if (swscanf_s(t, L"tcp_depth=%f", &fX) == 1)
            {
                trackControl.depth = fX;
            }
            else if (!wcscmp(t, L"END_TRACK_CONTROL_POINT"))
            {
                trackEdge.deqControls.push_back(trackControl);
            }
            else if (!wcscmp(t, L"END_TRACK_EDGE"))
            {
                if (CTrackGraph* pTrackGraph = dynamic_cast<CTrackGraph*>(pCom))
                    pTrackGraph->Get_Edges().push_back(trackEdge);
            }

			// HeightMap
			else if (swscanf_s(t, L"m_fEditStrength=%f", &fX) == 1)
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->Set_EditStrength(fX);
			}
			else if (swscanf_s(t, L"m_fEditRadius=%f", &fX) == 1)
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->Set_EditRadius(fX);
			}
			else if (swscanf_s(t, L"m_iCntX=%d", &iX) == 1)
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->Set_CntX(iX);
			}
			else if (swscanf_s(t, L"m_iCntZ=%d", &iX) == 1)
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->Set_CntZ(iX);
			}
			else if (swscanf_s(t, L"m_fItv=%f", &fX) == 1)
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->Set_Itv(fX);
			}
			else if (!wcscmp(t, L"HM_VERTEX"))
			{
				vtxTex = {};
			}
			else if (swscanf_s(t, L"hm_vtx_pos=%f %f %f", &fX, &fY, &fZ) == 3)
			{
				vtxTex.vPosition = { fX, fY, fZ };
			}
			else if (swscanf_s(t, L"hm_vtx_uv=%f %f", &fX, &fY) == 2)
			{
				vtxTex.vTexUV = { fX, fY };
			}
			else if (!wcscmp(t, L"END_HM_VERTEX"))
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->GetVertices().push_back(vtxTex);
			}
			else if (!wcscmp(t, L"HM_FACE"))
			{
				face = {};
			}
			else if (swscanf_s(t, L"hm_face_index=%u %u %u", &iX, &iY, &iZ) == 3)
			{
				face.indices = { _ulong(iX), _ulong(iY), _ulong(iZ) };
			}
			else if (swscanf_s(t, L"hm_face_normal=%f %f %f", &fX, &fY, &fZ) == 3)
			{
				face.vNoraml = { fX, fY, fZ };
			}
			else if (!wcscmp(t, L"END_HM_FACE"))
			{
				if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom))
					pHM->GetFaces().push_back(face);
			}

            else
            {
                st.Unget();
                break;
            }
        }
        if (CSpline* pSpline = dynamic_cast<CSpline*>(pCom)) {
            pSpline->Compute_Spline();
        }
        else if (CTrackGraph* pTrackGraph = dynamic_cast<CTrackGraph*>(pCom)) {
            pTrackGraph->Finalize_LoadedData();
        }
        else if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pCom)) {
			pHM->Ready_BufferByVec();
        }
    }

    // 특정 접두사로 시작하는지 확인
    inline bool StartsWith(const wchar_t* line, const wchar_t* prefix)
    {
        return wcsncmp(line, prefix, wcslen(prefix)) == 0;
    }

    
	inline void SaveGameObject(FILE* pf, CGameObject* pObj, int depth)
	{
		auto writeIndent = [](FILE* f, int depth)
			{
				for (int i = 0; i < depth; ++i)
					fwprintf(f, L"  ");
			};

		writeIndent(pf, depth);
		fwprintf(pf, L"OBJECT\n");

		writeIndent(pf, depth + 1);
		fwprintf(pf, L"type=%s\n", pObj->GetType());
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"prefabPath=%s\n", pObj->Get_PrefabPath());
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"belong=%d\n", pObj->Get_Belong() ? 1 : 0);
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"name=%s\n", pObj->GetName());
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"tag=%s\n", pObj->GetTag());
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"collisionLayer=%d\n", (int)pObj->Get_CollisionLayer());
		writeIndent(pf, depth + 1);
		fwprintf(pf, L"cullDistance=%d\n", pObj->Get_CullDistance());

		for (auto& pairCom : pObj->Get_ComponentMap())
		{
			CComponent* pCom = pairCom.second;
			if (pCom == nullptr)
				continue;

			const WCHAR* mapTag = pairCom.first ? pairCom.first : L"";
			const WCHAR* protoTag = pCom->Get_ProtoTag();
			if (protoTag == nullptr)
				protoTag = L"";

			auto writeCompHead = [&](const WCHAR* kind)
				{
					writeIndent(pf, depth + 1);
					fwprintf(pf, L"COMP kind=%s proto=%s mapTag=%s\n", kind, protoTag, mapTag);
				};

			switch (pCom->Get_Kind()) {
			case CK_TRANSFORM:
			{
				CTransform* pTF = static_cast<CTransform*>(pCom);

				_matrix matLocal = *pTF->Get_LocalWorld();
				float t[3], r[3], s[3];
				Engine::DecomposeMatrixToComponents(&matLocal, t, r, s);
				D3DXQUATERNION q = pTF->Get_Quaternion();
				writeCompHead(L"Transform");
				writeIndent(pf, depth + 2);
				fwprintf(pf, L"pos=%f %f %f\n", t[0], t[1], t[2]);
				writeIndent(pf, depth + 2);
				fwprintf(pf, L"rot=%f %f %f\n", r[0], r[1], r[2]);
				writeIndent(pf, depth + 2);
				fwprintf(pf, L"quat=%f %f %f %f\n", q.x, q.y, q.z, q.w);
				writeIndent(pf, depth + 2);
				fwprintf(pf, L"scale=%f %f %f\n", s[0], s[1], s[2]);
			}
			break;
			case CK_MESH:
			{
				CVIBuffer* pBuf = static_cast<CVIBuffer*>(pCom);

				for (auto& proto : CProtoMgr::GetInstance()->Get_Prototypes())
				{
					const ProtoRecord& rec = proto.second;
					if (rec.proto->Get_Kind() == CK_MESH && rec.proto &&
						typeid(*pBuf) == typeid(*rec.proto))
					{
						protoTag = rec.tag;
						break;
					}
				}
				writeCompHead(L"Mesh");
				if (CSpline* pSpline = dynamic_cast<CSpline*>(pBuf)) {
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"SampleUnit=%f\n", pSpline->Get_SampleUnit());
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"TextureUnit=%f\n", pSpline->Get_TextureUnit());
					auto& vecPoints = pSpline->Get_ControlPoints();
					for (auto& cp : vecPoints) {
						writeIndent(pf, depth + 2);
						fwprintf(pf, L"CONTROL_POINT\n");
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"cp_pos=%f %f %f\n", cp.position.x, cp.position.y, cp.position.z);
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"cp_bank=%f\n", cp.bank);
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"cp_width=%f\n", cp.width);
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"cp_depth=%f\n", cp.depth);
						writeIndent(pf, depth + 2);
						fwprintf(pf, L"END_CONTROL_POINT\n");
					}
				}
				else if (CHeightMap* pHM = dynamic_cast<CHeightMap*>(pBuf)) {
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"m_fEditStrength=%f\n", pHM->Get_EditStrength());
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"m_fEditRadius=%f\n", pHM->Get_EditRadius());
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"m_iCntX=%d\n", pHM->Get_CntX());
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"m_iCntZ=%d\n", pHM->Get_CntZ());
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"m_fItv=%f\n", pHM->Get_Itv());
					
					auto& vertices = pHM->GetVertices();
					for (auto& v : vertices) {
						writeIndent(pf, depth + 2);
						fwprintf(pf, L"HM_VERTEX\n");
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"hm_vtx_pos=%f %f %f\n", v.vPosition.x, v.vPosition.y, v.vPosition.z);
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"hm_vtx_uv=%f %f\n", v.vTexUV.x, v.vTexUV.y);
						writeIndent(pf, depth + 2);
						fwprintf(pf, L"END_HM_VERTEX\n");
					}

					auto& faces = pHM->GetFaces();
					for (auto& f : faces) {
						writeIndent(pf, depth + 2);
						fwprintf(pf, L"HM_FACE\n");
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"hm_face_index=%u %u %u\n", f.indices._0, f.indices._1, f.indices._2);
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"hm_face_normal=%f %f %f\n", f.vNoraml.x, f.vNoraml.y, f.vNoraml.z);
						writeIndent(pf, depth + 2);
						fwprintf(pf, L"END_HM_FACE\n");
					}
				}
			}
			break;
			case CK_TRACKGRAPH:
			{
				CTrackGraph* pTrackGraph = static_cast<CTrackGraph*>(pCom);
				writeCompHead(L"TrackGraph");

				writeIndent(pf, depth + 2);
				fwprintf(pf, L"tg_sample_unit=%f\n", pTrackGraph->Get_SampleUnit());

				for (const auto& node : pTrackGraph->Get_Nodes())
				{
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"TRACK_NODE\n");
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"tn_id=%u\n", node.id);
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"tn_pos=%f %f %f\n",
						node.position.x, node.position.y, node.position.z);
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"tn_start=%d\n", node.bStart ? 1 : 0);
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"tn_finish=%d\n", node.bFinish ? 1 : 0);
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"END_TRACK_NODE\n");
				}

				for (const auto& edge : pTrackGraph->Get_Edges())
				{
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"TRACK_EDGE\n");
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"te_id=%u\n", edge.id);
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"te_nodes=%u %u\n", edge.fromNode, edge.toNode);
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"te_default_size=%f %f\n",
						edge.fWidthDefault, edge.fHeightDefault);
					writeIndent(pf, depth + 3);
					fwprintf(pf, L"te_cost_bias=%f\n", edge.fCostBias);

					for (const auto& control : edge.deqControls)
					{
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"TRACK_CONTROL_POINT\n");
						writeIndent(pf, depth + 4);
						fwprintf(pf, L"tcp_id=%u\n", control.id);
						writeIndent(pf, depth + 4);
						fwprintf(pf, L"tcp_pos=%f %f %f\n",
							control.position.x, control.position.y, control.position.z);
						writeIndent(pf, depth + 4);
						fwprintf(pf, L"tcp_bank=%f\n", control.bank);
						writeIndent(pf, depth + 4);
						fwprintf(pf, L"tcp_width=%f\n", control.width);
						writeIndent(pf, depth + 4);
						fwprintf(pf, L"tcp_depth=%f\n", control.depth);
						writeIndent(pf, depth + 3);
						fwprintf(pf, L"END_TRACK_CONTROL_POINT\n");
					}

					writeIndent(pf, depth + 2);
					fwprintf(pf, L"END_TRACK_EDGE\n");
				}
			}
			break;
			case CK_TEXTURE:
			{
				CTexture* pTex = static_cast<CTexture*>(pCom);
				writeCompHead(L"Texture");
			}
			break;
			case CK_COLLIDER:
			{
				CCollider* pCol = static_cast<CCube_Collider*>(pCom);
				if (pCol->GetColliderType() == CUBE_COLLIDER) {
					CCube_Collider* pBox = static_cast<CCube_Collider*>(pCom);

					_vec3 offset = pBox->Get_Offset();
					_vec3 extents = ToVec3(pBox->Get_Info().Extents);
					writeCompHead(L"CubeCollider");
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"trigger=%d\n", pBox->GetIsTrigger() ? 1 : 0);
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"offset=%f %f %f\n", offset.x, offset.y, offset.z);
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"extents=%f %f %f\n", extents.x, extents.y, extents.z);
				}
				else if (pCol->GetColliderType() == SPHERE_COLLIDER) {
					CSphere_Collider* pSphere = static_cast<CSphere_Collider*>(pCom);

					_vec3 offset = pSphere->Get_Offset();
					writeCompHead(L"SphereCollider");
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"trigger=%d\n", pSphere->GetIsTrigger() ? 1 : 0);
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"offset=%f %f %f\n", offset.x, offset.y, offset.z);
					writeIndent(pf, depth + 2);
					fwprintf(pf, L"radius=%f\n", pSphere->Get_Info().Radius);
				}
			}
			break;
			}
		}

		for (CGameObject* pChild : pObj->Get_Children())
		{
			if (pChild)
				SaveGameObject(pf, pChild, depth + 1);
		}

		writeIndent(pf, depth);
		fwprintf(pf, L"ENDOBJECT\n");
	}

	inline CGameObject* LoadGameObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev,
		CGameObject* pParent)
	{
		CGameObject* pObj = CEmpty::Create(pGraphicDev);
		if (pObj == nullptr)
			return nullptr;

		const uint32_t guid = CManagement::GetInstance()->GenerateGuid();
		pObj->SetGuid(guid);
		if (pParent)
			pParent->Set_Child(pObj);

		wstring key = to_wstring(guid);
		CManagement::GetInstance()->Add_GameObject(L"Default", key.c_str(), pObj);

		wchar_t* t = nullptr;
		while (st.Next(t))
		{
			if (!wcscmp(t, L"ENDOBJECT"))
				break;

			if (!wcscmp(t, L"OBJECT"))
			{
				LoadGameObject(st, pGraphicDev, pObj);
				continue;
			}

			if (StartsWith(t, L"type="))
				pObj->SetType(t + 5);
			else if (StartsWith(t, L"prefabPath="))
				pObj->Set_PrefabPath(t + 11);
			else if (StartsWith(t, L"belong="))
				pObj->Set_Belong(_wtoi(t + 7) != 0);
			else if (StartsWith(t, L"name="))
				pObj->SetName(t + 5);
			else if (StartsWith(t, L"tag="))
				pObj->SetTag(t + 4);
			else if (StartsWith(t, L"collisionLayer="))
				pObj->Set_CollisionLayer((COLLISION_LAYER)_wtoi(t + 15));
			else if (StartsWith(t, L"cullDistance="))
				pObj->Set_CullDistance(_wtoi(t + 13));
			else if (StartsWith(t, L"COMP "))
			{
				wchar_t kind[64] = {};
				wchar_t proto[256] = {};
				wchar_t mapTag[256] = {};
				ExtractCompField(t, L"kind", kind, 64);
				ExtractCompField(t, L"proto", proto, 256);
				ExtractCompField(t, L"mapTag", mapTag, 256);

				CComponent* pCom = nullptr;
				if (!wcscmp(kind, L"Transform"))
				{
					pCom = pObj->Get_Transform();
				}
				else
				{
					const WCHAR* interned = InternProtoTag(proto);
					if (interned == nullptr)
						interned = InternProtoTag(mapTag);
					if (interned)
					{
						pCom = pObj->Add_Component(interned, interned);
					}
				}

				ApplyCompProperties(st, pObj, pCom);
			}
		}

		return pObj;
	}

	inline CGameObject* LoadGameObject(FileReadState& st, LPDIRECT3DDEVICE9 pGraphicDev,
		CGameObject* pParent, CScene* pScene)
	{
		CGameObject* pObj = CEmpty::Create(pGraphicDev);
		if (pObj == nullptr)
			return nullptr;

		const uint32_t guid = pScene->GenerateGuid();
		pObj->SetGuid(guid);
		if (pParent)
			pParent->Set_Child(pObj);

		wstring key = to_wstring(guid);
		pScene->Add_GameObject(L"Default", key.c_str(), pObj);

		wchar_t* t = nullptr;
		while (st.Next(t))
		{
			if (!wcscmp(t, L"ENDOBJECT"))
				break;

			if (!wcscmp(t, L"OBJECT"))
			{
				LoadGameObject(st, pGraphicDev, pObj, pScene);
				continue;
			}

			if (StartsWith(t, L"type="))
				pObj->SetType(t + 5);
			else if (StartsWith(t, L"prefabPath="))
				pObj->Set_PrefabPath(t + 11);
			else if (StartsWith(t, L"belong="))
				pObj->Set_Belong(_wtoi(t + 7) != 0);
			else if (StartsWith(t, L"name="))
				pObj->SetName(t + 5);
			else if (StartsWith(t, L"tag="))
				pObj->SetTag(t + 4);
			else if (StartsWith(t, L"collisionLayer="))
				pObj->Set_CollisionLayer((COLLISION_LAYER)_wtoi(t + 15));
			else if (StartsWith(t, L"cullDistance="))
				pObj->Set_CullDistance(_wtoi(t + 13));
			else if (StartsWith(t, L"COMP "))
			{
				wchar_t kind[64] = {};
				wchar_t proto[256] = {};
				wchar_t mapTag[256] = {};
				ExtractCompField(t, L"kind", kind, 64);
				ExtractCompField(t, L"proto", proto, 256);
				ExtractCompField(t, L"mapTag", mapTag, 256);

				CComponent* pCom = nullptr;
				if (!wcscmp(kind, L"Transform"))
				{
					pCom = pObj->Get_Transform();
				}
				else
				{
					const WCHAR* interned = InternProtoTag(proto);
					if (interned == nullptr)
						interned = InternProtoTag(mapTag);
					if (interned)
					{
						pCom = pObj->Add_Component(interned, interned);
					}
				}

				ApplyCompProperties(st, pObj, pCom);
			}
		}

		return pObj;
	}
}

#endif
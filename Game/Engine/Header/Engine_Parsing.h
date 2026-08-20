#ifndef Engine_Parsing_h__
#define Engine_Parsing_h__

#include "Engine_Typedef.h"
#include "CProtoMgr.h"
#include "CGameObject.h"
#include "CComponent.h"

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
        wchar_t* t = nullptr;
        while (st.Next(t))
        {
            float x = 0.f, y = 0.f, z = 0.f, w = 0.f;
            int   trig = 0;

            if (swscanf_s(t, L"pos=%f %f %f", &x, &y, &z) == 3)
            {
                if (CTransform* pTF = pObj->Get_Transform())
                    pTF->Set_Pos(_vec3(x, y, z));
            }
            else if (swscanf_s(t, L"rot=%f %f %f", &x, &y, &z) == 3)
            {
                // 런타임 회전은 quat가 기준. rot는 저장용 미리보기.
            }
            else if (swscanf_s(t, L"quat=%f %f %f %f", &x, &y, &z, &w) == 4)
            {
                D3DXQUATERNION q(x, y, z, w);
                if (CTransform* pTF = pObj->Get_Transform())
                    pTF->Set_Quaternion(&q);
            }
            else if (swscanf_s(t, L"scale=%f %f %f", &x, &y, &z) == 3)
            {
                if (CTransform* pTF = pObj->Get_Transform())
                    pTF->Set_Scale(_vec3(x, y, z));
            }
            else if (swscanf_s(t, L"trigger=%d", &trig) == 1)
            {
                if (CCollider* pCol = dynamic_cast<CCollider*>(pCom))
                    pCol->SetIsTrigger(trig != 0);
            }
            else if (swscanf_s(t, L"offset=%f %f %f", &x, &y, &z) == 3)
            {
                if (CCube_Collider* pBox = dynamic_cast<CCube_Collider*>(pCom))
                    pBox->Set_Offset(_vec3(x, y, z));
                else if (CSphere_Collider* pSphere = dynamic_cast<CSphere_Collider*>(pCom))
                    pSphere->Set_Offset(_vec3(x, y, z));
            }
            else if (swscanf_s(t, L"extents=%f %f %f", &x, &y, &z) == 3)
            {
                if (CCube_Collider* pBox = dynamic_cast<CCube_Collider*>(pCom))
                    pBox->Set_Extents(_vec3(x, y, z));
            }
            else if (swscanf_s(t, L"radius=%f", &x) == 1)
            {
                if (CSphere_Collider* pSphere = dynamic_cast<CSphere_Collider*>(pCom))
                    pSphere->Set_Radius(x);
            }
            else
            {
                st.Unget();
                break;
            }
        }
    }

    // 특정 접두사로 시작하는지 확인
    inline bool StartsWith(const wchar_t* line, const wchar_t* prefix)
    {
        return wcsncmp(line, prefix, wcslen(prefix)) == 0;
    }

}

#endif
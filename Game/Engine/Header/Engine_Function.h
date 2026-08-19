#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	// 템플릿은 기능의 정해져있으나 자료형은 정해져있지 않은 것
	// 기능을 인스턴스화 하기 위하여 만들어두는 틀

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = NULL;
		}

		return dwRefCnt;
	}


	// Functor
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag){}
		~CTag_Finder(void) {}

	public:
		template<typename T> 
		_bool		operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
				return true;
			
			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// 연관컨테이너 삭제용
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};

	template <typename T>
	inline T clampT(T _value, T _min, T _max) {
		return max(_min, min(_max, _value));
	}

	inline DirectX::XMVECTOR ToXMVec(_vec3 vec){
		return DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&vec));
	}

	inline DirectX::XMFLOAT3 ToXMFLOAT3(const _vec3& v)
	{
		return DirectX::XMFLOAT3(v.x, v.y, v.z);
	}

	inline _vec3 ToVec3(const DirectX::XMFLOAT3& _XMFLOAT3)
	{
		return _vec3(_XMFLOAT3.x, _XMFLOAT3.y, _XMFLOAT3.z);
	}

	inline DirectX::XMFLOAT4 ToXMFLOAT4(const D3DXQUATERNION& q)
	{
		return DirectX::XMFLOAT4(q.x, q.y, q.z, q.w);
	}

	inline std::string ToUtf8(const _tchar* wstr) 
	{
		if (!wstr) return "";
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		std::string str(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], size_needed, NULL, NULL);
		str.pop_back(); // Remove trailing null terminator copy
		return str;
	}

	inline std::wstring FromUtf8(const char* utf8)
	{
		if (!utf8) return L"";
		int n = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
		std::wstring w(n, 0);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &w[0], n);
		if (!w.empty() && w.back() == L'\0') w.pop_back();
		return w;
	}

	inline vector<int> MakeFailure(const string& pattern) {
		int m = pattern.size();
		vector<int> fail(m, 0);

		int j = 0;

		for (int i = 1; i < m; ++i) {
			while (j > 0 && pattern[i] != pattern[j]) {
				j = fail[j - 1];
			}

			if (pattern[i] == pattern[j]) {
				fail[i] = ++j;
			}
		}

		return fail;
	}

	inline bool KMPContain(const string& text, const string& pattern)
	{
		if (pattern.empty())
			return false;

		vector<int> fail = MakeFailure(pattern);

		int j = 0;

		for (int i = 0; i < text.size(); ++i) {
			while (j > 0 && text[i] != pattern[j]) {
				j = fail[j - 1];
			}

			if (text[i] == pattern[j]) {
				++j;

				if (j == pattern.size())
					return true;
			}
		}

		return false;
	}

	inline bool CheckCollisionUI(HWND hWnd, _vec3 vCenter, _vec3 vScale)
	{
		POINT		pt{};

		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		pt.x -= WINCX * 0.5f;
		pt.y = -pt.y + WINCY * 0.5f;
		//cout << pt.y << endl;
		if (pt.x < vCenter.x - vScale.x / 2 || pt.x > vCenter.x + vScale.x / 2)
			return false;
		if (pt.y < vCenter.y - vScale.y / 2 || pt.y > vCenter.y + vScale.y / 2)
			return false;

		return true;
	}
}

#endif // Engine_Function_h__

#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"
#include <cmath>

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

	inline _quaternion ToQuaternion(const DirectX::XMFLOAT4& _XMFLOAT4)
	{
		return _quaternion(_XMFLOAT4.x, _XMFLOAT4.y, _XMFLOAT4.z, _XMFLOAT4.w);
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

	inline bool CheckCollisionUI(HWND hWnd, _vec3 vCenter, _vec3 vScale, LPDIRECT3DDEVICE9 pGraphicDev)
	{
		POINT		pt{};

		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		D3DVIEWPORT9 vp;
		pGraphicDev->GetViewport(&vp);

		pt.x -= vp.Width * 0.5f;
		pt.y = -pt.y + vp.Height * 0.5f;

		if (pt.x < vCenter.x - vScale.x / 2 || pt.x > vCenter.x + vScale.x / 2)
			return false;
		if (pt.y < vCenter.y - vScale.y / 2 || pt.y > vCenter.y + vScale.y / 2)
			return false;

		return true;
  }
  
	inline const char* GetLayerName(COLLISION_LAYER cl) {
		switch (cl) {
		case CL_DEFAULT: return "Default";
		case IGNORE_RAYCAST: return "Ignore Raycast";
		case CL_ITEM_BOX: return "ITEM_BOX"; case CL_CART_WHEEL: return "CART_WHEEL";
		case CL_CART_BODY: return "CART_BODY"; case CL_ITEM: return "ITEM";
		case CL_LAYER5: return "Layer5"; case CL_LAYER6: return "Layer6";
		case CL_LAYER7: return "Layer7"; case CL_LAYER8: return "Layer8";
		case CL_LAYER9: return "Layer9"; case CL_LAYER10: return "Layer10";
		case CL_LAYER11: return "Layer11"; case CL_LAYER12: return "Layer12";
		case CL_LAYER13: return "Layer13"; case CL_LAYER14: return "Layer14";
		case CL_LAYER15: return "Layer15"; case CL_LAYER16: return "Layer16";
		case CL_LAYER17: return "Layer17"; case CL_LAYER18: return "Layer18";
		case CL_LAYER19: return "Layer19"; case CL_LAYER20: return "Layer20";
		case CL_LAYER21: return "Layer21"; case CL_LAYER22: return "Layer22";
		case CL_LAYER23: return "Layer23"; case CL_LAYER24: return "Layer24";
		case CL_LAYER25: return "Layer25"; case CL_LAYER26: return "Layer26";
		case CL_LAYER27: return "Layer27"; case CL_LAYER28: return "Layer28";
		case CL_LAYER29: return "Layer29"; case CL_LAYER30: return "Layer30";
		}
	}

	inline DWORD FtoDw(float f) {
		return *(DWORD*)&f;
	}

	inline float GetRandomFloat(float LowBound, float HighBound)
	{
		if (LowBound >= HighBound)
			return LowBound;
		float f = (rand() % 10000) * 0.0001f;
		return (f * (HighBound - LowBound)) + LowBound;
	}

	inline void GetRandomVector(_vec3* vOut, _vec3* vMin, _vec3* vMax)
	{
		vOut->x = GetRandomFloat(vMin->x, vMax->x);
		vOut->y = GetRandomFloat(vMin->y, vMax->y);
		vOut->z = GetRandomFloat(vMin->z, vMax->z);
	}

	inline float Lerp(float t, float start, float end) {
		float s = 1.f - t;
		return start * s + end * t;
	}

	//P(t) = s^2(1+2t)A +t^2(1+2s)D + s^2t*vA - st^2*vD
	inline _vec3 Cubic_Hermite_Curve(float t, _vec3 A, _vec3 D, _vec3 vA, _vec3 vD) {
		float s = 1.f - t;
		return s * s * (1 + 2 * t) * A
			+ t * t * (1 + 2 * s) * D
			+ s * s * t * vA
			- s * t * t * vD;
	}

	// h00(t) = s^2(1+2t) = (t^2-2t+1)(1+2t)
	// 2t³ - 3t² + 1, h00'(t) = 6t^2 - 6t = -6st
	// 
	// h10(t) = t^2(1+2s) = t^2(3-2t)
	// -2t^3+3t^2,  h10'(t) = -6t^2+6t = 6t(1-t) = 6st 
	// 
	// h01(t) = s^2t = (1-t)^2t = t^3-2t^2+t
	// h01'(t) = 3t^2-4t+1 = (3t-1)(t-1) = s(1-3t)
	// 
	// h11(t) = st^2 = (1-t)t^2 = -t^3 + t^2
	// h11'(t) = -3t^2+2t = t(-3t + 2)
	// P'(t) = h00'(t)·A + h10'(t)·D + h01'(t)·vA - h11'(t)·vD
	// 나온 값을 정규화 할 필요가 있음
	inline _vec3 Cubic_Hermite_Curve_Derivative(float t, _vec3 A, _vec3 D, _vec3 vA, _vec3 vD)
	{
		float s = 1.f - t;
		return (-6 * s * t) * A
			+ (6 * s * t) * D
			+ (s * (1.f - 3 * t)) * vA
			+ (t * (3.f * t - 2.f)) * vD;
	}

	inline void DecomposeMatrixToComponents(_matrix* mat, float* translation, float* rotation, float* scale)
	{
		_vec3 vRight, vUp, vLook;
		memcpy(&vRight, &mat->m[0], sizeof(_vec3));
		memcpy(&vUp, &mat->m[1], sizeof(_vec3));
		memcpy(&vLook, &mat->m[2], sizeof(_vec3));
		scale[0] = D3DXVec3Length(&vRight);
		scale[1] = D3DXVec3Length(&vUp);
		scale[2] = D3DXVec3Length(&vLook);

		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);

		memcpy(&mat->m[0], &vRight, sizeof(_vec3));
		memcpy(&mat->m[1], &vUp, sizeof(_vec3));
		memcpy(&mat->m[2], &vLook, sizeof(_vec3));

		rotation[0] = D3DXToDegree(atan2f(mat->m[1][2], mat->m[2][2]));
		rotation[1] = D3DXToDegree(atan2f(-mat->m[0][2], sqrtf(mat->m[1][2] * mat->m[1][2] + mat->m[2][2] * mat->m[2][2])));
		rotation[2] = D3DXToDegree(atan2f(mat->m[0][1], mat->m[0][0]));

		memcpy(translation, &mat->m[3], sizeof(_vec3));
	}
}

#endif // Engine_Function_h__

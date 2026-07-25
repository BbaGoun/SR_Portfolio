#include "CHeightMapLoader.h"

HRESULT CHeightMapLoader::LoadHeightMapByByte(const _tchar* path, vector<BYTE>& vecByte, size_t sizeX, size_t sizeZ)
{
	HANDLE hFile = CreateFile(path,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		WCHAR wMessage[256];
		swprintf_s(wMessage, L"Load Image Fail : %s", path);
		MSG_BOXF(wMessage);
		return E_FAIL;
	}

	DWORD dwByte(0);

	// 1) 파일 헤더 읽기
	BITMAPFILEHEADER fh{};
	ReadFile(hFile, &fh, sizeof(fh), &dwByte, nullptr);
	
	// 2) BMP 시그니처 검증 ('B','M')
	if (fh.bfType != 0x4D42)
	{
		CloseHandle(hFile);
		return E_FAIL;
	}

	// 3) 정보 헤더 읽기 (bitCount 검증 등에 사용 가능)
	BITMAPINFOHEADER ih{};
	ReadFile(hFile, &ih, sizeof(ih), &dwByte, nullptr);

	// 4) 픽셀 데이터 시작 위치로 이동 (★ 헤더 스킵의 핵심)
	SetFilePointer(hFile, fh.bfOffBits, nullptr, FILE_BEGIN);

	BYTE byteData;
	int size = sizeX * sizeZ * 4;
	vecByte.reserve(size);
	for (int i = 0; i < size; ++i) {
		ReadFile(hFile, &byteData, sizeof(BYTE), &dwByte, nullptr);
		
		if (dwByte == 0)
			break;
		
		vecByte.push_back(byteData);
	}

	CloseHandle(hFile);

	return S_OK;
}

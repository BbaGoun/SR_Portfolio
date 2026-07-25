#pragma once
#include "Engine_Define.h"

class CHeightMapLoader
{
public:
	static HRESULT LoadHeightMapByByte(const _tchar* path, vector<BYTE>& vecByte, size_t sizeX, size_t sizeZ);
};


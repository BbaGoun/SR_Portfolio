#include "pch.h"
#include "SoundMgr.h"
#include <io.h>

SoundMgr::SoundMgr()
{
	m_pSystem = nullptr;
	ZeroMemory(m_pChannelArr, sizeof(m_pChannelArr));
}

SoundMgr::~SoundMgr()
{
	Release();
}

void SoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();
}

void SoundMgr::Release()
{
	StopAll();

	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		if (Mypair.second)
			FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	if (m_pSystem)
	{
		FMOD_System_Close(m_pSystem);
		FMOD_System_Release(m_pSystem);
		m_pSystem = nullptr;
	}

	ZeroMemory(m_pChannelArr, sizeof(m_pChannelArr));
}

void SoundMgr::PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& Pair)->bool
		{
			return !lstrcmp(pSoundKey, Pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	if (m_pChannelArr[eID])
		FMOD_Channel_Stop(m_pChannelArr[eID]);

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_OFF);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

void SoundMgr::PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	auto iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& Pair)->bool
		{
			return !lstrcmp(pSoundKey, Pair.first);
		});

	if (iter == m_mapSound.end())
		return;

	if (m_pChannelArr[SOUND_BGM])
		FMOD_Channel_Stop(m_pChannelArr[SOUND_BGM]);

	FMOD_System_PlaySound(m_pSystem, iter->second, nullptr, FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void SoundMgr::Update()
{
	if (!m_pChannelArr[SOUND_BGM])
		return;

	FMOD_System_Update(m_pSystem);
}

void SoundMgr::StopSound(CHANNELID eID)
{
	if (m_pChannelArr[eID])
		FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void SoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
	{
		if (m_pChannelArr[i])
			FMOD_Channel_Stop(m_pChannelArr[i]);
	}
}

void SoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (!m_pChannelArr[eID])
		return;

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}

void SoundMgr::LoadSoundFile()
{
	LoadSoundFileRecursive(L"../Bin/Resource/Sound/", L"");
	FMOD_System_Update(m_pSystem);
}

void SoundMgr::LoadSoundFileRecursive(const WCHAR* szFolderPath, const WCHAR* szRelativePrefix)
{
	WCHAR szSearchPath[MAX_PATH] = {};
	swprintf_s(szSearchPath, L"%s*.*", szFolderPath);

	_wfinddata64_t fd;
	intptr_t handle = _wfindfirst64(szSearchPath, &fd);

	if (handle == -1)
		return;

	do
	{
		if (!wcscmp(fd.name, L".") || !wcscmp(fd.name, L".."))
			continue;

		if (fd.attrib & _A_SUBDIR)
		{
			WCHAR szSubFolder[MAX_PATH] = {};
			swprintf_s(szSubFolder, L"%s%s/", szFolderPath, fd.name);

			WCHAR szSubPrefix[MAX_PATH] = {};
			swprintf_s(szSubPrefix, L"%s%s/", szRelativePrefix, fd.name);

			LoadSoundFileRecursive(szSubFolder, szSubPrefix);
			continue;
		}

		WCHAR wszFullPath[MAX_PATH] = {};
		swprintf_s(wszFullPath, L"%s%s", szFolderPath, fd.name);

		char szFullPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, wszFullPath, -1, szFullPath, MAX_PATH, nullptr, nullptr);

		FMOD_SOUND* pSound = nullptr;
		FMOD_MODE mode = FMOD_DEFAULT | FMOD_2D;

		const WCHAR* pExt = wcsrchr(fd.name, L'.');
		if (pExt && (!lstrcmpi(pExt, L".ogg") || !lstrcmpi(pExt, L".mp3")))
			mode |= FMOD_CREATESTREAM;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, mode, nullptr, &pSound);

		if (eRes == FMOD_OK)
		{
			WCHAR szSoundKey[MAX_PATH] = {};
			swprintf_s(szSoundKey, L"%s%s", szRelativePrefix, fd.name);

			size_t iLength = wcslen(szSoundKey) + 1;
			TCHAR* pSoundKey = new TCHAR[iLength];
			wcscpy_s(pSoundKey, iLength, szSoundKey);

			m_mapSound.insert({ pSoundKey, pSound });
		}
	} while (_wfindnext64(handle, &fd) == 0);

	_findclose(handle);
}

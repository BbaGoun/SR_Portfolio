#pragma once

enum CHANNELID { SOUND_BGM, 
	SOUND_EFFECT1, SOUND_EFFECT2, SOUND_EFFECT3, SOUND_EFFECT4,
	SOUND_EFFECT5, SOUND_EFFECT6, SOUND_EFFECT7, SOUND_EFFECT8,
	SOUND_SURPRISE,
	COLLISION_EFFECT_SUB, COLLISION_EFFECT, GLOBAL_EFFECT, GLOBAL_EFFECT_SUB, 
	SOUND_DRIFT, SOUND_BOOST, SOUND_BANANA, 
	SOUND_THUNDERCLOUD, SOUND_THUNDER, SOUND_THUNDERPLAYER,
	SOUND_ITEMGAIN, SOUND_CLOUD,
	SOUND_STARTCOUNT1, SOUND_STARTCOUNT2, SOUND_STARTCOUNT3,
	SOUND_ENDCOUND,SOUND_MOTOR,
	MAXCHANNEL };

class SoundMgr
{
private:
	SoundMgr();
	~SoundMgr();
	SoundMgr(const SoundMgr& rhs) = delete;
	SoundMgr& operator=(const SoundMgr& rhs) = delete;

public:
	static SoundMgr& GetInstance() {
		static SoundMgr Instance;
		return Instance;
	}

public:
	void Initialize();
	void Release();

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	void Update();

private:
	void LoadSoundFile();
	void LoadSoundFileRecursive(const WCHAR* szFolderPath, const WCHAR* szRelativePrefix);

private:
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	FMOD_SYSTEM* m_pSystem;
};


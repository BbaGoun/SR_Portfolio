#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	class CGameObject;

	typedef struct tagVertexColor
	{
		_vec3		vPosition;			
		_ulong		dwColor;
	
	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec2		vTexUV;

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexMesh
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;
		float		aBoneWeights[4];
		BYTE		aBoneIndices[4];
	}VTXMESH;

	typedef struct tagVertexTextureColor
	{
		_vec3		vPosition;
		_ulong		dwColor;
		_vec2		vTexUV;
		_vec3		vNormal;
	}VTXTC;
	const _ulong	FVF_TC = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;


	const D3DVERTEXELEMENT9 MeshVertexElements[] =
	{
		// Stream, Offset, Type, Method, Usage, UsageIndex
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 48, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)

	typedef struct tagVertexMissileColor
	{
		_vec3		vPosition;
		_ulong		dwColor;

	}VTXMISSILE;

	const _ulong	FVF_MISSILE = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	typedef struct tagFullScreen
	{
		_vec4		vPosition;
		_ulong		dwColor;
		_vec2		vTexUV;

	}VTXSCREEN;

	const _ulong	FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1; 


	// raycast 선 그리기 용 정점
	typedef struct tagVertexLine
	{
		_vec3 position;
		D3DCOLOR color;
	}VTXLINE;
	const _ulong	FVF_LINE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	// 16비트 인덱스 3개로 삼각형을 지정
	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	// 32비트 인덱스 3개로 삼각형을 지정
	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;	

	// 인덱스 3개와 면의 법선
	typedef struct tagFace32
	{
		INDEX32 indices;
		_vec3	vNoraml = { 0, 0 ,0 };
	}FACE32;


	typedef struct _tagCameraInfo
	{
		_matrix matView;
		_matrix matProj;
	}CameraInfo;

	typedef struct _tagControlPoint
	{
		_vec3		position;
		_vec3		V;		// 3차 허밋 곡선에서의 벡터
		_vec3		T;		// 진행 방향
		_vec3		R;		// 우측 방향
		_vec3		U;		// 윗 방향
		uint32_t	id;		// 식별 번호
		_float		bank;	// 진행 방향을 축으로 회전하는 정도
		_float		width;	// 지형의 너비
		_float		depth;	// 지형의 깊이 (중심이 제일 바닥)
	}ControlPoint;

	typedef struct Particle
	{
		_vec3		vPosition;
		_ulong		dwColor;
	}PARTICLE; 
	
	const _ulong	FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct Attribute
	{
		Attribute()
		{
			fLifeTime = 0.0f;
			fAge = 0.0f;
			bIsAlive = true;
		}

		_vec3		vPosition;
		_vec3		vVelocity;
		_vec3		vAcceleration;
		float       fLifeTime;     // how long the particle lives for before dying  
		float       fAge;          // current age of the particle  
		D3DXCOLOR   color;        // current color of the particle   
		D3DXCOLOR   colorFade;    // how the color fades with respect to time
		bool        bIsAlive;
	}ATTRIBUTE;

	struct RGB {
		int r, g, b;
	};

	typedef struct RenderTargetInfo
	{
		LPDIRECT3DTEXTURE9      pRTTexture = nullptr;
		LPDIRECT3DSURFACE9      pRTSurface = nullptr;
		LPDIRECT3DSURFACE9      pRTDepthStencil = nullptr;

		float					fWidth;
		float					fHeight;

		list<CGameObject*>		RenderList;
	}RTINFO;

	typedef struct _tagTrackNode
	{
		_vec3			position;
		NodeId			id;
		float			s_Global; // s는 전역 진행량(호 길이)라는 뜻
		bool			bStart;		// 시작점
		bool			bFinish;	// 끝점
		int				in_Degree;   // 위상 정렬용 진입 차수
		vector<EdgeId>	vecInEdgeIds, vecOutEdgeIds;
	}TrackNode;

	typedef struct _tagTrackSample
	{
		_vec3			position;
		_vec3			T, R, U;
		float			u; // 이 엣지 시작부터의 호장
		float			s; // 전역 진행량
		float			halfW, halfH, halfL; // OBB 반 크기
		float			speed; // 이 구간의 감속량
	}TrackSample;

	typedef struct _tagTrackEdge
	{
		EdgeId			id;
		NodeId			fromNode, toNode;
		float			sStart, sEnd;
		float			fLength; // 엣지의 실제 길이, sEnd-sStart와 다를 수 있음
		float			fWidthDefault, fHeightDefault;
		float			fCostBias;	// 경로 선택의 가중치
		deque<ControlPoint>		deqControls;
		vector<TrackSample>		vecSamples;
	}TrackEdge;


	typedef struct _tagTrackLocator
	{
		_vec3			localPos;
		EdgeId			edgeId;			// 현재 경로(갈래)
		int				iSampleIndex;	// 검색 시작점
		float			u;				// 엣지 로컬 호장
		float			s;				// 전역 진행량(0~lapLength)
		int				iLap = 0;		// 진행한 랩 수
		bool			bValid = false;	// 터널 밖, 리셋 중
	}TrackLocator;

	typedef struct _tagHazardRecord
	{
		EdgeId			edgeId;			// 어느 도로인가
		float			u;				// 그 도로의 어디인가
		float			lateral;		// R 방향 오프셋
		float			radius;			// 회피 반경
		ITEM_TYPE		eType;			// 아이템 타입
		CGameObject*	pOwner;			// 설치된 오브젝트 주소
	}HazardRecord;

	// Advance, 스폰 용
	typedef struct _tagTrackPose {
		_vec3	position, T, R, U;
		EdgeId	edgeId;
		float	u, s;
		float	speed;
		float	halfW;
		bool	bValid = false;
		bool	bDodge = false;
	}TrackPose;

	struct ArcLengthEntry
	{
		float globalT;  // segmentIndex + localT
		float length;   // 곡선 시작부터의 누적 거리
	};

	struct CheckInfo {
		TrackLocator bestLocater;
		float bestScore = FLT_MAX;
		bool bFound = false;
		float bestLateral = 0;
	};
}


#endif // Engine_Struct_h__

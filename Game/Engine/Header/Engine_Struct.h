#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
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
		_vec3	vNoraml;
	}FACE32;



#pragma pack(push, 1)
	// PSK 파일의 각 청크 헤더 (ACTRHEAD)
	struct VChunkHeader {
		char		ChunkID[20];    // 청크 식별자
		int32_t		TypeFlag;       // 타입 플래그
		uint32_t	DataSize;       // 각 데이터 항목의 크기
		uint32_t	DataCount;      // 데이터 항목 수
	};

	// 버텍스 위치 (PNTS0000)
	// _vec3 사용

	// 웨지 - 버텍스 + UV (VTXW0000)
	// The format of this structure depends on the edge count.
	// 65536d보다 작거나 같으면
	struct PSKWedgeL {
		uint16_t	PointIndex;
		uint16_t	Padding1;
		float		U, V;
		uint8_t		MatIndex;
		uint8_t		Reserved;
		uint16_t	Padding2;
	};
	
	// 65536d보다 크면
	struct PSKWedgeG {
		uint32_t	PointIndex;  // VPoint 배열의 인덱스
		float		U, V;        // UV 좌표
		uint32_t	MatIndex;
	};

	// 삼각형 면 (FACE0000) - Unreal Engine format
	struct PSKTriangle00 {
		uint16_t	WedgeIndex[3];   // VVertex 배열의 인덱스 3개
		uint8_t		MatIndex;
		uint8_t		AuxMatIndex;
		int32_t		SmoothingGroups;
	};

	// 삼각형 면 (FACE0032) - Umodel PSK extension
	struct PSKTriangle32 {
		uint32_t	WedgeIndex[3];   // VVertex 배열의 인덱스 3개
		uint8_t		MatIndex;
		uint8_t		AuxMatIndex;
		int32_t		SmoothingGroups;
	};

	// Material (MATT0000)
	struct PSKMaterial {
		char		Name[64];
		uint32_t	TexIndex;
		int32_t		PolyFlags;
		int32_t		AuxMat;
		int32_t		AuxFlags;
		int32_t		LODBias;
		int32_t		LODStyle;
	};

	// 본 구조 (REFSKELT)
	struct PSKBone {
		char		Name[64];
		int32_t		Flags;
		int32_t		NumChildren;
		int32_t		ParentIndex;		// -1 또는 0이면 루트
		float		Orientation[4];		// Quaternion (x,y,z,w)
		float		Position[3];		// 본 위치
		float		Length;
		float		XSize, YSize, ZSize;
	};

	// 스키닝 웨이트 (RAWWEIGHTS)
	struct PSKRawBoneWeight {
		float    Weight;		// 가중치 (0.0 ~ 1.0)
		uint32_t  PointIndex;	// 버텍스 인덱스
		uint32_t  BoneIndex;	// 본 인덱스
	};

	struct PSKSubset
	{
		DWORD					dwStartIndex;	// 인덱스 버퍼 내 시작 위치
		DWORD					dwPrimCount;	// 이 서브셋의 삼각형 수
		uint8_t					MatIndex;		// 이 서브셋에 사용할 텍스처
	};
#pragma pack(pop)

	typedef struct _tagCameraInfo
	{
		_matrix matView;
		_matrix matProj;
	}CameraInfo;
}


#endif // Engine_Struct_h__

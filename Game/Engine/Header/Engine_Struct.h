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


	typedef struct _tagCameraInfo
	{
		_matrix matView;
		_matrix matProj;
	}CameraInfo;

	typedef struct _tagControlPoint
	{
		_vec3	position;
		_float	bank;
		_float	width;
		_float	depth;
	}ControlPoint;
}


#endif // Engine_Struct_h__

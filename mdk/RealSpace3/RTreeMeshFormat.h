#pragma once

#include "RTypes.h"

namespace rs3
{

////////////////////////////////////////////////////////////////////////////
//
// Tree Vertexforamt for GPU
//
struct SFVFBranchVertex_GPU_Light
{
	RVector			m_vPosition;            // Always Used                          
	RVector			m_vNormal;              // Dynamic Lighting Only    
	FLOAT           m_fTexCoords[2];        // Always Used
	FLOAT           m_fWind[2];
	FLOAT           m_fShadowCoords[2];     // Texture coordinates for the shadows
	FLOAT			m_fDetailCoords[2];
};


struct SFVFFrondVertex_GPU_Light
{
	RVector			m_vPosition;            // Always Used                          
	RVector			m_vNormal;              // Dynamic Lighting Only    
	FLOAT           m_fTexCoords[2];        // Always Used
	FLOAT           m_fWind[2];
	FLOAT           m_fShadowCoords[4];     // Texture coordinates for the shadows	// G-stage������ Frond�� branch�� ���� ���̴��� ��� �Ѵ�.�׷��� byteũ�⸦ ���߾� �ֵ��� ����.
};

struct SFVFLeafVertex_GPU_Light
{
	RVector4		m_vPosition;            // Always Used                          
	RVector			m_vNormal;              // Dynamic Lighting Only    
	FLOAT           m_fTexCoords[4];        // Always Used
	FLOAT           m_fTexCoords2[4];		// Always Used
	FLOAT           m_fTexCoords3[4];		// Always Used
};


struct SFVFLeafMeshVertex_GPU_Light
{
	RVector4		m_vPosition;            // Always Used                          
	RVector			m_vNormal;              // Dynamic Lighting Only    
	FLOAT           m_fTexCoords[4];        // Always Used
	FLOAT           m_fTexCoords2[3];        // Always Used
	FLOAT           m_fTexCoords3[3];        // Always Used
	FLOAT           m_fTexCoords4[4];        // Always Used
};


}
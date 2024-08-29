#include "pch.h"
#include "Effects.h"
#include "Texture.h"
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

namespace dae {
	Effect::Effect(ID3D11Device* pDevice, const std::wstring& assetFile)
		: BaseEffect(pDevice, assetFile)
	{
		m_pDiffuseTexture = Texture::LoadFromFile(pDevice, "Resources/vehicle_diffuse.png");
		m_pGlossTexture = Texture::LoadFromFile(pDevice, "Resources/vehicle_gloss.png");
		m_pNormalTexture = Texture::LoadFromFile(pDevice, "Resources/vehicle_normal.png");
		m_pSpecularTexture = Texture::LoadFromFile(pDevice, "Resources/vehicle_specular.png");


		m_pCameraOrigin = m_pEffect->GetVariableByName("gCameraPosition")->AsVector();
		if (!m_pCameraOrigin->IsValid())
		{
			std::wcout << L"gCameraPosition not valid!\n";
		}

		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
		{
			std::wcout << L"m_pDiffuseMapVariable not valid!\n";
		}

		m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
		if (!m_pNormalMapVariable->IsValid())
		{
			std::wcout << L"m_pNormalMapVariable not valid!\n";
		}

		m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
		if (!m_pSpecularMapVariable->IsValid())
		{
			std::wcout << L"m_pSpecularMapVariable not valid!\n";
		}

		m_pGlossMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
		if (!m_pGlossMapVariable->IsValid())
		{
			std::wcout << L"m_pGlossMapVariable not valid!\n";
		}

		m_NormalMappingOnVariable = m_pEffect->GetVariableByName("gNormalMappingOn")->AsScalar();
		if (!m_NormalMappingOnVariable)
		{
			std::wcout << L"m_NormalMappingOnVariable not valid!\n";
		}

		SetDiffuseMap(m_pDiffuseTexture);
		SetGlossMap(m_pGlossTexture);
		SetNormalMap(m_pNormalTexture);
		SetSpecularMap(m_pSpecularTexture);
	}

	Effect::~Effect()
	{
		delete m_pDiffuseTexture;
		m_pDiffuseTexture = nullptr;
		delete m_pNormalTexture;
		m_pNormalTexture = nullptr;
		delete m_pGlossTexture;
		m_pGlossTexture = nullptr;
		delete m_pSpecularTexture;
		m_pSpecularTexture = nullptr;
	}

	void Effect::SetDiffuseMap(Texture* pDiffuseTexture)
	{
		if (m_pDiffuseMapVariable)
			m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetSRV());
	}

	void Effect::SetNormalMap(Texture* pNormalTexture)
	{
		if (m_pNormalMapVariable)
			m_pNormalMapVariable->SetResource(pNormalTexture->GetSRV());
	}

	void Effect::SetSpecularMap(Texture* pSpecularTexture)
	{
		if (m_pSpecularMapVariable)
			m_pSpecularMapVariable->SetResource(pSpecularTexture->GetSRV());
	}

	void Effect::SetGlossMap(Texture* pGlossTexture)
	{
		if (m_pGlossMapVariable)
			m_pGlossMapVariable->SetResource(pGlossTexture->GetSRV());
	}


	void Effect::SetCameraOrigin(Vector3 origin)
	{
		m_pCameraOrigin->SetFloatVector(reinterpret_cast<const float*>(&origin));
	}

	void Effect::NormalMappingToggle()
	{
		m_NormalMappingOn = !m_NormalMappingOn;
		m_NormalMappingOnVariable->AsScalar()->SetBool(m_NormalMappingOn);
		if (m_NormalMappingOn) std::cout << "Normal mapping on\n";
		else std::cout << "Normal mapping off\n";
		
	};
}
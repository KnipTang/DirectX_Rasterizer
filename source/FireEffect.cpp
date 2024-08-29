#include "pch.h"
#include "FireEffect.h"
#include "Texture.h"
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

namespace dae {
	FireEffect::FireEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
		: BaseEffect(pDevice, assetFile)
	{
		m_pDiffuseTexture = Texture::LoadFromFile(pDevice, "Resources/fireFX_diffuse.png");

		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseMapVariable->IsValid())
		{
			std::wcout << L"m_pDiffuseMapVariable not valid!\n";
		}

		SetDiffuseMap(m_pDiffuseTexture);
	}

	FireEffect::~FireEffect()
	{
		if (m_pDiffuseTexture != nullptr)
		{
			delete m_pDiffuseTexture;
			m_pDiffuseTexture = nullptr;
		}
	}

	void FireEffect::SetDiffuseMap(Texture* pDiffuseTexture)
	{
		if (m_pDiffuseMapVariable)
			m_pDiffuseMapVariable->SetResource(pDiffuseTexture->GetSRV());
	}
}
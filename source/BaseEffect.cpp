#include "pch.h"
#include "BaseEffect.h"
#include "Texture.h"
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

namespace dae {
	BaseEffect::BaseEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		m_pEffect = LoadEffect(pDevice, assetFile);

		m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
		if (!m_pTechnique->IsValid())
			std::wcout << L"Technique not valid\n";

		m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
		if (!m_pMatWorldViewProjVariable->IsValid())
		{
			std::wcout << L"m_pMatWorldViewProjVariable not valid!\n";
		}

		m_pWorldMatrixVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();
		if (!m_pWorldMatrixVariable->IsValid())
		{
			std::wcout << L"gWorldMatrix not valid!\n";
		}

		m_pSampleState = m_pEffect->GetVariableByName("gTextureSampler")->AsSampler();
		if (!m_pSampleState->IsValid())
		{
			std::wcout << L"m_pSampleState not valid!\n";
		}
	}

	BaseEffect::~BaseEffect()
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}

	void BaseEffect::SetWorldViewProjectionMatrix(Matrix worldViewProjectMatrix)
	{
		m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<const float*>(&worldViewProjectMatrix));
	}
	void BaseEffect::SetWorldMatrix(Matrix worldMatrix)
	{
		m_pWorldMatrixVariable->SetMatrix(reinterpret_cast<const float*>(&worldMatrix));
	}

	void BaseEffect::SwitchBetweenTechnique(ID3D11Device* pDevice, int technique)
	{
		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		switch (technique)
		{
		case 0:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			std::cout << "SampleState = Point" << '\n';
			break;
		case 1:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			std::cout << "SampleState = Linear" << '\n';
			break;
		case 2:
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			std::cout << "SampleState = Anisotropic" << '\n';
			break;
		}
		ID3D11SamplerState* SamplerState = nullptr;
		pDevice->CreateSamplerState(&samplerDesc, &SamplerState);
		m_pSampleState->SetSampler(0, SamplerState);

		SamplerState->Release();
		SamplerState = nullptr;
	}

	ID3DX11Effect* BaseEffect::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		HRESULT result;
		ID3D10Blob* pErrorBlob{ nullptr };
		ID3DX11Effect* pEffect{ nullptr };

		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		result = D3DX11CompileEffectFromFile(assetFile.c_str(),
			nullptr,
			nullptr,
			shaderFlags,
			0,
			pDevice,
			&pEffect,
			&pErrorBlob);

		if (FAILED(result))
		{
			if (pErrorBlob != nullptr)
			{
				const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

				std::wstringstream ss;
				for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
					ss << pErrors[i];
				OutputDebugStringW(ss.str().c_str());
				pErrorBlob->Release();
				pErrorBlob = nullptr;
				std::wcout << ss.str() << std::endl;

			}
			else
			{
				std::wstringstream ss;
				ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
				std::wcout << ss.str() << std::endl;
				return nullptr;
			}
		}
		return pEffect;
	}
}
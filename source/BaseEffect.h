#pragma once
#include <string>

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

namespace dae
{
	class Texture;

	class BaseEffect
	{
	public:
		BaseEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~BaseEffect();

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
		ID3DX11Effect* GetEffect() { return m_pEffect; };
		ID3DX11EffectTechnique* GetTechnique() { return m_pTechnique; };
		void SetWorldViewProjectionMatrix(Matrix worldViewProjectMatrix);
		void SetWorldMatrix(Matrix worldMatrix);
		void SwitchBetweenTechnique(ID3D11Device* pDevice, int technique);
	protected:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable{};
		ID3DX11EffectSamplerVariable* m_pSampleState{};

	};
}


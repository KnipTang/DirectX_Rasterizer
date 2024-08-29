#pragma once
#include <string>

#include "BaseEffect.h"

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

namespace dae
{
	class Texture;

	class Effect final : public BaseEffect
	{
	public:
		Effect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~Effect();

		void SetCameraOrigin(Vector3 origin);
		void SetDiffuseMap(Texture* pDiffuseTexture);
		void SetNormalMap(Texture* pNormalTexture);
		void SetSpecularMap(Texture* pSpecularTexture);
		void SetGlossMap(Texture* pGlossTexture);
		void NormalMappingToggle();
	private:
		
		ID3DX11EffectVectorVariable* m_pCameraOrigin{};
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable{};
		ID3DX11EffectShaderResourceVariable* m_pGlossMapVariable{};
		Texture* m_pDiffuseTexture;
		Texture* m_pGlossTexture;
		Texture* m_pNormalTexture;
		Texture* m_pSpecularTexture;

		ID3DX11EffectVariable* m_NormalMappingOnVariable;
		bool m_NormalMappingOn = true;
	};
}

#pragma once
#include <string>

#include "BaseEffect.h"

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

namespace dae
{
	class Texture;

	class FireEffect final : public BaseEffect
	{
	public:
		FireEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
		~FireEffect();

		void SetDiffuseMap(Texture* pDiffuseTexture);
	private:

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
		Texture* m_pDiffuseTexture;
	};
}

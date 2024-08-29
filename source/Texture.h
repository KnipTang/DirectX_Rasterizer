#pragma once
#include <d3d11.h>
#include <SDL_surface.h>
#include <string>
#include "ColorRGB.h"

namespace dae
{
	struct Vector2;

	class Texture
	{
	public:
		~Texture();

		static Texture* LoadFromFile(ID3D11Device* devicePtr, const std::string& path);
		ID3D11ShaderResourceView* GetSRV() { return m_pSRV; };
	private:
		Texture(ID3D11Device* pDevice, SDL_Surface* pSurface);

		ID3D11ShaderResourceView* m_pSRV{};
		ID3D11Texture2D* m_pResource{};
	};

}
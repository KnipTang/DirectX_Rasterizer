#pragma once
#include "Mesh.h"
#include "Camera.h"
//class Mesh;
struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class FireEffect;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;
		void CycleSampleState();
		void RotationSwitch() { m_Rotation = !m_Rotation; if (m_Rotation) std::cout << "Rotation on\n"; else std::cout << "Rotation off\n";};
		void FireMeshSwitch() { m_FireMesh = !m_FireMesh; if (m_FireMesh) std::cout << "FireFX on\n"; else std::cout << "FireFX off\n"; };
		void NormalSwitch() { m_pEffect->NormalMappingToggle(); };
	private:
		SDL_Window* m_pWindow{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		Mesh* m_pMeshPrimitive{};
		Mesh* m_pMeshFire{};
		Effect* m_pEffect{};
		FireEffect* m_pFireEffect{};
		//DIRECTX
		HRESULT InitializeDirectX();
		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};
		IDXGISwapChain* m_pSwapChain{};
		ID3D11Texture2D* m_pDepthStencilBuffer{};
		ID3D11DepthStencilView* m_pDepthStencilView{};
		ID3D11Resource* m_pRenderTargetBuffer{};
		ID3D11RenderTargetView* m_pRenderTargetView{};

		Camera m_Camera{};

		int m_CurrentTechnique = 0;

		bool m_Rotation = true;
		bool m_FireMesh = true;
	};
}

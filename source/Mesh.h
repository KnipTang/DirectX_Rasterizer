#pragma once
#include "Effects.h"
#include "Texture.h"
#include <string>
#include <vector>


struct ID3D11Device;
struct Vertex;
namespace dae
{
	struct Vertex_In
	{
		Vector3 position;
		Vector3 color;
		Vector2 uv;
		Vector3 normal;
		Vector3 tangent;
	};

	class Mesh final
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::vector<Vertex_In>& vertices, const std::vector<uint32_t>& indices, const std::string& pathModel, BaseEffect* effect);
		~Mesh();
		void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjectionMatrix);
		void SetTechnique(ID3D11Device* pDevice, int technique);
		void UpdateRotation(const Timer* pTimer);
	private:
		void Initialize(ID3D11Device* pDevice);
		ID3DX11EffectTechnique* m_pTechnique;
		ID3D11InputLayout* m_pInputLayout;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
		uint32_t m_NumIndices;
		BaseEffect* m_pEffect;
		Matrix m_pWorldMatrix{ {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0} };
		std::vector<Vertex_In> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}

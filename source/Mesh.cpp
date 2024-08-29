#include "pch.h"
#include "Mesh.h"
#include "Effects.h"
#include "Utils.h"
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

namespace dae {
	Mesh::Mesh(ID3D11Device* pDevice, const std::vector<Vertex_In>& vertices, const std::vector<uint32_t>& indices, const std::string& pathModel, BaseEffect* effect) :
		m_Vertices{ vertices },
		m_Indices{ indices },
		m_pEffect{ effect }
	{
		Utils::ParseOBJ(pathModel, m_Vertices, m_Indices);
		//Create Vertex Layout
		Initialize(pDevice);
	}
	Mesh::~Mesh()
	{
		m_pInputLayout->Release();
		m_pInputLayout = nullptr;
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;

	}

	void Mesh::SetTechnique(ID3D11Device* pDevice, int technique)
	{
		m_pEffect->SwitchBetweenTechnique(pDevice, technique);
	}

	void Mesh::UpdateRotation(const Timer* pTimer)
	{
		const float deltaTime = pTimer->GetElapsed();

		constexpr float rotationRate = 1.0f;
		const float rotationAngle = rotationRate * deltaTime;

		const Matrix rotationMatrix = Matrix::CreateRotationY(rotationAngle);
		m_pWorldMatrix = rotationMatrix * m_pWorldMatrix;
	}

	void Mesh::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjectionMatrix)
	{

		//1 Set Primitive Topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//2 Set Input Layout
		pDeviceContext->IASetInputLayout(m_pInputLayout);
		Matrix worldViewProjection = m_pWorldMatrix * viewProjectionMatrix;
		m_pEffect->SetWorldViewProjectionMatrix(worldViewProjection);
		m_pEffect->SetWorldMatrix(m_pWorldMatrix);
		//3 Set VertexBuffer
		constexpr UINT stride = sizeof(Vertex_In);
		constexpr UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		//4. Set IndexBuffer
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//5. Draw
		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_pEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}

	void Mesh::Initialize(ID3D11Device* pDevice)
	{
		static constexpr uint32_t numElements{ 4 };
		D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

		vertexDesc[0].SemanticName = "POSITION";
		vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[0].AlignedByteOffset = 0;
		vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[1].SemanticName = "TEXCOORD";
		vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[1].AlignedByteOffset = 24;
		vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[2].SemanticName = "NORMAL";
		vertexDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[2].AlignedByteOffset = 32;
		vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[3].SemanticName = "TANGENT";
		vertexDesc[3].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[3].AlignedByteOffset = 44;
		vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		//Create Input Layout
		D3DX11_PASS_DESC passDesc{};

		m_pTechnique = m_pEffect->GetTechnique();
		m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

		HRESULT result = pDevice->CreateInputLayout(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_pInputLayout);
		if (FAILED(result))
			SDL_assert(false); //or return

		// Create vertex buffer
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex_In) * static_cast<uint32_t>(m_Vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = m_Vertices.data();

		result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
		if (FAILED(result))
			return;

		//Create index buffer
		m_NumIndices = static_cast<uint32_t>(m_Indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = m_Indices.data();
		result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
		if (FAILED(result))
			return;
	}

}

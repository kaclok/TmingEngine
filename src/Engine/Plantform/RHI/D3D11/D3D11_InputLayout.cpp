
//= IMPLEMENTATION ===============
#include "../RHI_Implementation.h"
#ifdef API_GRAPHICS_D3D11
//================================

//= INCLUDES =====================
#include "../RHI_InputLayout.h"
#include "../RHI_Device.h"
#include "../../Logging/Log.h"
//================================

//==================
using namespace std;
//==================

namespace TmingEngine
{
	RHI_InputLayout::RHI_InputLayout(const shared_ptr<RHI_Device>& rhi_device)
	{
		m_rhi_device = rhi_device;
	}

	RHI_InputLayout::~RHI_InputLayout()
	{
		safe_release(static_cast<ID3D11InputLayout*>(m_resource));
	}

	bool RHI_InputLayout::Create(void* vertex_shader_blob, const RHI_Vertex_Attribute_Type vertex_attributes)
	{
		if (!vertex_shader_blob)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return false;
		}
		m_vertex_attributes = vertex_attributes;
		
		// Fill in attribute descriptions
		vector<D3D11_INPUT_ELEMENT_DESC> attribute_desc;
		if (m_vertex_attributes & Vertex_Attribute_Position2d)
		{
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (m_vertex_attributes & Vertex_Attribute_Position3d)
		{
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (m_vertex_attributes & Vertex_Attribute_Texture)
		{
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (m_vertex_attributes & Vertex_Attribute_Color8)
		{
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (m_vertex_attributes & Vertex_Attribute_Color32)
		{
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (m_vertex_attributes & Vertex_Attribute_NormalTangent)
		{
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			attribute_desc.emplace_back(D3D11_INPUT_ELEMENT_DESC{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		// Create input layout
		auto d3d_blob = static_cast<ID3D10Blob*>(vertex_shader_blob);
		if (FAILED(m_rhi_device->GetContext()->device->CreateInputLayout
		(
			attribute_desc.data(),
			static_cast<UINT>(attribute_desc.size()),
			d3d_blob->GetBufferPointer(),
			d3d_blob->GetBufferSize(),
			reinterpret_cast<ID3D11InputLayout**>(&m_resource)
		)))
		{
			LOG_ERROR("Failed to create input layout");
			return false;
		}

		return true;
	}
}
#endif
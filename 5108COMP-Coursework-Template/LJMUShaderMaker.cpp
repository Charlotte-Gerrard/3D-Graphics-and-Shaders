#include "LJMUShaderMaker.h"

#include "PCH.h"
#include "LJMUShaderMaker.h"
#include "RasterizerStateConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "BlendStateConfigDX11.h"
#include "ShaderResourceParameterWriterDX11.h"
#include "SamplerParameterWriterDX11.h"
#include "SamplerStateConfigDX11.h"

using namespace Glyph3;
using namespace LJMUDX;

/////////////////////////////////
// Construct a Material from a 
// custom shader file specification
// and the mask defining the shader
// types.
/////////////////////////////////
MaterialPtr LJMUShaderMaker::MakeCustom(RendererDX11& prdr, std::wstring& pfile, unsigned int pmask)
{
	MaterialPtr tmat = MaterialPtr(new MaterialDX11());
	RenderEffectDX11* tfx = new RenderEffectDX11();

	if (pmask & VERTEX_SHADER_MSK)
		tfx->SetVertexShader(prdr.LoadShader(VERTEX_SHADER, pfile, std::wstring(L"VSMAIN"), std::wstring(L"vs_5_0"), false));

	if (pmask & HULL_SHADER_MSK)
		tfx->SetHullShader(prdr.LoadShader(HULL_SHADER, pfile, std::wstring(L"HSMAIN"), std::wstring(L"hs_5_0")));

	if (pmask & DOMAIN_SHADER_MSK)
		tfx->SetDomainShader(prdr.LoadShader(DOMAIN_SHADER, pfile, std::wstring(L"DSMAIN"), std::wstring(L"ds_5_0")));

	if (pmask & GEOMETRY_SHADER_MSK)
		tfx->SetGeometryShader(prdr.LoadShader(GEOMETRY_SHADER, pfile, std::wstring(L"GSMAIN"), std::wstring(L"gs_5_0")));

	if (pmask & PIXEL_SHADER_MSK)
		tfx->SetPixelShader(prdr.LoadShader(PIXEL_SHADER, pfile, std::wstring(L"PSMAIN"), std::wstring(L"ps_5_0")));

	if (pmask & COMPUTE_SHADER_MSK)
		tfx->SetComputeShader(prdr.LoadShader(COMPUTE_SHADER, pfile, std::wstring(L"CSMAIN"), std::wstring(L"cs_5_0")));

	tmat->Params[VT_PERSPECTIVE].bRender = true;
	tmat->Params[VT_PERSPECTIVE].pEffect = tfx;

	return tmat;

}

//////////////////////////////////
// Setup the Static Mesh Material
//////////////////////////////////
MaterialPtr LJMUShaderMaker::MakeStaticMesh(RendererDX11& prdr)
{
	MaterialPtr tmat = MaterialPtr(new MaterialDX11());
	RenderEffectDX11* tfx = new RenderEffectDX11();

	tfx->SetVertexShader(prdr.LoadShader(VERTEX_SHADER, std::wstring(L"MeshStaticTextured.hlsl"), std::wstring(L"VSMAIN"), std::wstring(L"vs_5_0")));
	tfx->SetPixelShader(prdr.LoadShader(PIXEL_SHADER, std::wstring(L"MeshStaticTextured.hlsl"), std::wstring(L"PSMAIN"), std::wstring(L"ps_5_0")));

	RasterizerStateConfigDX11 RS;
	RS.CullMode = D3D11_CULL_NONE;

	tfx->m_iRasterizerState = prdr.CreateRasterizerState(&RS);
	tmat->Params[VT_PERSPECTIVE].bRender = true;
	tmat->Params[VT_PERSPECTIVE].pEffect = tfx;
	return tmat;
}

//////////////////////////////////
// Setup the Vertex Colour Material
// for Actor Objects.
//////////////////////////////////
MaterialPtr LJMUShaderMaker::MakeActorPhong(RendererDX11& prdr)
{
	MaterialPtr tmat = MaterialPtr(new MaterialDX11());
	RenderEffectDX11* tfx = new RenderEffectDX11();

	tfx->SetVertexShader(prdr.LoadShader(VERTEX_SHADER, std::wstring(L"ImmediateGeometrySolid.hlsl"), std::wstring(L"VSMAIN"), std::wstring(L"vs_4_0")));
	tfx->SetPixelShader(prdr.LoadShader(PIXEL_SHADER, std::wstring(L"ImmediateGeometrySolid.hlsl"), std::wstring(L"PSMAIN"), std::wstring(L"ps_4_0")));

	tmat->Params[VT_PERSPECTIVE].bRender = true;
	tmat->Params[VT_PERSPECTIVE].pEffect = tfx;
	return tmat;
}

//////////////////////////////////
// Setup the UV Mapped Texture Material
// for Actor Objects. 
//////////////////////////////////
MaterialPtr LJMUShaderMaker::MakeActorTextured(RendererDX11& prdr)
{
	MaterialPtr tmat = MaterialPtr(new MaterialDX11());
	RenderEffectDX11* tfx = new RenderEffectDX11();

	tfx->SetVertexShader(prdr.LoadShader(VERTEX_SHADER, std::wstring(L"ImmediateGeometryTextured.hlsl"), std::wstring(L"VSMAIN"), std::wstring(L"vs_4_0")));
	tfx->SetPixelShader(prdr.LoadShader(PIXEL_SHADER, std::wstring(L"ImmediateGeometryTextured.hlsl"), std::wstring(L"PSMAIN"), std::wstring(L"ps_4_0")));

	tmat->Params[VT_PERSPECTIVE].bRender = true;
	tmat->Params[VT_PERSPECTIVE].pEffect = tfx;

	tmat->Parameters.SetShaderResourceParameter(L"ColorTexture", nullptr);

	SamplerStateConfigDX11 SamplerConfig;
	SamplerConfig.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerConfig.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerConfig.MaxAnisotropy = 0;

	int LinearSampler = RendererDX11::Get()->CreateSamplerState(&SamplerConfig);
	tmat->Parameters.SetSamplerParameter(L"LinearSampler", LinearSampler);

	return tmat;
}
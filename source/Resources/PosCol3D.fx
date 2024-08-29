float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorldMatrix : WORLD;
float3 gCameraPosition : CAMERA;

SamplerState gTextureSampler : Sampler;

Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossinessMap : GlossinessMap;

bool gNormalMappingOn = true;

float3 gLightDirection = float3(0.577f, -0.577f, 0.577f);

static const float PI = 3.14159265359f;
static const float gLightIntensity = 7.0f;
static const float gShininess = 25.0f;

//custom
static const float3 gAmbient = float3(0.03f, 0.03f, 0.03f);
static const float gLambertCoefficient = 1.0f;
static const float gLambertDiffuse = gLambertCoefficient / PI;

RasterizerState gRasterizerState
{
    CullMode = back;
    FrontCounterClockwise = false; // default
};

BlendState gBlendState
{
    BlendEnable[0] = false;
    SrcBlend = src_alpha;
    DestBlend = inv_src_alpha;
    BlendOp = add;
    SrcBlendAlpha = zero;
    DestBlendAlpha = zero;
    BlendOpAlpha = add;
    RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState gDepthStencilState
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = less;
    StencilEnable = false;

    StencilReadMask = 0x0F;
    StencilWriteMask = 0x0F;

    FrontFaceStencilFunc = always;
    BackFaceStencilFunc = always;

    FrontFaceStencilDepthFail = keep;
    BackFaceStencilDepthFail = keep;

    FrontFaceStencilPass = keep;
    BackFaceStencilPass = keep;

    FrontFaceStencilFail = keep;
    BackFaceStencilFail = keep;
};
//Input/Output Structs
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TextureUV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TextureUV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 WorldPosition : WORLD_POSITION;
};


//Vertex Shader
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.Position = mul(float4(input.Position, 1.0f), gWorldViewProj);
    output.TextureUV = input.TextureUV;

    output.Normal = mul(input.Normal, (float3x3) gWorldMatrix);
    output.Tangent = mul(input.Tangent, (float3x3) gWorldMatrix);
    output.WorldPosition = normalize(gCameraPosition - output.Position.xyz);

    return output;
}

// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    if (gNormalMappingOn)
    {
        float3 sampledNormal = gNormalMap.Sample(gTextureSampler, input.TextureUV).xyz; //
        sampledNormal = 2.f * sampledNormal - float3(1.f, 1.f, 1.f); //
        float3 binormal = cross(input.Normal, input.Tangent); //
        float3x3 tangentSpaceAxis = float3x3(input.Tangent, binormal, input.Normal); //
    
        input.Normal = mul(sampledNormal, tangentSpaceAxis);
        input.Normal = normalize(input.Normal);
    }
    
    float cosTheta = dot(input.Normal, -gLightDirection); //
    cosTheta = max(0.0f, cosTheta); //
    
    //Diffuse
    const float3 textureColor = gDiffuseMap.Sample(gTextureSampler, input.TextureUV); //

    // Ambient
    const float3 ambientContribution = gAmbient * textureColor; //
	
	//Phoge//Specular
    const float3 reflection = reflect(gLightDirection, input.Normal); //
    const float specularColor = gSpecularMap.Sample(gTextureSampler, input.TextureUV).r; //
    const float glossiness = gGlossinessMap.Sample(gTextureSampler, input.TextureUV).r * gShininess; //
    const float specularIntensity = specularColor * pow(max(0.0f, dot(reflection, input.WorldPosition)), glossiness); //
    const float3 specularContribution = float3(specularIntensity, specularIntensity, specularIntensity); //
    
    float3 finalColor = float3((ambientContribution + specularContribution + (gLambertDiffuse * textureColor)) * cosTheta * gLightIntensity);

    return float4(finalColor, 1.0f);
}

//Technique
technique11 DefaultTechnique
{
    pass P0
    {
        SetRasterizerState(gRasterizerState);
        SetDepthStencilState(gDepthStencilState, 0);
        SetBlendState(gBlendState, float4(0.f, 0.f, 0.f, 0.f), 0xFFFFFFFF);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}

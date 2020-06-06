struct VOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD0)
{
	VOut output;

	output.position = position;
	output.texCoord = texCoord;

	return output;
}
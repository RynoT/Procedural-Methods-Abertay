////////////////////////////////////////////////////////////////////////////////
// Filename: horizontalblur.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ScreenSizeBuffer
{
	float screenWidth;
	float screenHeight;
	float2 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ConvolutionVertexShader(VertexInputType input)
{
    PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	float texSizeW = 1.0f / screenWidth, texSizeH = 1.0f / screenHeight;
	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	output.texCoord1 = input.tex + float2(texSizeW * -1.0f, texSizeH * -1.0f);
	output.texCoord2 = input.tex + float2(texSizeW *  0.0f, texSizeH * -1.0f);
	output.texCoord3 = input.tex + float2(texSizeW *  1.0f, texSizeH * -1.0f);
	output.texCoord4 = input.tex + float2(texSizeW * -1.0f, texSizeH *  0.0f);
	output.texCoord5 = input.tex + float2(texSizeW *  0.0f, texSizeH *  0.0f);
	output.texCoord6 = input.tex + float2(texSizeW *  1.0f, texSizeH *  0.0f);
	output.texCoord7 = input.tex + float2(texSizeW * -1.0f, texSizeH *  1.0f);
	output.texCoord8 = input.tex + float2(texSizeW *  0.0f, texSizeH *  1.0f);
	output.texCoord9 = input.tex + float2(texSizeW *  1.0f, texSizeH *  1.0f);

    return output;
}
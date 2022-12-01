struct Output
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoords : TEX_COORDS;
    float2 TexIndex : TEX_INDEX;
};

cbuffer Transform
{
    matrix VP;
};

Output main(
    float3 pos : Position, 
    float4 color : Color, 
    float3 normal : Normal, 
    float2 texCoords : TexCoords, 
    float texIndex : TexIndex
)
{
    Output output;
    output.Position = mul(float4(pos, 1.0f), VP);
    output.Color = color;
    output.Normal = normal;
    output.TexCoords = texCoords;
    output.TexIndex = texIndex;
    return output;
}

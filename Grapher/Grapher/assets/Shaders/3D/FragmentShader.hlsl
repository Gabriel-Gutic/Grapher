
Texture2D u_Textures[32] : register(t0);
SamplerState u_SamplerState;

// Converts from linear RGB space to gamma.
float3 LinearToGamma(float3 color)
{
	return pow(color, 1 / 2.2);
}

float4 main(
	float4 pos : SV_POSITION, 
	float4 color : COLOR, 
	float3 normal : NORMAL, 
	float2 texCoords : TEX_COORDS, 
	float texIndex : TEX_INDEX
) : SV_TARGET
{
	if (texIndex > -1.0)
	{
		int index = int(texIndex + 0.1);
		float4 tex;
		switch (index)
		{
		case 0:  tex = u_Textures[ 0].Sample(u_SamplerState, texCoords); break;
		case 1:  tex = u_Textures[ 1].Sample(u_SamplerState, texCoords); break;
		case 2:  tex = u_Textures[ 2].Sample(u_SamplerState, texCoords); break;
		case 3:  tex = u_Textures[ 3].Sample(u_SamplerState, texCoords); break;
		case 4:  tex = u_Textures[ 4].Sample(u_SamplerState, texCoords); break;
		case 5:  tex = u_Textures[ 5].Sample(u_SamplerState, texCoords); break;
		case 6:  tex = u_Textures[ 6].Sample(u_SamplerState, texCoords); break;
		case 7:  tex = u_Textures[ 7].Sample(u_SamplerState, texCoords); break;
		case 8:  tex = u_Textures[ 8].Sample(u_SamplerState, texCoords); break;
		case 9:  tex = u_Textures[ 9].Sample(u_SamplerState, texCoords); break;
		case 10: tex = u_Textures[10].Sample(u_SamplerState, texCoords); break;
		case 11: tex = u_Textures[11].Sample(u_SamplerState, texCoords); break;
		case 12: tex = u_Textures[12].Sample(u_SamplerState, texCoords); break;
		case 13: tex = u_Textures[13].Sample(u_SamplerState, texCoords); break;
		case 14: tex = u_Textures[14].Sample(u_SamplerState, texCoords); break;
		case 15: tex = u_Textures[15].Sample(u_SamplerState, texCoords); break;
		case 16: tex = u_Textures[16].Sample(u_SamplerState, texCoords); break;
		case 17: tex = u_Textures[17].Sample(u_SamplerState, texCoords); break;
		case 18: tex = u_Textures[18].Sample(u_SamplerState, texCoords); break;
		case 19: tex = u_Textures[19].Sample(u_SamplerState, texCoords); break;
		case 20: tex = u_Textures[20].Sample(u_SamplerState, texCoords); break;
		case 21: tex = u_Textures[21].Sample(u_SamplerState, texCoords); break;
		case 22: tex = u_Textures[22].Sample(u_SamplerState, texCoords); break;
		case 23: tex = u_Textures[23].Sample(u_SamplerState, texCoords); break;
		case 24: tex = u_Textures[24].Sample(u_SamplerState, texCoords); break;
		case 25: tex = u_Textures[25].Sample(u_SamplerState, texCoords); break;
		case 26: tex = u_Textures[26].Sample(u_SamplerState, texCoords); break;
		case 27: tex = u_Textures[27].Sample(u_SamplerState, texCoords); break;
		case 28: tex = u_Textures[28].Sample(u_SamplerState, texCoords); break;
		case 29: tex = u_Textures[29].Sample(u_SamplerState, texCoords); break;
		case 30: tex = u_Textures[30].Sample(u_SamplerState, texCoords); break;
		case 31: tex = u_Textures[31].Sample(u_SamplerState, texCoords); break;
		}
		return float4(LinearToGamma(tex.xyz), tex.w);
	}
	return color;
}


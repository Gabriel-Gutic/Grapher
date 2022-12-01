#version 330 core

in vec4 o_Color;
in vec3 o_Normal;
in vec2 o_TexCoords;
in float o_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
	//vec3 light = normalize(vec3(0.0, -2.0, 0.0));
	//vec3 normal = normalize(o_Normal);
	//float prod = max(dot(-light, normal), 0.0);
	if (o_TexIndex < 0)
	{
		FragColor = o_Color;
	}
	else
	{
		int index = int(o_TexIndex + 0.1);
		//vec4 tex = texture(u_Textures[index], o_TexCoords);
		//FragColor = vec4(prod * tex.xyz, tex.w);
		FragColor = texture(u_Textures[index], o_TexCoords);
	}
}


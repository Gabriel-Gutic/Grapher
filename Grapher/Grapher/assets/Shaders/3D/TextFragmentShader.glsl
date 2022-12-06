#version 330 core

in vec4 o_Color;
in vec3 o_Normal;
in vec2 o_TexCoords;
in float o_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
	if (o_TexIndex < 0)
	{
		FragColor = o_Color;
	}
	else
	{
		int index = int(o_TexIndex + 0.1);
		vec4 tex = texture(u_Textures[index], o_TexCoords);
		FragColor = vec4(o_Color.xyz, tex.r);
	}
}

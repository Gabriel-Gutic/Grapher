#version 330 core

in vec4 Color;
in float TexIndex;
in vec2 TexCoords;
in float Type;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
	if (TexIndex < 0)
		FragColor = Color;
	else 
	{
		int index = int(TexIndex + 0.1f);
		int type = int(Type + 0.1f);

		if (type == 0)
		{
			FragColor = texture(u_Textures[index], TexCoords);
		}
		else if (type == 1)
		{
			float r = texture(u_Textures[index], TexCoords).r;

			if (r < 0.1)
				discard;

			vec4 sampled = vec4(1.0, 1.0, 1.0, r);
			FragColor = Color * sampled;
		}
		else 
		{
			FragColor = Color;
		}
	}
}


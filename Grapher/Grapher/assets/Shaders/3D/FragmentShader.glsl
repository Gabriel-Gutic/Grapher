#version 330 core

uniform vec3 u_CameraPosition;
uniform vec3 u_CameraTarget;

uniform float u_IsUsingLight;
uniform vec3 u_LightSource;

in vec3 o_Position;
in vec4 o_Color;
in vec3 o_Normal;
in vec2 o_TexCoords;
in float o_TexIndex;

out vec4 FragColor;

uniform sampler2D u_Textures[32];

void main()
{
	//
	vec3 cp = u_CameraPosition;
	vec3 ct = u_CameraTarget;
	//

	vec3 light = normalize(o_Position - u_LightSource);
	vec3 normal = normalize(o_Normal);
	float prod = max(dot(-light, normal), 0.8);

	if (o_TexIndex < 0)
	{
		if (u_IsUsingLight > 0.0f)
			FragColor = vec4((prod * o_Color.rgb), o_Color.a);
		else
			FragColor = o_Color;
	}
	else
	{
		int index = int(o_TexIndex + 0.1);

		if (u_IsUsingLight > 0.0f)
			FragColor =vec4((prod * o_Color.rgb), o_Color.a) * texture(u_Textures[index], o_TexCoords);
		else
			FragColor = o_Color * texture(u_Textures[index], o_TexCoords);
	}
}


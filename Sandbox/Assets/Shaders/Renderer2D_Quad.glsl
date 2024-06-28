#type vertex
#version 450 core

layout(location = 0) in vec3  a_Location;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in int   a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int   a_EntityID;

layout(std140, binding = 0) uniform FCamera
{
	mat4 u_ViewProjection;
};

struct FVertexOutput
{
	vec4 m_Color;
	vec2 m_TexCoord;
	float m_TilingFactor;
};

layout(location = 0) out FVertexOutput Output;
layout(location = 3) out flat int v_TexIndex;
layout(location = 4) out flat int v_EntityID;

void main()
{
	Output.m_Color = a_Color;
	Output.m_TexCoord = a_TexCoord;
	Output.m_TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Location, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct FVertexOutput
{
	vec4 m_Color;
	vec2 m_TexCoord;
	float m_TilingFactor;
};

layout(location = 0) in FVertexOutput Input;
layout(location = 3) in flat int v_TexIndex;
layout(location = 4) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 color = Input.m_Color * texture(u_Textures[v_TexIndex], Input.m_TexCoord * Input.m_TilingFactor);

	if (color.a == 0.0)
	{
		discard;
	}

	o_Color = color;
	o_EntityID = v_EntityID;
}

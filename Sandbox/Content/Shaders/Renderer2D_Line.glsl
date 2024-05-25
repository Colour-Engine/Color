#type vertex
#version 450 core

layout(location = 0) in vec3 a_Location;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int  a_EntityID;

layout(std140, binding = 0) uniform FCamera
{
	mat4 u_ViewProjection;
};

struct FVertexOutput
{
	vec4 m_Color;
};

layout (location = 0) out FVertexOutput Output;
layout (location = 1) out flat int v_EntityID;

void main()
{
	Output.m_Color = a_Color;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Location, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_Color;
layout (location = 1) out int o_EntityID;

struct FVertexOutput
{
	vec4 m_Color;
};

layout (location = 0) in FVertexOutput Input;
layout (location = 1) in flat int v_EntityID;

void main()
{
	o_Color = Input.m_Color;
	o_EntityID = v_EntityID;
}

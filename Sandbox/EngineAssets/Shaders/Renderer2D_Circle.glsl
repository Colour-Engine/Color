#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldLocation;
layout(location = 1) in vec3 a_LocalLocation;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform FCamera
{
	mat4 u_ViewProjection;
};

struct FVertexOutput
{
	vec3 m_LocalLocation;
	vec4 m_Color;
	float m_Thickness;
	float m_Fade;
};

layout(location = 0) out FVertexOutput Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
	Output.m_LocalLocation = a_LocalLocation;
	Output.m_Color = a_Color;
	Output.m_Thickness = a_Thickness;
	Output.m_Fade = a_Fade;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_WorldLocation, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct FVertexOutput
{
	vec3 m_LocalLocation;
	vec4 m_Color;
	float m_Thickness;
	float m_Fade;
};

layout(location = 0) in FVertexOutput Input;
layout(location = 4) in flat int v_EntityID;

void main()
{
	float Distance = 1.0 - length(Input.m_LocalLocation);
	float Circle = smoothstep(0.0, Input.m_Fade, Distance) * smoothstep(Input.m_Thickness + Input.m_Fade, Input.m_Thickness, Distance);

	if (Circle == 0.0)
	{
		discard;
	}

	o_Color = Input.m_Color;
	o_Color.a *= Circle;

	o_EntityID = v_EntityID;
}

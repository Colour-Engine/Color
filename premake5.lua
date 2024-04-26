workspace "Color"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Development",
		"Preview",
		"Shipping"
	}

include "Paths.lua"

group "Engine/ThirdParty"
	include "Color/ThirdParty/GLFW"
	include "Color/ThirdParty/Glad"
	include "Color/ThirdParty/stb"
group ""

group "Engine"
	include "Color"
group ""

group "Templates"
	include "Sandbox"
group ""

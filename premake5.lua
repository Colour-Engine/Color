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

group "Engine"
	include "Color"
group ""

group "Templates"
	include "Sandbox"
group ""

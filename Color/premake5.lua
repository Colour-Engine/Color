project "Color"
	language "C++"
	cppdialect "C++20"
	kind "StaticLib"
	
	targetdir "%{OutputDir.Binaries}/%{prj.name}"
	objdir    "%{OutputDir.Intermediates}/%{prj.name}"
	
	pchheader "ColorPCH.h"
	pchsource "Source/ColorPCH.cpp"
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs
	{
		"Source",
		"Source/Color",

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.VulkanSDK}"
	}
	
	defines
	{
		"GLFW_INCLUDE_NONE"
	}

	links
	{
		"GLFW",
		"Glad",
		"stb"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"CL_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS", -- Disable stupid CRT compilation "warning error"s.
		}
		excludes
		{
			"Source/Platform/Linux/**.cpp"
		}

	filter "system:linux"
		defines
		{
			"CL_PLATFORM_LINUX"
		}
		excludes
		{
			"Source/Platform/Windows/**.cpp"
		}
	
	filter "configurations:Development"
		runtime "Debug"
		symbols "On"
		defines
		{
			"CL_DEVELOPMENT"
		}
		links
		{
			"%{Libraries.ShaderC_Debug}",
			"%{Libraries.SPIRV_Cross_Debug}",
			"%{Libraries.SPIRV_Cross_GLSL_Debug}"
		}
	
	filter "configurations:Preview"
		runtime "Release"
		optimize "Speed"
		defines
		{
			"CL_PREVIEW"
		}
		links
		{
			"%{Libraries.ShaderC_Release}",
			"%{Libraries.SPIRV_Cross_Release}",
			"%{Libraries.SPIRV_Cross_GLSL_Release}"
		}
	
	filter "configurations:Shipping"
		runtime "Release"
		optimize "Speed"
		defines
		{
			"CL_SHIPPING"
		}
		links
		{
			"%{Libraries.ShaderC_Release}",
			"%{Libraries.SPIRV_Cross_Release}",
			"%{Libraries.SPIRV_Cross_GLSL_Release}"
		}
	
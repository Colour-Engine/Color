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
		"Source/Color"
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
	
	filter "configurations:Preview"
		runtime "Release"
		optimize "Speed"
		defines
		{
			"CL_PREVIEW"
		}
	
	filter "configurations:Shipping"
		runtime "Release"
		optimize "Speed"
		defines
		{
			"CL_SHIPPING"
		}
	
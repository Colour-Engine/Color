project "ColorHeaderTool"
	language "C++"
	cppdialect "C++20"
	kind "ConsoleApp"
	
	targetdir "%{OutputDir.Binaries}/%{prj.name}"
	objdir    "%{OutputDir.Intermediates}/%{prj.name}"

	pchheader "HeaderToolPCH.h"
	pchsource "Source/HeaderToolPCH.cpp"
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs
	{
		"%{IncludeDir.Color}",
		"%{SourceDir.Color}",
		"Source",
		
		"%{IncludeDir.glm}"
	}

	links
	{
		"Color"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"CL_PLATFORM_WINDOWS"
		}
		
	filter "system:linux"
		defines
		{
			"CL_PLATFORM_LINUX"
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
	
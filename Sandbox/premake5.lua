project "Sandbox"
	language "C++"
	cppdialect "C++17"
	
	targetdir "%{OutputDir.Binaries}/%{prj.name}"
	objdir    "%{OutputDir.Intermediates}/%{prj.name}"
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs
	{
		"%{IncludeDir.Color}",
		"%{SourceDir.Color}",
		"Source"
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
			"CL_PLATFORM_LINUXU"
		}
	
	filter "configurations:Development"
		runtime "Debug"
		symbols "On"
		kind "ConsoleApp"
		defines
		{
			"CL_DEVELOPMENT"
		}
	
	filter "configurations:Preview"
		runtime "Release"
		optimize "Speed"
		kind "ConsoleApp"
		defines
		{
			"CL_PREVIEW"
		}
	
	filter "configurations:Shipping"
		runtime "Release"
		optimize "Speed"
		kind "WindowedApp"
		defines
		{
			"CL_SHIPPING"
		}
	
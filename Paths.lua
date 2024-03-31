OutputDir = { }
OutputDir["Completer"] = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OutputDir["Binaries"] = "%{wks.location}/Binaries/%{OutputDir.Completer}"
OutputDir["Intermediates"] = "%{wks.location}/Binaries-Int/%{OutputDir.Completer}"

ProjectDir = { }
ProjectDir["Color"] = "%{wks.location}/Color"
ProjectDir["Sandbox"] = "%{wks.location}/Sandbox"
ProjectDir["GLFW"] = "%{ProjectDir.Color}/ThirdParty/GLFW"
ProjectDir["Glad"] = "%{ProjectDir.Color}/ThirdParty/Glad"

IncludeDir = { }
IncludeDir["Color"] = "%{ProjectDir.Color}/Source/Color"
IncludeDir["Sandbox"] = "%{ProjectDir.Sandbox}/Source"
IncludeDir["GLFW"] = "%{ProjectDir.GLFW}/include"
IncludeDir["Glad"] = "%{ProjectDir.Glad}/include"
IncludeDir["glm"] = "%{ProjectDir.Color}/ThirdParty/glm"

SourceDir = { }
SourceDir["Color"] = "%{ProjectDir.Color}/Source"
SourceDir["Sandbox"] = "%{ProjectDir.Sandbox}/Source"
SourceDir["GLFW"] = "%{ProjectDir.GLFW}/src"
SourceDir["Glad"] = "%{ProjectDir.Glad}/src"

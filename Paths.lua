OutputDir = { }
OutputDir["Completer"] = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OutputDir["Binaries"] = "%{wks.location}/Binaries/%{OutputDir.Completer}"
OutputDir["Intermediates"] = "%{wks.location}/Binaries-Int/%{OutputDir.Completer}"

ProjectDir = { }
ProjectDir["Color"] = "%{wks.location}/Color"
ProjectDir["Sandbox"] = "%{wks.location}/Sandbox"

IncludeDir = { }
IncludeDir["Color"] = "%{ProjectDir.Color}/Source/Color"
IncludeDir["Sandbox"] = "%{ProjectDir.Sandbox}/Source"

SourceDir = { }
SourceDir["Color"] = "%{ProjectDir.Color}/Source"
SourceDir["Sandbox"] = "%{ProjectDir.Sandbox}/Source"

VULKAN_SDK = os.getenv("VULKAN_SDK")

OutputDir = { }
OutputDir["Completer"] = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OutputDir["Binaries"] = "%{wks.location}/Binaries/%{OutputDir.Completer}"
OutputDir["Intermediates"] = "%{wks.location}/Binaries-Int/%{OutputDir.Completer}"

ProjectDir = { }
ProjectDir["Color"] = "%{wks.location}/Color"
ProjectDir["Sandbox"] = "%{wks.location}/Sandbox"
ProjectDir["GLFW"] = "%{ProjectDir.Color}/ThirdParty/GLFW"
ProjectDir["Glad"] = "%{ProjectDir.Color}/ThirdParty/Glad"
ProjectDir["stb"] = "%{ProjectDir.Color}/ThirdParty/stb"

IncludeDir = { }
IncludeDir["Color"] = "%{ProjectDir.Color}/Source/Color"
IncludeDir["Sandbox"] = "%{ProjectDir.Sandbox}/Source"
IncludeDir["GLFW"] = "%{ProjectDir.GLFW}/include"
IncludeDir["Glad"] = "%{ProjectDir.Glad}/include"
IncludeDir["stb"] = "%{ProjectDir.stb}/include"
IncludeDir["glm"] = "%{ProjectDir.Color}/ThirdParty/glm"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

SourceDir = { }
SourceDir["Color"] = "%{ProjectDir.Color}/Source"
SourceDir["Sandbox"] = "%{ProjectDir.Sandbox}/Source"
SourceDir["GLFW"] = "%{ProjectDir.GLFW}/src"
SourceDir["Glad"] = "%{ProjectDir.Glad}/src"
SourceDir["stb"] = "%{ProjectDir.stb}/src"

LibraryDir = { }
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Libraries = { }
Libraries["Vulkan"] = "%{LibraryDir.Vulkan}/vulkan-1.lib"
Libraries["VulkanUtils"] = "%{LibraryDir.Vulkan}/VkLayer_utils.lib"
Libraries["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Libraries["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Libraries["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Libraries["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"
Libraries["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Libraries["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Libraries["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

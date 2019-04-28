workspace "CDA"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "CDA/vendor/GLFW/include"
IncludeDir["Glad"] = "CDA/vendor/Glad/include"
IncludeDir["ImGui"] = "CDA/vendor/imgui"
IncludeDir["glm"] = "CDA/vendor/glm"

include "CDA/vendor/GLFW"
include "CDA/vendor/Glad"
include "CDA/vendor/imgui"

project "CDA"
	location "CDA"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cdapch.h"
	pchsource "CDA/src/cdapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CDA_PLATFORM_WINDOWS",
			"CDA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "CDA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CDA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CDA_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"CDA/vendor/spdlog/include",
		"CDA/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"CDA"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CDA_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CDA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CDA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CDA_DIST"
		runtime "Release"
		optimize "On"
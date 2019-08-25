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
IncludeDir["stb_image"] = "CDA/vendor/stb_image"

include "CDA/vendor/GLFW"
include "CDA/vendor/Glad"
include "CDA/vendor/imgui"

project "CDA"
	location "CDA"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cdapch.h"
	pchsource "CDA/src/cdapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CDA_PLATFORM_WINDOWS",
			"CDA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CDA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CDA_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CDA_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"CDA/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"CDA"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CDA_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CDA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CDA_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CDA_DIST"
		runtime "Release"
		optimize "on"
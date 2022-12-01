project "Grapher"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir (bin_dir)
	objdir (bin_int_dir)

	pchheader("pch.h")
	pchsource("src/pch.cpp")

	files 
	{
		"src/**.cpp",
		"src/**.h",
	}

	includedirs
	{
		"src",
		"dependencies/Tomato/Tomato/Tomato/src",
		"dependencies/Tomato/Tomato/Tomato/src/Tomato",
		"dependencies/Tomato/Tomato/Tomato/dependencies/spdlog/include",
		"dependencies/Tomato/Tomato/Tomato/dependencies/entt/src",
		"dependencies/Tomato/Tomato/Tomato/dependencies/ImGui",
		"dependencies/Tomato/Tomato/Tomato/dependencies/yaml-cpp/yaml-cpp/include",
	}

	links
	{
		"Tomato",
	}

	filter "system:windows"
		systemversion "latest"
		kind "WindowedApp"

		defines
		{
			"GRAPHER_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "DEBUG"
		defines "GRAPHER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		defines "GRAPHER_RELEASE"
		runtime "Release"
		symbols "on"
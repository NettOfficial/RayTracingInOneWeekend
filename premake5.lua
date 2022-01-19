workspace "RayTracingInOneWeekend"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RayTracingInOneWeekend"
	location ""
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src/headers",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
		 "_WINDLL;"
		}


		filter "configurations:Debug"
			buildoptions "/MDd"
			symbols "On"
			
		filter "configurations:Release"
			buildoptions "/MD"
			optimize "On"
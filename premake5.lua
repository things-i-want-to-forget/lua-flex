
solution "lua-flex"
	language "C++"
	location "./project"

	configurations "Release"

	-- if this causes an error for you, update premake5
	architecture "x86"

	configuration "Release"
		defines  { "NDEBUG" }
		optimize "On"

	include "lau"


	project "lua-flex"

		flags     "StaticRuntime"
		kind      "SharedLib"

		targetdir "build"

		buildoptions { "/Os", "/MP", "/arch:SSE2" }

		libdirs { "lib", "lau/build" }
		links   { "user32", "lau/lib/lau531" }
		includedirs {
			"lau/src",
			"src"
		}

		files {
			"src/**.cpp",
			"src/**.h",
			"src/**.hpp",
			"lau/src/**.cpp",
			"lau/src/**.h",
			"lau/src/**.hpp"
		}

		defines {
			"AIM_FLEX",
			"WIN32",
			"_WINDOWS",
			"_USRDLL"
		}

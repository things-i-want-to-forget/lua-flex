include "lau"

solution "lua-flex"
	language "C++"
	location "./project"

	configurations "Release"

	-- if this causes an error for you, update premake5
	architecture "x86"

	configuration "Release"
		defines  { "NDEBUG" }
		optimize "On"

	project "lua-flex"

		flags     "StaticRuntime"
		kind      "SharedLib"

		targetdir "build"

		buildoptions { "/Os", "/GL", "/MP", "/arch:SSE2" }

		libdirs "lib"
		links   "lau"
		includedirs "lau/src"

		files {
			"src/**.cpp",
			"src/**.h"
		}

		defines {
			"AIM_FLEX",
			"WIN32",
			"_WINDOWS",
			"_USRDLL"
		}

solution "lua-flex"
	language "C++"
	location ("./proj")
	
	configurations
	{ 
		"Release"
	}
		
	configuration "Release"
		defines {"NDEBUG"}
		optimize "On"

	project "lua-flex_overlay"
		kind "SharedLib"
		
		targetdir "build/"
		
		buildoptions { "/Os", "/MP", "/arch:SSE2" }
		
		files {
			"overlay/**.cpp",
			"overlay/**.h",
		}
		
	
	project "lua-flex"
		kind "SharedLib"

		targetdir ("build/")
		
		buildoptions { "/Os", "/MP", "/arch:SSE2" }
		
		libdirs {
			"lib"
		}
		links {
			"lua53"
		}

		includedirs {
			"lua53"
		}

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

	project "lua53"
		kind "StaticLib"
		files "lua53/**"

		buildoptions { "/Os", "/MP", "/arch:SSE2" }
		
		targetdir ("lib/")

		defines {
			"AIM_FLEX",
			"WIN32",
			"_LIB",
		}
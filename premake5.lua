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
			"lua53",
			"hack",
			"packets",
		}

		files {
			"src/**.cpp",
			"src/**.h",
			"packets/**.h",
			"packets/**.cpp",
			"hack/**.h",
			"hack/**.cpp",
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
		
	project "lua-flex-overlay"
		kind "SharedLib"
		
		targetdir "build/"
		
		buildoptions { "/Os", "/MP", "/arch:SSE2" }
		
		includedirs {
			"hack",
			"packets",
		}
		
		files {
			"overlay/**.cpp",
			"overlay/**.h",
			"packets/**.h",
			"packets/**.cpp",
			"hack/**.h",
			"hack/**.cpp",
		}

		defines {
			"AIM_FLEX_OVERLAY",
			"WIN32",
			"_WINDOWS",
			"_USRDLL"
		}
		
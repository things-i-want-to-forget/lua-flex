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
			"lua53",
			"Ws2_32",
		}

		includedirs {
			"lua53",
			"hack",
			"packets",
		}

		files {
			"src/**.cpp",
			"src/**.h",
			"hack/**.h",
			"hack/**.cpp",
			"packets/**.cpp",
			"packets/**.h",
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
		kind "ConsoleApp"
		
		targetdir "build/"
		
		buildoptions { "/Os", "/MP", "/arch:SSE2" }
		
		includedirs {
			"hack",
			"packets",
			"sdl",
		}
		
		libdirs {
			"lib"
		}
		links {
			"SDL2",
			"opengl32",
			"Dwmapi",
			"Ws2_32",
		}
		
		files {
			"overlay/**.cpp",
			"overlay/**.h",
			"packets/**.h",
			"packets/**.cpp",
			"hack/**.h",
			"hack/**.cpp",
			"sdl/**.h",
		}

		defines {
			"AIM_FLEX_OVERLAY",
			"WIN32",
			"_WINDOWS",
			"_USRDLL"
		}
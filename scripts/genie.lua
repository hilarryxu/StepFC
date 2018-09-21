solution "StepFC"
	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"x32",
		"x64",
		"Native", -- for targets where bitness is not specified
	}

	language "C"

STEP_FC_DIR = path.getabsolute("..")
STEP_FC_BUILD_DIR = path.join(STEP_FC_DIR, ".build")
STEP_FC_THIRD_PARTY_DIR = path.join(STEP_FC_DIR, "3rdparty")

dofile "toolchain.lua"
toolchain(STEP_FC_BUILD_DIR, STEP_FC_THIRD_PARTY_DIR)

removeflags {
	"ExtraWarnings"
}

function copyLib()
end

function example_prj(dir, with_video)
	local name = dir
	project(name)
		kind "ConsoleApp"
		targetdir(path.join(STEP_FC_DIR, dir))

		-- if with_video then
		-- 	language "C++"
		-- end

		includedirs {
			path.join(STEP_FC_DIR, dir),
		}

		files {
			path.join(STEP_FC_DIR, dir .. "/*.c"),
		}
		
		if with_video then
			files {
				path.join(STEP_FC_DIR, "common/d2d_draw.cpp"),
			}
			links {
				"dxgi",
				"dxguid",
				"d2d1",
				"d3d11",
			}
		end

		links {
		}

		configuration {}

		strip()
end

-- example_prj("step0")
-- example_prj("step1")
-- example_prj("step2")
-- example_prj("step3")
example_prj("step4", 'video')
-- example_prj("step5")

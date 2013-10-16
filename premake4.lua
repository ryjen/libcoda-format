package = "format"

newaction {
   trigger     = "indent",
   description = "Format source files",
   execute = function ()
      os.execute("astyle -N -A1 -R '*.cpp' '*.h'");
   end
}
newaction {
    trigger     = "install",
    description = "Install headers and lib",
    execute = function()

    if not _ARGS[1] or not os.isdir(_ARGS[1]) then
      error("You must specify an install location")
    end

    if not os.isdir("bin/release") then
      error("You must make a release build first")
    end

    bindir = _ARGS[1].."/lib/"
    headerdir = _ARGS[1].."/include/arg3/format/"

    if not os.isdir(bindir) then
      os.mkdir(bindir)
    end

    if not os.isdir(headerdir) then
      os.mkdir(headerdir)
    end

    if os.isfile("bin/release/libarg3format.a") then
        os.copyfile("bin/release/libarg3format.a", bindir);
    end

    headers = os.matchfiles("**.h")

    for i=1, #headers do
      os.copyfile(headers[i], headerdir);
    end
}
newaction {
    trigger   = "release",
    description = "builds a release",
    execute = function()
       if( os.isdir("bin")) then
          error("You must run clean first")
       end

       if _ARGS[1] then
         version = _ARGS[1]
       else
         version = "1.0"
       end
       os.execute("tar --exclude=libarg3#{package}_"..version..".tar.gz -czf libarg3#{package}_"..version..".tar.gz .");
    end
}
newoption {
   trigger     = "shared",
   description = "Build a shared library"
}

if _ACTION == "clean" then
    matches = os.matchfiles("**.orig")
    for i=1, #matches do
        os.remove(matches[i])
    end
    matches = os.matchfiles("**.a")
    for i=1, #matches do
        os.remove(matches[i])
    end
    os.rmdir("bin")
    os.rmdir("obj")
end

solution "arg3"
    configurations { "debug", "release" }
    language "C++"

    buildoptions { "-std=c++11", "-stdlib=libc++", "-Wall", "-Werror"}

    linkoptions { "-stdlib=libc++" }

    configuration "Debug"
        flags "Symbols"
        targetdir "bin/debug"
        buildoptions { "-g -DARG3_DEBUG" }
    configuration "release"
        targetdir "bin/release"
        buildoptions { "-O" }

    project "arg3format"
    	if not _OPTIONS["shared"] then
        	kind "StaticLib"
    	else
            kind "SharedLib"
   		 end
            files {
                "**.cpp",
                "**.h"
            }
            excludes {
                "**.test.cpp"
            }

    project "arg3test"
        kind "ConsoleApp"
        files {
            "**.test.cpp"
        }

        links { "arg3format" }

        configuration "Debug"
        postbuildcommands {
          "bin/debug/arg3test"
        }
        configuration "Release"
        postbuildcommands {
          "bin/release/arg3test"
        }



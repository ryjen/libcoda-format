package = "format"
local prefix = _OPTIONS["prefix"] or "./dist/libarg3"..package

local projectname = "arg3"..package

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

    if not os.isdir("bin/release") then
      error("You must make a release build first")
    end

    bindir = prefix.."/lib"
    headerdir = prefix.."/include/arg3/"..package

    if not os.isdir(bindir) then
      os.mkdir(bindir)
    end

    if not os.isdir(headerdir) then
      os.mkdir(headerdir)
    end

    if os.isfile("bin/release/libarg3"..package..".a") then
        os.copyfile("bin/release/libarg3"..package..".a", bindir);
    end

    headers = os.matchfiles("*.h")

    for i=1, #headers do
      if not string.endswith(headers[i], ".test.h") then
        os.copyfile(headers[i], headerdir);
      end
    end

    extra = os.matchdirs("*")

    for i=1, #extra do
      if not string.endswith(extra[i], "vendor") then
        headers = os.matchfiles(extra[i].."/*.h")
        for j=1, #headers do
          if not string.endswith(headers[j], ".test.h") then
            tempdir = headerdir.."/"..extra[i]
            if not os.isdir(tempdir) then
              os.mkdir(tempdir)
            end
            os.copyfile(headers[j], tempdir)
          end
        end
      end
    end
  end
}

newoption {
   trigger     = "prefix",
   description = "Specify prefix for installs"
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
       os.execute("tar --exclude=libarg3"..package.."_"..version..".tar.gz -czf libarg3"..package.."_"..version..".tar.gz .");
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

    project ("arg3"..package)

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

        includedirs { "vendor" }

        links { "arg3"..package }

        configuration "Debug"
        postbuildcommands {
          "bin/debug/arg3test"
        }
        configuration "Release"
        postbuildcommands {
          "bin/release/arg3test"
        }



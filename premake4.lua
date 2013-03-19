project "arg3format"
    kind "StaticLib"
    files {
        "format.h",
        "format.cpp",
        "specifier.h"
    }
    excludes {
        "format.test.cpp"
    }
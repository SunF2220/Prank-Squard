#!/usr/bin/env python

# Prank Squad — GDExtension build script
# Usage: scons platform=windows

import os
import sys

# Godot CPP bindings path
godot_cpp_path = "godot-cpp"

# Ensure godot-cpp submodule exists
if not os.path.isdir(godot_cpp_path):
    print("Error: godot-cpp not found. Run: git submodule update --init")
    sys.exit(1)

# Add godot-cpp to path
sys.path.insert(0, os.path.join(godot_cpp_path, "tools"))

from SCons import ARGUMENTS
from SCons.Script import DefaultEnvironment, Glob, Export, Import

# Detect platform
platform = ARGUMENTS.get("platform", "windows")

# Build godot-cpp first
env = SConscript(os.path.join(godot_cpp_path, "SConstruct"))

# Project source files
sources = Glob("src/cpp/gdextension/*.cpp")
sources += Glob("src/cpp/core/*.cpp")
sources += Glob("src/cpp/generator/*.cpp")
sources += Glob("src/cpp/ai/*.cpp")
sources += Glob("src/cpp/audio/*.cpp")

# Library name
library_name = "PrankSquad"

# Build shared library
library = env.SharedLibrary(
    "bin/{}{}".format(library_name, env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)

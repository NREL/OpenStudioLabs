import sys
from pathlib import Path

from conan import ConanFile
from conan.tools.apple import is_apple_os
from conan.tools.cmake import CMakeToolchain

required_conan_version = ">=2.0"


class OpenStudioBuildRecipe(ConanFile):
    name = "openstudio"
    description = "Build recipe for OpenStudio SDK which install necessary conan dependencies"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"  # CMakeToolchain explicitly instantiated

    options = {
        "with_ruby": [True, False],
        "with_python": [True, False],
    }
    default_options = {
        "with_ruby": True,
        "with_python": True,
    }

    def requirements(self):
        self.requires("fmt/9.1.0")  # 811e918ca4b4e0b9ddd6d5a2883efa82
        self.requires("swig/4.1.1")  # Pending https://github.com/conan-io/conan-center-index/pull/19058
        self.requires(
            "minizip/1.2.13"
        )  # 0b5296887a2558500d0323c6c94c8d02 # This depends on zlib, and basically patches it
        self.requires(
            "zlib/[>=1.2.11 <2]"
        )  # 3b9e037ae1c615d045a06c67d88491ae # Also needed, so we can find zlib.h and co (+ pinning exactly is good)

    def generate(self):
        tc = CMakeToolchain(self)

        tc.cache_variables["USE_RUBY_ENGINE"] = bool(self.options.with_ruby)
        tc.cache_variables["USE_PYTHON_ENGINE"] = bool(self.options.with_python)

        tc.cache_variables["CPACK_BINARY_TGZ"] = True
        tc.cache_variables["CPACK_BINARY_IFW"] = False
        tc.cache_variables["CPACK_BINARY_DEB"] = False
        if self.settings.build_type == "Release":
            if is_apple_os(self) or self.settings.os == "Windows":
                tc.cache_variables["CPACK_BINARY_IFW"] = True
            else:
                tc.cache_variables["CPACK_BINARY_DEB"] = True
        tc.cache_variables["CPACK_BINARY_NSIS"] = False
        tc.cache_variables["CPACK_BINARY_RPM"] = False
        tc.cache_variables["CPACK_BINARY_STGZ"] = False
        tc.cache_variables["CPACK_BINARY_TBZ2"] = False
        tc.cache_variables["CPACK_BINARY_TXZ"] = False
        tc.cache_variables["CPACK_BINARY_TZ"] = False

        if self.options.with_python:
            v = sys.version_info
            if (v.major, v.minor) == (3, 12):
                python_version = f"{v.major}.{v.minor}.{v.micro}"
                self.output.info(
                    f"Setting PYTHON_VERSION and Python_ROOT_DIR from your current python: {python_version}, '{sys.base_prefix}'"
                )
                tc.cache_variables["PYTHON_VERSION"] = python_version
                tc.cache_variables["Python_ROOT_DIR"] = str(Path(sys.base_prefix))
            else:
                self.output.warning(
                    "Your current python is not in the 3.12.x range, which is what we target.\n"
                    "You'll need to pass it properly when configuring CMake\n"
                    "via -DPYTHON_VERSION:STRING='3.12.xx' and -DPython_ROOT_DIR:PATH='/path/to/python3.12/'"
                )
        tc.generate()

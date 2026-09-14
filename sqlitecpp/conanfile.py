from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.cmake import CMakeDeps
from conan.tools.files import copy
import os

class SqliteCppConan(ConanFile):
    name = "sqlitecpp"

    # 版本号
    version = "3.3.0"

    license = "MIT"
    author = "Sebastien Rombauts"
    description = "C++ SQLite3 wrapper"
    topics = ("sqlite", "database", "sql")
    
    # 多体系架构
    settings = "os", "compiler", "build_type", "arch"
    
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
        "build_examples": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": False,
        "build_examples": False
    }
    
    # 导出所有文件 
    exports_sources = "*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    # ============ sqlitecpp 对 sqlite3 的版本依赖 ============
    def requirements(self):
        self.requires("sqlite3/3.31.1@baselib/test")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.set_property("sqlite3", "cmake_target_name", "SQLite::SQLite3")
        deps.generate()

        tc = CMakeToolchain(self)
        tc.variables["SQLITECPP_INTERNAL_SQLITE"] = "OFF"
        tc.variables["SQLITECPP_BUILD_TESTS"] = self.options.build_tests
        tc.variables["SQLITECPP_BUILD_EXAMPLES"] = self.options.build_examples
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        
        copy(self, "*.h", 
             src=os.path.join(self.source_folder, "include", "SQLiteCpp"), 
             dst=os.path.join(self.package_folder, "include", "SQLiteCpp"))

    def package_info(self):
        self.cpp_info.libs = ["SQLiteCpp"]
        if self.settings.os != "Windows":
            self.cpp_info.system_libs = ["pthread"]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
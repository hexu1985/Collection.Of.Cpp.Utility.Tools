from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get, rmdir, collect_libs
from conan.tools.build import check_min_cppstd
from conan.tools.scm import Version
import os

class SQLite3Conan(ConanFile):
    name = "sqlite3"
    version = "3.31.1"
    
    # 包描述信息
    description = "SQLite is a C-language library that implements a small, fast, self-contained, high-reliability, full-featured, SQL database engine"
    license = "Public Domain"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://www.sqlite.org/"
    topics = ("database", "sql", "sqlite", "embedded")
    
    # 设置和选项
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_shell": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_shell": False
    }
    
    # 导出源码
    exports_sources = "CMakeLists.txt", "src/*", "include/*"
    
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
    
    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
    
    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        tc = CMakeToolchain(self)
        
        # 传递构建选项到CMake
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.variables["BUILD_SQLITE3_SHELL"] = self.options.build_shell
        
        tc.generate()
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
        
        # 删除CMake配置文件中的绝对路径，保持可重定位
        # 但 install(EXPORT) 已经生成了相对路径，通常不需要额外处理
        
        # 收集库文件
        collect_libs(self)
    
    def package_info(self):
        # 设置库名称
        self.cpp_info.libs = ["sqlite3"]
        
        # 设置标准库依赖
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32", "advapi32"]
        elif self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs = ["dl", "pthread"]
        elif self.settings.os == "Macos":
            self.cpp_info.system_libs = ["pthread"]
        
        # 设置cmake查找路径
        self.cpp_info.set_property("cmake_find_mode", "config")
        self.cpp_info.set_property("cmake_config_package", "SQLite3")
        self.cpp_info.set_property("cmake_target_name", "SQLite::sqlite3")
        
        # 如果有shell可执行文件，添加到PATH
        if self.options.build_shell:
            bindir = os.path.join(self.package_folder, "bin")
            self.env_info.PATH.append(bindir)


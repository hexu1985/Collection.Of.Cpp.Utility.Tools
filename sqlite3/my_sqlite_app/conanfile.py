from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.build import can_run
import os

class MySqliteAppConan(ConanFile):
    name = "my_sqlite_app"
    version = "1.0.0"
    
    # 依赖配置
    requires = "sqlite3/3.31.1@baselib/test"
    
    # 为依赖项设置选项
    default_options = {
        "sqlite3/*:shared": True,  # 关键：要求 sqlite3 也是 shared
        # 如果你的项目也需要 shared，可以在这里设置
        # "shared": True,
    }

    # 设置
    settings = "os", "compiler", "build_type", "arch"
    
    # 生成器
    generators = "CMakeDeps", "CMakeToolchain"
    
    # 导出源码
    exports_sources = "CMakeLists.txt", "src/*"
    
    def layout(self):
        cmake_layout(self)
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "my_sqlite_app")
            self.run(cmd, env="conanrun")

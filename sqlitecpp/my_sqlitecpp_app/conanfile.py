from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.build import can_run
import os

class MySqliteCppAppConan(ConanFile):
    name = "my_sqlitecpp_app"
    version = "1.0.0"

    # 依赖 sqlitecpp 包（它内部依赖 sqlite3）
    requires = "sqlitecpp/3.3.0@baselib/test"

    # 为依赖项设置选项：要求 sqlitecpp 与 sqlite3 都构建为共享库
    default_options = {
        "sqlitecpp/*:shared": True,
        "sqlitecpp/*:internal_sqlite": False,
        "sqlitecpp/*:build_examples": False,
        "sqlitecpp/*:build_tests": False,
        "sqlite3/*:shared": True,
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
            cmd = os.path.join(self.cpp.build.bindir, "my_sqlitecpp_app")
            self.run(cmd, env="conanrun")

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, rmdir
from conan.tools.build import check_min_cppstd
from conan.errors import ConanInvalidConfiguration
import os

class SQLiteCppConan(ConanFile):
    name = "sqlitecpp"
    version = "3.3.0"

    # 基础信息
    description = "SQLiteCpp is a smart and easy to use C++ SQLite3 wrapper"
    license = "MIT"
    url = "https://github.com/SRombauts/SQLiteCpp"
    homepage = "https://srombauts.github.io/SQLiteCpp/"
    topics = ("sqlite", "database", "sql", "wrapper")

    # 设置与选项
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "internal_sqlite": [True, False],
        "build_examples": [True, False],
        "build_tests": [True, False],
        "use_asan": [True, False],
        "use_gcov": [True, False],
    }
    default_options = {
        "shared": True,           # 对应 -DBUILD_SHARED_LIBS=ON
        "fPIC": True,
        "internal_sqlite": False, # 对应 -DSQLITECPP_INTERNAL_SQLITE=OFF
        "build_examples": False,  # 对应 -DSQLITECPP_BUILD_EXAMPLES=OFF
        "build_tests": False,     # 对应 -DSQLITECPP_BUILD_TESTS=OFF
        "use_asan": False,        # 对应 -DSQLITECPP_USE_ASAN=OFF
        "use_gcov": False,        # 对应 -DSQLITECPP_USE_GCOV=OFF
        # 为依赖项 sqlite3 设置选项，要求其构建为共享库
        "sqlite3/*:shared": True,
    }

    # 依赖配置
    requires = "sqlite3/3.31.1@baselib/test"

    # 导出源码
    exports_sources = (
            "CMakeLists.txt",
            "src/*",
            "include/*",
            "cmake/*",
            "package.xml",
            "README.md",
            "LICENSE.txt",
            "CHANGELOG.md",
            "TODO.txt",
            )

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        # SQLiteCpp 需要 C++11 或更高
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 11)
        # 如果使用内部 sqlite，则不需要外部 sqlite3 依赖
        # 但这里通过 requires 硬性依赖，所以 internal_sqlite 必须为 False
        if self.options.internal_sqlite:
            raise ConanInvalidConfiguration(
                "This recipe always depends on external sqlite3. "
                "Set internal_sqlite=False."
            )

    def validate(self):
        # SQLiteCpp 在 Windows 上不支持构建为共享库
        if self.settings.os == "Windows" and self.options.shared:
            raise ConanInvalidConfiguration("SQLiteCpp can not be built as shared lib on Windows")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)

        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.variables["SQLITECPP_INTERNAL_SQLITE"] = self.options.internal_sqlite
        tc.variables["SQLITECPP_BUILD_EXAMPLES"] = self.options.build_examples
        tc.variables["SQLITECPP_BUILD_TESTS"] = self.options.build_tests
        tc.variables["SQLITECPP_USE_ASAN"] = self.options.use_asan
        tc.variables["SQLITECPP_USE_GCOV"] = self.options.use_gcov

        # 关键：关闭 config/script 文件包含，避免引用未导出的文件
        tc.variables["SQLITECPP_INCLUDE_SCRIPT"] = False

        # 关闭 cpplint/cppcheck/doxygen，它们不是构建必需
        tc.variables["SQLITECPP_RUN_CPPLINT"] = False
        tc.variables["SQLITECPP_RUN_CPPCHECK"] = False
        tc.variables["SQLITECPP_RUN_DOXYGEN"] = False

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        # 复制 License
        copy(self, "LICENSE.txt", src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))

        # 清理不必要的 CMake 配置文件
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "SQLiteCpp")
        self.cpp_info.set_property("cmake_target_name", "SQLiteCpp")
        self.cpp_info.libs = ["SQLiteCpp"]

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs = ["pthread", "dl"]
        elif self.settings.os == "Macos":
            self.cpp_info.system_libs = ["pthread"]

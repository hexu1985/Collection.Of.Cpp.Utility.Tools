
# conan new cmake_lib -d name=hello -d version=1.0

conan create .

# conan list hello
# conan create . -s build_type=Debug
# conan create . -o hello/1.0:shared=True
# conan list "hello/1.0:*"

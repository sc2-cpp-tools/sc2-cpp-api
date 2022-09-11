sources := $(shell find src test -iname *.cpp)
headers := $(shell find include -iname *.hpp)

CMAKE_DIRS = src third_party cmake
cmakefiles := $(shell find $(CMAKE_DIRS) -iname *.cmake -o -name CMakeLists.txt)

# Clang-format configuration
CLANG_FORMAT = clang-format
CLANG_FORMAT_COMMON = --style=file --verbose
CLANG_FORMAT_LINT := $(CLANG_FORMAT_COMMON) --Werror --dry-run
CLANG_FORMAT_FIX := $(CLANG_FORMAT_COMMON) -i

# Cmake-tools configuration
CMAKE_LINT = cmake-lint
CMAKE_FORMAT = cmake-format
CMAKE_FORMAT_LINT = --check
CMAKE_FORMAT_FIX = -i

# Use cmake-format --check instead of cmake-lint as there is no means of
# disabling errors for exceeding line length limit. However, given
# cmake-format --check is a binary response of whether or not a change would
# take place, use cmake-lint as a separate command to drill down for
# specific issues
lint: $(sources) $(headers) $(cmakefiles)
	$(CLANG_FORMAT) $(CLANG_FORMAT_LINT) $(sources) $(headers) && \
	$(CMAKE_FORMAT) $(CMAKE_FORMAT_LINT) CMakeLists.txt $(cmakefiles)

# Disregard linter warnings for lines enclosed in cmake-format: off/on
# cmake-lint does not support the annotations
cmake-lint: $(cmakefiles)
	$(CMAKE_LINT) CMakeLists.txt $(cmakefiles)

format: $(sources) $(headers) $(cmakefiles)
	$(CLANG_FORMAT) $(CLANG_FORMAT_FIX) $(sources) $(headers)  && \
	$(CMAKE_FORMAT) $(CMAKE_FORMAT_FIX) CMakeLists.txt $(cmakefiles)

clean:
	@sudo rm -rf build

all: build

configure: 
	@cmake -DCMAKE_BUILD_TYPE=Release \
	-DBUILD_TESTS=YES \
	-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
	-S . -B build

build: configure
	@cmake --build build -j

clean:
	@rm -rf build

rebuild: clean build

format:
	@find src include -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) -exec clang-format -i {} +

test: build
	@ctest --test-dir build

.PHONY: configure build clean rebuild format all test

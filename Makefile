.PHONY: build sync async test lcov del
build:
	mkdir build && cd build && mkdir sync &&  mkdir async
	cd build/sync && cmake ../.. -DSYNC=ON && make
	cd build/async && cmake ../.. -DSYNC=OFF && make
sync:
	make build
	./build/sync/iz2
async:
	make build
	./build/async/iz2
test:
	make build
	@echo "STATIC SYNC LIBRARY"
	./build/sync/tests/tests_getsum
	@echo "SHARED ASYNC LIBRARY"
	./build/async/tests/tests_getsum
lcov:
	make test
	@echo "STATIC SYNC LIBRARY COVERAGE"
	lcov -t "./build/sync/tests/tests_getsum" -o coverage.info -c -d ./build/sync
	genhtml -o ./build/sync/report coverage.info
	@echo "SHARED ASYNC LIBRARY COVERAGE"
	lcov -t "./build/async/tests/tests_getsum" -o coverage.info -c -d ./build/async
	genhtml -o ./build/sync/report coverage.info
del:
	rm -rf build


# You can change these variables if you want to use a different compiler or debugger
CC = clang
DBG = lldb

# We use .PHONY to tell make to always run these commands even if a file exists
# with the same name in this directory. For more information on .PHONY, see
# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: build-main
build-main: build-dir
	$(CC) -fopenmp -lm -Wall -O0 -g -o build/main src/main.c

build-mandel: build-dir
	$(CC) -fopenmp -Wall -O0 -g -o build/mandel src/mandel.c

build-linked: build-dir
	$(CC) -fopenmp -Wall -O0 -g -o build/linked src/linked.c

build-linked-sol: build-dir
	$(CC) -fopenmp -Wall -O0 -g -o build/linked_sol src/linked_solution.c

.PHONY: ex8
ex8: build-dir
	$(CC) -fopenmp -Wall -O0 -g -o build/ex8 src/prod_cons.c
	./build/ex8

.PHONY: ex9
ex9: build-dir
	$(CC) -fopenmp -Wall -O0 -g -o build/ex9 src/pi_mc.c src/random.c
	./build/ex9

compare-solution-ex6: build-linked build-linked-sol
	build/linked > linked.data
	build/linked_sol > linked_sol.data
	diff linked.data linked_sol.data || true

.PHONY: check
check:
	@which $(CC) > /dev/null && echo "SUCCESS: $(CC) is installed" || echo "ERROR: $(CC) not found, please install clang"
	@which $(DBG) > /dev/null && echo "SUCCESS: $(DBG) is installed" || echo "ERROR: $(DBG) not found, please install lldb"

.PHONY: build-dir
build-dir:
	if [ ! -d build ]; then mkdir build; fi

.PHONY: build-test
build-test: build-dir
	$(CC) -fopenmp -lm -Wall -O0 -g -o build/test src/test.c

.PHONY: run
run: build-main
	./build/main

.PHONY: test
test: build-test
	./build/test

.PHONY: debug
debug: build-main
	$(DBG) ./build/main

.PHONY: debug-test
debug-test: build-test
	$(DBG) ./build/test

.PHONY: perf-main
perf-main: build-main
	perf stat ./build/main

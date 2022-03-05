
.PHONY: all build test clean

all: build
	python setup.py build
	python setup.py install
	python tests/test.py

build:
	nvcc -lib --compiler-options '-fPIC' -O3 -o libs/libvectoradd.a cuda/vectorAdd.cu

test: build
	g++ tests/test.c -Llibs -lvectoradd -o tests/test -L${CUDA_PATH}/lib64 -lcudart

clean:
	rm -rf build/* libs/*

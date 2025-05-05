.PHONY: all

all: main

libhapticmorseextension.so:
	/usr/bin/clang++ -o liblibrary.so library.cpp -std=c++20 -O3 -shared
main: libhapticmorseextension.so

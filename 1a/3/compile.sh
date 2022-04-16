clang++ 2.cpp -o 0 \
	-Weverything -Wno-pre-c++17-compat -Wno-c++98-compat -Wno-exit-time-destructors -Wno-global-constructors \
	-O2 \
	--std=c++17

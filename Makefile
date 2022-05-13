build:
	mkdir bin || true
	g++ -O3 src/main.cpp -o bin/ipmt -I src/algorithms
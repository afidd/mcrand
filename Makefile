CXXFLAGS = -O2 -I include
LDFLAGS = -L/usr/local/cuda-5.5/lib64 -lcudart

PRANDLIB = prand/lib/libprand.a

a.out: a.o $(PRANDLIB)
	$(CXX) a.o $(PRANDLIB) $(LDFLAGS) -o a.out

a.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c -o a.o src/main.cpp


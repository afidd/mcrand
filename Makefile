CXXFLAGS = -O2 -I./include -fPIC
LDFLAGS = -fPIC
NVFLAGS = -L/usr/local/cuda-5.5/lib64 -lcudart -fPIC

PRANDLIB = prand/lib/libprand.a
RANDLIB = rngsselib/lib/librngsse.a

mttest: src/mt_test.cpp lib/libmcrand.so.1.0
	$(CXX) $(CXXFLAGS) -std=c++11 src/mt_test.cpp -L./lib -lmcrand $(LDFLAGS) -o mttest

lib/libmcrand.so.1.0: src/mt19937.o $(RANDLIB)
	-mkdir lib
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,libmcrand.so.1 -o lib/libmcrand.so.1.0 src/mt19937.o $(RANDLIB)
	-(cd lib; ln -s libmcrand.so.1.0 libmcrand.so)
	-(cd lib; ln -s libmcrand.so.1.0 libmcrand.so.1)

src/mt19937.o: src/mt19937.cpp include/pr_mt19937.h
	$(CXX) -std=c++11 $(CXXFLAGS) src/mt19937.cpp -c -o src/mt19937.o

a.out: a.o $(RANDLIB)
	$(CXX) a.o $(RANDLIB) $(LDFLAGS) -o a.out

limits: src/limits.cpp
	$(CXX) $(CXXFLAGS) -std=c++11 src/limits.cpp -o limits

a.o: src/main.cpp include/pr_mt19937.h
	$(CXX) $(CXXFLAGS) -c -o a.o src/main.cpp

# This line creates C-friendly versions of the headers that have CUDA tags.
include/pr_mt19937.h: prand/include/mt19937.h
	cd prand/include; \
	for i in *.h; \
	do \
	  cat $${i} | perl -pe "s/__host__//g;" -pe "s/__device__//g;" > ../../include/pr_$${i}; \
	done

clean:
	rm -f a.out a.o include/pr_*.h src/*.o lib/libmcrand*

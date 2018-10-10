CXX		=	g++
CXXFLAGS	=	-g -O3 -Wall -DDEBUG

all:		predict

predict:	predict.cc trace.cc debug.cc predictor.h branch.h trace.h my_predictor.h debug.h
		$(CXX) $(CXXFLAGS) -o predict predict.cc trace.cc debug.cc

clean:
		rm -f predict

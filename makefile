CXX = g++
CXXFLAGS = -Wall -g



redBlackTree: RedBlackTree.o main.o treeHelper.o
	$(CXX) $(CXXFLAGS) -o redBlackTree RedBlackTree.o main.o treeHelper.o

main.o: RedBlackTree.h treeHelper.h main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp


RedBlackTree.o: RedBlackTree.h RedBlackTree.cpp
	$(CXX) $(CXXFLAGS) -c RedBlackTree.cpp

treeHelper.o: RedBlackTree.h treeHelper.cpp 
	$(CXX) $(CXXFLAGS) -c treeHelper.cpp


clean:
	$(RM) *.o *~

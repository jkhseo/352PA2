CXX = g++
CXXFLAGS = -Wall -g



redBlackTree: redblacktree.o main.o treehelper.o
	$(CXX) $(CXXFLAGS) -o redBlackTree redblacktree.o main.o treehelper.o

main.o: RedBlackTree.h treeHelper.h main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp


redblacktree.o: RedBlackTree.h RedBlackTree.cpp
	$(CXX) $(CXXFLAGS) -c RedBlackTree.cpp

treehelper.o: RedBlackTree.h treeHelper.cpp 
	$(CXX) $(CXXFLAGS) -c treeHelper.cpp


clean:
	$(RM) *.o *~

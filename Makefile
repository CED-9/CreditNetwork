SYSTEM     = x86-64_sles10_4.1
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /usr/caen/cplex-12.6.2/cplex
CONCERTDIR    = /usr/caen/cplex-12.6.2/concert
# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++ -O0
CC  = gcc -O0
JAVAC = javac 

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD
COPT  = -m64 -fPIC -fno-strict-aliasing 
JOPT  = -classpath $(CPLEXDIR)/lib/cplex.jar -O

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread
CLNFLAGS  = -L$(CPLEXLIBDIR) -lcplex -lm -pthread
JAVA      = java -d64 -Djava.library.path=$(CPLEXDIR)/bin/x86-64_sles10_4.1 -classpath $(CPLEXJARDIR):

#------------------------------------------------------------
#
# My test
#
#------------------------------------------------------------
test: CN_Edge.cpp CN_Node.cpp
	g++ -g -c CN_Node.cpp -std=c++11
	g++ -g -c CN_Edge.cpp -std=c++11
	g++ -g -c CN_Graph.cpp -std=c++11
	g++ -g -c CN_WidgetGraph.cpp -std=c++11
	$(CCC) -c $(CCFLAGS) CN_CplexSolver.cpp -std=c++11
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o test test_edge.cpp *.o $(CCLNFLAGS) -std=c++11
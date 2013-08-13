# Universal makefile for multiple C++ programs and a Java program
#
# Use gcc flag -MMD (user) or -MD (user/system) to generate dependences among source files.
# Use gmake default rules for commonly used file-name suffixes and make variables names.
#
# % gmake (make)  [ searcher | searcherJava | phil ]

########## Variables ##########

CXX = g++					# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD -DDEBUG # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = main1.o DLT.o		# object files forming 1st executable
EXEC1 = DLT				# 1st executable name

OBJECTS2 = main2.o BN.o # object files forming 2nd executable
EXEC2 = BN					# 2nd executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}	# all executables

########## Targets ##########

.PHONY : all clean				# not file names

all : ${EXECS}					# build all executables

${EXEC1} : ${OBJECTS1}				# link step 1st executable
	${CXX} $^ -o $@				# additional object files before $^

${EXEC2} : ${OBJECTS2}				# link step 2nd executable
	${CXX} $^ -o $@				# additional object files before $^

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXECS}
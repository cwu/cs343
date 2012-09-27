CXX = u++								# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD				# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS0 = q2_orig.o	# optional build of given throwcatch program
EXEC0 = throwcatch		# 0th executable name

OBJECTS1 = q2_modified.o # object files forming 1st executable with prefix "q2"
EXEC1 = longjmp								# 1st executable name

OBJECTS2 = q3grammar.o # object files forming 2nd executable with prefix "q3"
EXEC2 = grammar								# 2nd executable name

OBJECTS = ${OBJECTS0} ${OBJECTS1} ${OBJECTS2}	# all object files
DEPENDS = ${OBJECTS:.o=.d}						# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}						# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}								# build all executables

q2%.o : q2%.cc								# change compiler 2nd executable
	g++ ${CXXFLAGS} -c $< -o $@

${EXEC0} : ${OBJECTS0}							# link step 0th executable
	g++ $^ -o $@

${EXEC1} : ${OBJECTS1}							# link step 1st executable
	g++ $^ -o $@

${EXEC2} : ${OBJECTS2}							# link step 2nd executable
	${CXX} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}						# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}							# include *.d files containing program dependences

clean :									# remove files that can be regenerated
	rm -f *.d *.o ${EXEC0} ${EXECS}

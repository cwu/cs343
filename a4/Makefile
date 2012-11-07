OPT:=

CXX = u++										# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD ${OPT} # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q1station.o
EXEC1 = #tokenring								# 1st executable name

OBJECTS2 = q2main.o q2santa.o q2elf.o q2reindeer.o q2workshop.o q2printer.o q2random.o
EXEC2 = northpole								# 2nd executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}						# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}						# all executables

#############################################################
.PHONY : all clean

all : ${EXECS}									# build all executables

${EXEC1} : ${OBJECTS1}							# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################
${OBJECTS} : ${MAKEFILE_NAME}					# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}								# include *.d files containing program dependences

clean :											# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ImplType

KIND:=NOBUSY
OPT:=

CXX = u++										# compiler
CXXFLAGS = -g -Wall -Wno-unused-label -MMD ${OPT} -D"${KIND}"
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q1main.o q1random.o q1producer.o q1consumer.o
EXEC1 = boundedBuffer

OBJECTS2 = q2main.o q2philosopher.o q2table.o q2printer.o q2random.o
EXEC2 = phil

OBJECTS = ${OBJECTS1} ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}						# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}						# all executables

.PHONY : all clean

all : ${EXECS}									# build all executables

#############################################################
-include ImplKind

ifeq (${IMPLKIND},${KIND})						# same implementation type as last time ?
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else
ifeq (${KIND},)									# no implementation type specified ?
# set type to previous type
KIND=${IMPLKIND}
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else											# implementation type has changed
.PHONY : ${EXEC1}
${EXEC1} :
	rm -f ImplKind
	touch q1buffer.h
	${MAKE} ${EXEC1} KIND=${KIND}
endif
endif

ImplKind :
	echo "IMPLKIND=${KIND}" > ImplKind

#############################################################
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################
${OBJECTS} : ${MAKEFILE_NAME}					# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}

clean :											# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} ImplKind ImplType

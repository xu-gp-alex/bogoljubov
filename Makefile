OBJECT_FILES = \
	board.o \
	data.o \
	main.o \
	cli.o \
#	debug.o \
# 	search.o \
# 	eval.o \

all: bogoljubov

# linking command, taking all the .o and making 
# final object file. -o to name it bogoljubov
bogoljubov: $(OBJECT_FILES)
	clang++ -O3 -o bogoljubov $(OBJECT_FILES)

# compiling command, which handles the includes
# -c because you need something to compile
# $< because we are trying to compile .cpp files only
# %.o: %.cpp board.hpp cli.hpp data.hpp def.hpp search.hpp eval.hpp
%.o: %.cpp protos.hpp data.hpp def.hpp cli.hpp #search.hpp eval.hpp
	clang++ -O3 -c $< -o $@

clean:
	rm -f *.o bogoljubov

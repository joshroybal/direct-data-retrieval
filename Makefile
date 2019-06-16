cc = clang++
cflags = -g
lflags = -lcgicc
program = bin/avlbase.cgi
objects = obj/bst.o \
          obj/avl.o \
			 obj/csv.o \
			 obj/main.o

$(program): $(objects)
	$(cc) $(objects) $(lflags) -o $(program)

obj/bst.o: src/bst.cpp src/bst.hpp
	$(cc) $(cflags) -c $< -o $@ 

obj/avl.o: src/avl.cpp src/avl.hpp src/bst.hpp
	$(cc) $(cflags) -c $< -o $@

obj/csv.o: src/csv.cpp src/csv.hpp
	$(cc) $(cflags) -c $< -o $@

obj/main.o: src/main.cpp src/avl.hpp src/csv.hpp
	$(cc) $(cflags) -c $< -o $@

.PHONY : clean
clean:
	rm $(program) $(objects)

install:
	sudo -u apache -g apache cp src/avlbase.html /srv/httpd/htdocs
	sudo -u apache -g apache cp src/avlbase.js /srv/httpd/htdocs
	sudo -u apache -g apache cp bin/avlbase.cgi /srv/httpd/cgi-bin

.PHONY: rebuild cone clean

rebuild: clean all 
all: cone clean

.DEFAULT_GOAL := rebuild

cone:
	cd src && make -f makefile.src
	
clean:
	cd src && make -f makefile.src clean

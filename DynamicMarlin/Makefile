SHELL=/bin/bash

BINARY=

all: check_param
	
start: files compile

compile: 

# Generate the C++ files using the perl scripts
files: 
	objdump -d $(BINARY) > code.S
	objdump -t $(BINARY) > symbols.S
	perl generator.pl code.S symbols.S

clean: 
	rm code.S
	rm symbols.S

check_param:
ifeq ($(BINARY),)
	@echo "You must define a binary to operate on!";
else
	@make start
endif

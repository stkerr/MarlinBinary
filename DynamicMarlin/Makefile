SHELL=/bin/bash

BINARY=

all: check_param
	
# Runs the different stages
start: files

# Executes the custom configured Marlin
execute:
	./CustomMarlinizer $(BINARY)

# Compile all the generated files along with the normal files togher
compile: 
	g++ -o CustomMarlinizer main.cpp patch.cpp

# Generate the C++ files using the perl scripts
files: 
	@objdump -d $(BINARY) > code.S		# disassembler
	@objdump -h $(BINARY) > symbols.S	# section headers
	@perl generator.pl code.S symbols.S $(BINARY)	# generator scripts

clean: 
	rm code.S
	rm symbols.S
	rm patch.cpp
	rm symbols.cpp
	rm main.cpp
	rm CustomMarlinizer

# Validates that there is a binary specified
check_param:
ifeq ($(BINARY),)
	@echo "You must define a binary to operate on!";
else
	@make start
endif

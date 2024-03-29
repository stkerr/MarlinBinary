#ifndef LIBC_JUMP_PATCH
#define LIBC_JUMP_PATCH

#include "text_symbols.h"

#include <list>
#include <map>
#include <string>

bool perform_patch(unsigned char *buffer, text_symbol symbol, int new_location);
bool lowlevel_patch(unsigned char *buffer, int offset, int dest);



typedef struct patch_struct {
	unsigned int library_offset; /* The offset of the function in the original library */

	char function_name[255]; /* The function containing the instruction to patch */
	unsigned int offset; /* The offset into the given function where the patch goes */	
	unsigned int patch_length; /* The number of bytes to patch */

	char dest_function_name[255]; /* The name of the destination function that the jump points to (optional) */
        unsigned int dest_function; /* The address of the destination function */
	unsigned int dest_offset; /* Any additional offset into the destination function that might be needed */
	bool in_plt; /* If this bit is set, the function_name will be used to find a PLT entry, rather than the body */
} patch;

class jumppatching
{
public:

	/*
		This is a list of <int, string> pairs. Each pair represents a 'call' instruction.
		The int is the offset into the function that the call occurs and the string is
		the name of the function that is being called. Since call instructions are a 4 byte
		offset, it will be possible to calculate the address these functions need to be
		remapped to
	*/
	std::list< patch > function_call_patches;

	/*
		Similar to the previous, but this is for 4 byte, relative jumps (E9 opcode)
		The int is the offset into the function to patch and the string is the function
		name destination
	*/
	std::list< patch > relative_jump_patches;
};

extern std::map<std::string, std::list < jumppatching*>* > *patch_database; // <symbol name, list of patches>

std::map<std::string, std::list<jumppatching*>* >* prepare_patch_database();
#endif
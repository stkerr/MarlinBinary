our $patch_start = "#include \"jumppatching.h\"
#include <iostream>
#include <map>
#include <string>

using namespace std;

map<string, list<jumppatching*>* > *patch_database; // <symbol name, list of patches>
std::map<std::string, int> current_addresses;
bool lowlevel_patch(unsigned char *buffer, int offset, int dest)
{
	cout << hex << \"Patching: 0x\" << offset << \" to 0x\" << dest << endl;
	cout << hex << \"\t\" << FILE_TO_TEXT(offset) << \" to 0x\" << FILE_TO_TEXT(dest) << endl;
	int distance_to_jump = dest-offset-4; // 4 bytes for the instruction
	*((int*)(buffer+offset)) = distance_to_jump;
	cout << hex << \"Patched jump: 0x\" << distance_to_jump << endl;
	return true;
}
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define START() patches = new list<jumppatching*>;
#define GEN_PATCH(func,dest,off,d_off,count)\
patch func ## _patch_ ## count;\
strcpy(func ## _patch_ ## count.function_name,STR(func));\
strcpy(func ## _patch_ ## count.dest_function_name, STR(dest));\
func ## _patch_ ## count.offset = off;\
func ## _patch_ ## count.dest_offset = d_off ;

#define GEN_JUMP(func,count) \
jumppatching* jump_ ## func ## _ ## count = new jumppatching(); \
jump_ ## func ## _ ## count ->function_call_patches.push_front( func ## _patch_ ## count);

#define GEN_INSERT(func,count) \
patches->push_back( jump_ ## func ## _ ## count  ); \
patch_db->insert(pair<string, list<jumppatching*>* > ( STR(func) , patches));

#define MASTER(func,dest,off,d_off,count) \
START() \
ADDITION(func,dest,off,d_off,count)

#define ADDITION(func,dest,off,d_off,count) \
GEN_PATCH( func , dest , off, d_off , count) \
GEN_JUMP( func, count) \
GEN_INSERT (  func , count)

map<string, list<jumppatching*>* >* prepare_patch_database()
{
list<jumppatching*> *patches;
map<string, list<jumppatching*>* >* patch_db = new map<string, list<jumppatching*>* >;
";

our $patching_end = "
    patch_database = patch_db;
        return patch_db;
	}
";








our $symbols_start = "
#ifndef LINUX_TEXT_SYMBOLS_H
#define LINUX_TEXT_SYMBOLS_H

#define TEXT_START (";



our $symbols_mid = "
#define TEXT_TO_FILE(addr) (addr - TEXT_START + FILE_START)
#define FILE_TO_TEXT(addr) (addr - FILE_START + TEXT_START)

#include <map>

enum jump_patch_enum {
    NOPATCHNEEDED = 0x0,
    NOTDETERMINED = 0x1,
    PATCHNEEDED = 0x10,
};

typedef struct symbol {
    unsigned int length;
    unsigned long address;
    char symbolName[512];
    int jumppatching;
} text_symbol;

typedef struct text_record_struct {
    text_symbol symbol;
    unsigned long old_address;
} text_record;

bool text_symbol_compare(text_symbol first, text_symbol second);


extern std::map<std::string, int> current_addresses; // <function name, current address>
void initialize_current_addresses();

namespace text_symbols {
	";

our $symbols_end = "
}
	

#endif
";
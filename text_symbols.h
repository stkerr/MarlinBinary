#ifndef LINUX_TEXT_SYMBOLS_H
#define LINUX_TEXT_SYMBOLS_H

typedef struct temp_struct
{
    int length;
    unsigned int address;
    char name[100];
    int patch_needed;
} entry;

// define the offset between virtual .text addresses and file offsets
//#define TEXT_OFFSET (0x080483f0) 
        //-0x00000380)

// the virtual memory address of the .text section
//#define TEXT_START (0x080483f0)

// the offset of the .text section in the file
//#define FILE_START (0x3f0)

#define TEXT_TO_FILE(addr) (addr - TEXT_START + FILE_START)
#define FILE_TO_TEXT(addr) (addr - FILE_START + TEXT_START)

#include <map>
#include <list>

enum jump_patch_enum {
    NOPATCHNEEDED = 0x0,
    DONOTADD = 0x1,
    NOTDETERMINED = 0x10,
    PATCHNEEDED = 0x100,
    THUNK_CX_NEEDED = 0x1000,
    THUNK_BX_NEEDED = 0x10000,
};

typedef struct symbol {
    unsigned int length;
    unsigned long address;
    char symbolName[512];
    int jumppatching;
} text_symbol;

extern text_symbol test_entry;
extern std::list<text_symbol> entry_list;

extern unsigned int TEXT_START;
extern unsigned int FILE_START;

typedef struct text_record_struct {
    text_symbol symbol;
    unsigned long old_address;
} text_record;

void read_patch_file(char* path);
bool text_symbol_compare(text_symbol first, text_symbol second);


extern std::map<std::string, int> current_addresses; // <function name, current address>
void initialize_current_addresses();
	

#endif
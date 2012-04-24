#ifndef LINUX_TEXT_SYMBOLS_H
#define LINUX_TEXT_SYMBOLS_H

// define the offset between virtual .text addresses and file offsets
#define TEXT_OFFSET (0x080483f0) 
        //-0x00000380)

// the virtual memory address of the .text section
#define TEXT_START (0x080483f0)

// the offset of the .text section in the file
#define FILE_START (0x3f0)

#define TEXT_TO_FILE(addr) (addr - TEXT_START + FILE_START)
#define FILE_TO_TEXT(addr) (addr - FILE_START + TEXT_START)

#include <map>

enum jump_patch_enum {
    NOPATCHNEEDED = 0x0,
    NOTDETERMINED = 0x1,
    PATCHNEEDED = 0x10,
    THUNK_CX_NEEDED = 0x100,
    THUNK_BX_NEEDED = 0x1000
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
    const text_symbol fun_function1 = { 0x1D, TEXT_TO_FILE(0x80484c4), "function1", NOPATCHNEEDED};
    const text_symbol fun_function2 = { 0x1D, TEXT_TO_FILE(0x80484e1), "function2", NOPATCHNEEDED};
    const text_symbol fun_function3 = { 0x1D, TEXT_TO_FILE(0x80484fe), "function3", NOPATCHNEEDED};
    const text_symbol fun_function4 = { 0x1D, TEXT_TO_FILE(0x804851b), "function4", NOPATCHNEEDED};
    const text_symbol fun_function5 = { 0x1D, TEXT_TO_FILE(0x8048538), "function5", NOPATCHNEEDED};
    const text_symbol fun_function6 = { 0x1D, TEXT_TO_FILE(0x8048555), "function6", NOPATCHNEEDED};
    const text_symbol fun_function7 = { 0x1D, TEXT_TO_FILE(0x8048572), "function7", NOPATCHNEEDED};
    const text_symbol fun_function8 = { 0x1D, TEXT_TO_FILE(0x804858f), "function8", NOPATCHNEEDED};
    const text_symbol fun_function9 = { 0x1D, TEXT_TO_FILE(0x80485ac), "function9", NOPATCHNEEDED};
    const text_symbol fun_function10 = { 0x1D, TEXT_TO_FILE(0x80485c9), "function10", NOPATCHNEEDED};

    const text_symbol fun_jumpfunction1 = { 0xA, TEXT_TO_FILE(0x80485e6), "jump_function1", PATCHNEEDED};
    const text_symbol fun_jumpfunction2 = { 0xA, TEXT_TO_FILE(0x80485f0), "jump_function2", PATCHNEEDED};
    const text_symbol fun_jumpfunction3 = { 0xA, TEXT_TO_FILE(0x80485fa), "jump_function3", PATCHNEEDED};
    const text_symbol fun_jumpfunction4 = { 0xA, TEXT_TO_FILE(0x8048604), "jump_function4", PATCHNEEDED};
    const text_symbol fun_jumpfunction5 = { 0xA, TEXT_TO_FILE(0x804860e), "jump_function5", PATCHNEEDED};
    const text_symbol fun_jumpfunction6 = { 0xA, TEXT_TO_FILE(0x8048618), "jump_function6", PATCHNEEDED};
    const text_symbol fun_jumpfunction7 = { 0xA, TEXT_TO_FILE(0x8048622), "jump_function7", PATCHNEEDED};
    const text_symbol fun_jumpfunction8 = { 0xA, TEXT_TO_FILE(0x804862c), "jump_function8", PATCHNEEDED};
    const text_symbol fun_jumpfunction9 = { 0xA, TEXT_TO_FILE(0x8048636), "jump_function9", PATCHNEEDED};
    const text_symbol fun_jumpfunction10 = { 0xA, TEXT_TO_FILE(0x8048640), "jump_function10", PATCHNEEDED};
    
    const text_symbol fun_main = { 0x108, TEXT_TO_FILE(0x804864a), "main", PATCHNEEDED};
}
	

#endif
#ifndef LINUX_TEXT_SYMBOLS_H
#define LINUX_TEXT_SYMBOLS_H

// define the offset between virtual .text addresses and file offsets
#define TEXT_OFFSET (0x08048380) 
        //-0x00000380)

#define TEXT_START (0x08048380)
#define FILE_START (0x380)

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
    const text_symbol fun_function1 = { 0x1D, TEXT_TO_FILE(0x8048454), "function1", NOPATCHNEEDED};
    const text_symbol fun_function2 = { 0x1D, TEXT_TO_FILE(0x8048471), "function2", NOPATCHNEEDED};
    const text_symbol fun_function3 = { 0x1D, TEXT_TO_FILE(0x804848e), "function3", NOPATCHNEEDED};
    const text_symbol fun_function4 = { 0x1D, TEXT_TO_FILE(0x80484ab), "function4", NOPATCHNEEDED};
    const text_symbol fun_function5 = { 0x1D, TEXT_TO_FILE(0x80484c8), "function5", NOPATCHNEEDED};
    const text_symbol fun_function6 = { 0x1D, TEXT_TO_FILE(0x80484e5), "function6", NOPATCHNEEDED};
    const text_symbol fun_function7 = { 0x1D, TEXT_TO_FILE(0x8048502), "function7", NOPATCHNEEDED};
    const text_symbol fun_function8 = { 0x1D, TEXT_TO_FILE(0x804851f), "function8", NOPATCHNEEDED};
    const text_symbol fun_function9 = { 0x1D, TEXT_TO_FILE(0x804853c), "function9", NOPATCHNEEDED};
    const text_symbol fun_function10 = { 0x1D, TEXT_TO_FILE(0x8048559), "function10", NOPATCHNEEDED};

    const text_symbol fun_jumpfunction1 = { 0xA, TEXT_TO_FILE(0x8048576), "jump_function1", PATCHNEEDED};
    const text_symbol fun_jumpfunction2 = { 0xA, TEXT_TO_FILE(0x8048580), "jump_function2", PATCHNEEDED};
    const text_symbol fun_jumpfunction3 = { 0xA, TEXT_TO_FILE(0x804858a), "jump_function3", PATCHNEEDED};
    const text_symbol fun_jumpfunction4 = { 0xA, TEXT_TO_FILE(0x8048594), "jump_function4", PATCHNEEDED};
    const text_symbol fun_jumpfunction5 = { 0xA, TEXT_TO_FILE(0x804859e), "jump_function5", PATCHNEEDED};
    const text_symbol fun_jumpfunction6 = { 0xA, TEXT_TO_FILE(0x80485a8), "jump_function6", PATCHNEEDED};
    const text_symbol fun_jumpfunction7 = { 0xA, TEXT_TO_FILE(0x80485b2), "jump_function7", PATCHNEEDED};
    const text_symbol fun_jumpfunction8 = { 0xA, TEXT_TO_FILE(0x80485bc), "jump_function8", PATCHNEEDED};
    const text_symbol fun_jumpfunction9 = { 0xA, TEXT_TO_FILE(0x80485c6), "jump_function9", PATCHNEEDED};
    const text_symbol fun_jumpfunction10 = { 0xA, TEXT_TO_FILE(0x80485d0), "jump_function10", PATCHNEEDED};
    
    const text_symbol fun_main = { 0x108, TEXT_TO_FILE(0x80485da), "main", PATCHNEEDED};
}
	

#endif
#ifndef LINUX_TEXT_SYMBOLS_H
#define LINUX_TEXT_SYMBOLS_H

// define the offset between virtual .text addresses and file offsets
#define TEXT_OFFSET (0x08048380-0x00000380)
#define TEXT_START (0x08048380)
#define FILE_START (0x380)

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
    const text_symbol fun_function1 = { 0x1D, 0x8048454 - TEXT_OFFSET, "function1", NOPATCHNEEDED};
    const text_symbol fun_function2 = { 0x1D, 0x8048471 - TEXT_OFFSET, "function2", NOPATCHNEEDED};
    const text_symbol fun_function3 = { 0x1D, 0x804848e - TEXT_OFFSET, "function3", NOPATCHNEEDED};
    const text_symbol fun_function4 = { 0x1D, 0x80484ab - TEXT_OFFSET, "function4", NOPATCHNEEDED};
    const text_symbol fun_function5 = { 0x1D, 0x80484c8 - TEXT_OFFSET, "function5", NOPATCHNEEDED};
    const text_symbol fun_function6 = { 0x1D, 0x80484e5 - TEXT_OFFSET, "function6", NOPATCHNEEDED};
    const text_symbol fun_function7 = { 0x1D, 0x8048502 - TEXT_OFFSET, "function7", NOPATCHNEEDED};
    const text_symbol fun_function8 = { 0x1D, 0x804851f - TEXT_OFFSET, "function8", NOPATCHNEEDED};
    const text_symbol fun_function9 = { 0x1D, 0x804853c - TEXT_OFFSET, "function9", NOPATCHNEEDED};
    const text_symbol fun_function10 = { 0x1D, 0x8048559 - TEXT_OFFSET, "function10", NOPATCHNEEDED};

    const text_symbol fun_jumpfunction1 = { 0xA, 0x8048576 - TEXT_OFFSET, "jump_function1", PATCHNEEDED};
    const text_symbol fun_jumpfunction2 = { 0xA, 0x8048580 - TEXT_OFFSET, "jump_function2", PATCHNEEDED};
    const text_symbol fun_jumpfunction3 = { 0xA, 0x804858a - TEXT_OFFSET, "jump_function3", PATCHNEEDED};
    const text_symbol fun_jumpfunction4 = { 0xA, 0x8048594 - TEXT_OFFSET, "jump_function4", PATCHNEEDED};
    const text_symbol fun_jumpfunction5 = { 0xA, 0x804859e - TEXT_OFFSET, "jump_function5", PATCHNEEDED};
    const text_symbol fun_jumpfunction6 = { 0xA, 0x80485a8 - TEXT_OFFSET, "jump_function6", PATCHNEEDED};
    const text_symbol fun_jumpfunction7 = { 0xA, 0x80485b2 - TEXT_OFFSET, "jump_function7", PATCHNEEDED};
    const text_symbol fun_jumpfunction8 = { 0xA, 0x80485bc - TEXT_OFFSET, "jump_function8", PATCHNEEDED};
    const text_symbol fun_jumpfunction9 = { 0xA, 0x80485c6 - TEXT_OFFSET, "jump_function9", PATCHNEEDED};
    const text_symbol fun_jumpfunction10 = { 0xA, 0x80485d0 - TEXT_OFFSET, "jump_function10", PATCHNEEDED};
    
    const text_symbol fun_main = { 0x108, 0x80485da - TEXT_OFFSET, "main", PATCHNEEDED};
}
	

#endif
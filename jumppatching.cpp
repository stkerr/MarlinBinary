#include "jumppatching.h"
#include <iostream>
#include <map>
#include <string>
#include <fstream>

using namespace std;

//const unsigned int jumppatching::thunk_bx_offset = 0x0001ec9b; /* Hardcoded for libc */
//const unsigned int jumppatching::thunk_cx_offset = 0x0001fc6c; /* Hardcoded for libc */

map<string, list<jumppatching*>* > *patch_database; // <symbol name, list of patches>

std::map<std::string, int> current_addresses;

typedef struct patch_entry_struct {
    text_symbol source;
    text_symbol dest;
    int offset;
    int d_offset;
} patch_entry;

text_symbol lookup_symbol_by_name(char* name)
{
    list<text_symbol>::iterator it;
    for(it = entry_list.begin(); it != entry_list.end(); it++)
    {
        if(strcmp(it->symbolName, name) == 0)
        {
            return *it;
        }
    }
}

list<patch_entry> patch_entry_list;

/**
 * Reads the file line by line & fills out a list of jump patches
 * Format:
 *     <source function> <dest function> <offset> <dest offset>
 * @param path
 * @return 
 */
void read_patch_file(char* path)
{
    fstream file;
    file.open(path, fstream::in);
    
    while(!file.eof())
    {
        char buffer[250];
        memset(buffer, 0, 250);
        file.getline(buffer, 250);
    
        if(strlen(buffer) == 0)
            break;
        
        char* toks = strtok(buffer, " ");
        
        text_symbol source = lookup_symbol_by_name(toks);
        
        toks = strtok(NULL, " ");
        
        text_symbol dest = lookup_symbol_by_name(toks);
        
        toks = strtok(NULL, " ");
        
        int offset = strtoul (toks,NULL,0);
        
        toks = strtok(NULL, " ");
        
        int d_offset = strtoul (toks,NULL,0);
        
        patch_entry temp = { source, dest, offset, d_offset };
        
        patch_entry_list.push_back(temp);
    }
    
    file.close();        
}


/* Calculates and updates the jump distance for teh given offset and destination address */
bool lowlevel_patch(unsigned char *buffer, int offset, int dest)
{
    if(offset > 0x1000000)
    {
//        cout << "Flipping offset of patch!" << endl;
        offset = TEXT_TO_FILE(offset);
    }
    
    if(dest > 0x1000000)
    {
//        cout << "Flipping dest of patch!" << endl;
        dest = TEXT_TO_FILE(offset);
    }
//	cout << hex << "Patching: 0x" << (offset) << " to 0x" << (dest) << endl;
//        cout << hex << "\t" << FILE_TO_TEXT(offset) << " to 0x" << FILE_TO_TEXT(dest) << endl;
        
	int distance_to_jump = dest-offset-4; // 4 bytes for the instruction

	*((int*)(buffer+offset)) = distance_to_jump;

//	cout << hex << "Patched jump: 0x" << distance_to_jump << endl;

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

/*
 * Iterates through every symbol and prepares the list of patches
 * needed for each symbol
 */
map<string, list<jumppatching*>* >* prepare_patch_database()
{
    
    list<jumppatching*> *patches;
    map<string, list<jumppatching*>* >* patch_db = new map<string, list<jumppatching*>* >;

    list<patch_entry>::iterator it;
    
    for(it = patch_entry_list.begin(); it != patch_entry_list.end(); it++)
    {
        if(patch_db->find(it->source.symbolName) != patch_db->end())
        {
            patches = (patch_db->find(it->source.symbolName))->second;
        }
        else
        {
            patches = new list<jumppatching*>;
        }
        
        
        patch temp_patch;
        strcpy(temp_patch.function_name, it->source.symbolName);
        strcpy(temp_patch.dest_function_name, it->dest.symbolName);
        temp_patch.offset = it->offset;
        temp_patch.dest_offset = it->d_offset;
        
        jumppatching* jump = new jumppatching();
        jump->function_call_patches.push_front(temp_patch);
        
        patches->push_back(jump);
        patch_db->insert(pair<string, list<jumppatching*>* > ( it->source.symbolName, patches));
    }
    
    /*
    patch jump_function1_patch;
    strcpy(jump_function1_patch.function_name, "jump_function1");
    strcpy(jump_function1_patch.dest_function_name, "function1");
    jump_function1_patch.offset = 4;
    jump_function1_patch.dest_offset = 0;
    */

/*
    jumppatching* jf1_patch = new jumppatching();
    jf1_patch->function_call_patches.push_front(jump_function1_patch);

    jumppatching* jf2_patch = new jumppatching();
    jf2_patch->function_call_patches.push_front(jump_function2_patch);
*/
     /*       
    patches->push_back(jf1_patch);
    patches->push_back(jf2_patch);
    
    patch_db->insert(pair<string, list<jumppatching*>* >("jump_function1", patches));
    patch_db->insert(pair<string, list<jumppatching*>* >("jump_function2", patches));
	*/


    patch_database = patch_db;
    return patch_db;
}

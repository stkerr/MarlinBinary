#include "jumppatching.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

//const unsigned int jumppatching::thunk_bx_offset = 0x0001ec9b; /* Hardcoded for libc */
//const unsigned int jumppatching::thunk_cx_offset = 0x0001fc6c; /* Hardcoded for libc */

map<string, list<jumppatching*>* > *patch_database; // <symbol name, list of patches>

std::map<std::string, int> current_addresses;



/* Calculates and updates the jump distance for teh given offset and destination address */
bool lowlevel_patch(unsigned char *buffer, int offset, int dest)
{
	cout << hex << "Patching: 0x" << offset << " to 0x" << dest << endl;

	int distance_to_jump = dest-offset-4; // 4 bytes for the instruction

	*((int*)(buffer+offset)) = distance_to_jump;

	cout << hex << "Patched jump: 0x" << distance_to_jump << endl;

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

	MASTER(jump_function1, function1, 4, 0, 0)
	MASTER(jump_function2, function2, 4, 0, 0)
	MASTER(jump_function3, function3, 4, 0, 0)
	MASTER(jump_function4, function4, 4, 0, 0)
	MASTER(jump_function5, function5, 4, 0, 0)
	MASTER(jump_function6, function6, 4, 0, 0)
	MASTER(jump_function7, function7, 4, 0, 0)
	MASTER(jump_function8, function8, 4, 0, 0)
	MASTER(jump_function9, function9, 4, 0, 0)
	MASTER(jump_function10, function10, 4, 0, 0)
/*
        MASTER(main, function1, 11, 0, 0)
        MASTER(main, function2, 16, 0, 1)
        MASTER(main, function3, 21, 0, 2)
        MASTER(main, function4, 26, 0, 3)
        MASTER(main, function5, 31, 0, 4)
        MASTER(main, function6, 36, 0, 5)
        MASTER(main, function7, 41, 0, 6)        
        MASTER(main, function8, 46, 0, 7)
        MASTER(main, function9, 51, 0, 8)
        MASTER(main, function10, 56, 0, 9)
       

        MASTER(main, jump_function1, 61, 0, 11)
        MASTER(main, jump_function2, 70, 0, 12)
        MASTER(main, jump_function3, 79, 0, 13)
        MASTER(main, jump_function4, 88, 0, 14)
        MASTER(main, jump_function5, 97, 0, 15)
        MASTER(main, jump_function6, 106, 0, 16)
        MASTER(main, jump_function7, 115, 0, 17)        
        MASTER(main, jump_function8, 124, 0, 18)
        MASTER(main, jump_function9, 133, 0, 19)
 
        MASTER(main, jump_function10, 142, 0, 10)
          */     
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

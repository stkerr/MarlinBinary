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
        cout << hex << "\t" << FILE_TO_TEXT(offset) << " to 0x" << FILE_TO_TEXT(dest) << endl;
        
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
                
        //MASTER(main, function1, 10, 0, 0)

                
        MASTER(main, function1, 10, 0, 0)
        ADDITION(main, function2, 15, 0, 1)
        ADDITION(main, function3, 20, 0, 2)
        ADDITION(main, function4, 25, 0, 3)
        ADDITION(main, function5, 30, 0, 4)
        ADDITION(main, function6, 35, 0, 5)
        ADDITION(main, function7, 40, 0, 6)        
        ADDITION(main, function8, 45, 0, 7)
        ADDITION(main, function9, 50, 0, 8)
        ADDITION(main, function10, 55, 0, 9)
       

        ADDITION(main, jump_function1, 60, 0, 11)
        ADDITION(main, jump_function2, 69, 0, 12)
        ADDITION(main, jump_function3, 78, 0, 13)
        ADDITION(main, jump_function4, 87, 0, 14)
        ADDITION(main, jump_function5, 96, 0, 15)
        ADDITION(main, jump_function6, 105, 0, 16)
        ADDITION(main, jump_function7, 114, 0, 17)        
        ADDITION(main, jump_function8, 123, 0, 18)
        ADDITION(main, jump_function9, 132, 0, 19)
        ADDITION(main, jump_function10, 141, 0, 10)
        
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

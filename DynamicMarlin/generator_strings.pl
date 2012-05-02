our $patch_start = "#include \"jumppatching.h\"
#include <iostream>
#include <map>
#include <string>

using namespace std;

map<string, list<jumppatching*>* > *patch_database; // <symbol name, list of patches>
std::map<std::string, int> current_addresses;
bool lowlevel_patch(unsigned char *buffer, int offset, int dest)
{
    //cout << hex << \"Patching: 0x\" << offset << \" to 0x\" << dest << endl;
    //cout << hex << \"\t\" << FILE_TO_TEXT(offset) << \" to 0x\" << FILE_TO_TEXT(dest) << endl;
    int distance_to_jump = dest-offset-4; // 4 bytes for the instruction
    *((int*)(buffer+offset)) = distance_to_jump;
   // cout << hex << \"Patched jump: 0x\" << distance_to_jump << endl;
    return true;
}
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define START() patches = new list<jumppatching*>;
#define GEN_PATCH(func,dest,off,d_off,count)\\
patch func ## _patch_ ## count;\\
strcpy(func ## _patch_ ## count.function_name,STR(func));\\
strcpy(func ## _patch_ ## count.dest_function_name, STR(dest));\\
func ## _patch_ ## count.offset = off;\\
func ## _patch_ ## count.dest_offset = d_off ;

#define GEN_JUMP(func,count) \\
jumppatching* jump_ ## func ## _ ## count = new jumppatching(); \\
jump_ ## func ## _ ## count ->function_call_patches.push_front( func ## _patch_ ## count);

#define GEN_INSERT(func,count) \\
patches->push_back( jump_ ## func ## _ ## count  ); \\
patch_db->insert(pair<string, list<jumppatching*>* > ( STR(func) , patches));

#define MASTER(func,dest,off,d_off,count) \\
START() \\
ADDITION(func,dest,off,d_off,count)

#define ADDITION(func,dest,off,d_off,count) \\
GEN_PATCH( func , dest , off, d_off , count) \\
GEN_JUMP( func, count) \\
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

our $main_start = "
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <map>
#include <string.h>
#include <vector>

#include \"jumppatching.h\"
#include \"text_symbols.h\"



using namespace std;

vector<pair<string, pair< unsigned int, unsigned int> > > function_addresses; // function name, <start address, length>
map < string, unsigned int > current_address_map; // function name, current address

unsigned char * read_file(char* path, unsigned long* length)
{
    FILE* fd;

    /* Open the file */
    fd = fopen(path, \"r\");

    if (fd == NULL)
    {
        printf(\"Could not open libc file: %s\\n\", path);
        return 0x0;
    }

    /* Seek to the end and record the length */
    fseek(fd, 0L, SEEK_END);
    *length = ftell(fd);

    /* Return to the beginning */
    fseek(fd, 0L, SEEK_SET);

    /* Allocate a buffer of the appropriate length */
    unsigned char * buffer = (unsigned char *) malloc(sizeof (unsigned char) * *length);

    /* Copy the file into the buffer */
    fread(buffer, *length, 1, fd);

    /* Close the file */
    fclose(fd);

    /* Return the buffer */
    return buffer;
}

void write_file(const char* path, unsigned char* buffer, unsigned int length)
{
    FILE* fd;

    fd = fopen(path, \"w\");

    if (fd == NULL)
    {
        printf(\"Couldn't open file for writing out!\");
        return;
    }

    fwrite(buffer, length, 1, fd);

    fclose(fd);
}

/*
 * 
 */
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << \"Usage: \" << argv[0] << \" filename\" << endl;
        return -1;
    }

    // get the file as a buffer
    unsigned long length;
    unsigned char* buffer = read_file(argv[1], &length);

    // mark where we are starting our randomization
    int current_address = 0;

    // initialize the initial function addresses
";

our $main_mid = "
/***
     * Shuffling stage
     ***/

    // seed the random number generator
    srand(time(NULL));

    // use another buffer temporarily
    unsigned char *tempbuffer = (unsigned char*) malloc(sizeof (unsigned char) * (end - start));

    // make an iterator
    vector<pair<string, pair< unsigned int, unsigned int> > >::iterator it;

    // while we have more blocks, keep going
    while (function_addresses.empty() == false)
    {
        // random number
        int block = rand() % function_addresses.size();

        // set the iterator
        it = function_addresses.begin() + block;

        // grab that block
        pair<string, pair< unsigned int, unsigned int> > item = *it;

        // record the length and address
        unsigned int address = item.second.first;
        unsigned int function_length = item.second.second;

        // copy that block into the temporary buffer
        //cout << hex  << \"Copying : \" << item.first << \". File offset : \" << address << \" => \" << function_length << endl;
        memcpy(tempbuffer + current_address, buffer + address, function_length);

        // record where this function now is (as far as current address)
        current_address_map[item.first] = TEXT_TO_FILE(start + current_address);

        //cout << hex << \"Putting \" << item.first << \" at \" << current_address_map[item.first] << \" to \" << current_address_map[item.first] + function_length << endl;
                
        // increase the current address
        current_address += function_length;

        // remove the block from future consideration
        function_addresses.erase(it);
    }

    //cout << endl;

    // write the temp buffer out
    //write_file((string(argv[1]) + \".TEMPBUFFER\").c_str(), tempbuffer, end - start);

    // copy the temporary buffer back to the normal buffer
    //cout << hex << \"Temp buffer length: 0x\" << end - start << endl;
    //cout << hex << \"First function at file offset: 0x\" << TEXT_TO_FILE(start) << endl;

    memcpy(buffer + TEXT_TO_FILE(start), tempbuffer, end - start);

    /***
     * Jump Patching Stage
     ***/
    list<text_symbol> symbols;
    list<text_symbol>::iterator symbols_it;
";

our $main_end = "
prepare_patch_database();
    /*map<string, list<jumppatching*>* >::iterator patch_mit;
                for ( patch_mit=patch_database->begin() ; patch_mit != patch_database->end(); patch_mit++ )
                {
                        //cout << (*patch_mit).first;
                        list<jumppatching*>* t_patches = patch_database->find((*symbols_it).symbolName)->second;
                        //cout << \":\" << t_patches->size() << endl;
                }
                //cout << \"----\" << endl;
      */   

#ifndef NO_JUMP_PATCHING
   
    for (symbols_it = symbols.begin(); symbols_it != symbols.end(); symbols_it++)
    {
        //cout << hex << \"Processing \" << symbols_it->symbolName << \"() from 0x\" << symbols_it->address << \" length: 0x\" << symbols_it->length << endl;
        //cout << hex << \"\t\" << \"Currently at: \" << current_address_map[symbols_it->symbolName] << endl;
        // Check for if we need to patch jump instructions 
        if (symbols_it->jumppatching >= NOTDETERMINED)
        {

            if (patch_database->count((*symbols_it).symbolName) == 0)
            {
                //cout << \"No patches for symbol: \" << (*symbols_it).symbolName << \" in patch database\" << endl;
                continue;
            }
            else
            {
                //cout << \"Patches found for symbol \" << (*symbols_it).symbolName << endl;
            }

            bool patch_results = true;

            list<jumppatching*>* patches = patch_database->find((*symbols_it).symbolName)->second;

            //cout << \"Size:\" << patches->size() << endl;
            
                
            list<jumppatching*>::iterator patch_it;
            for (patch_it = patches->begin(); patch_it != patches->end(); patch_it++)
            {
                /* Grab the patches that refer to jumps to a function */
                list<patch> function_patches = (*patch_it)->function_call_patches;

                list<patch>::iterator function_it;
                for (function_it = function_patches.begin(); function_it != function_patches.end(); function_it++)
                {
                    //cout << \"Function call patch\" << endl;
                    //cout << function_it->function_name << \"->\" << function_it->dest_function_name << endl;
                    
                    //cout << hex << \"DEST: \" << current_address_map[function_it->dest_function_name] << \":\" << 
                    //        FILE_TO_TEXT(current_address_map[function_it->dest_function_name]) << endl;
                    
                    lowlevel_patch(buffer,
                            current_address_map[function_it->function_name] + function_it->offset,
                            current_address_map[function_it->dest_function_name] + function_it->dest_offset);
                }

            }


            //patch_results = perform_patch(buffer, *it, new_location);
            if (patch_results == false)
            {
                //cout << \"Jump patching failed for randomized symbol :\" << symbols_it->symbolName << endl;
                //cout << endl;
                continue;
            }
            else
            {
                //cout << \"Jump patching succeeded for symbol: \" << symbols_it->symbolName << endl;
            }
        }
        else if (symbols_it->jumppatching == NOTDETERMINED)
        {
            //cout << \"Could not determine if jump patching needed. Might experience errors in symbol:\" << symbols_it->symbolName << endl;
            //cout << endl;
            continue;
        }
        else
        {
            //cout << \"Jump patching not needed for symbol: \" << symbols_it->symbolName << endl;
        }
        
        //cout << endl;
    }

#endif



    // write out the file
    write_file((string(argv[1]) + \".MARLIN\").c_str(), buffer, length);

    return 0;
}

";

our $jumppatching_whole;
$jumppatching_whole = "
#ifndef LIBC_JUMP_PATCH
#define LIBC_JUMP_PATCH

#include \"text_symbols.h\"

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
";

1;
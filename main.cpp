/* 
 * File:   main.cpp
 * Author: stkerr
 *
 * Created on April 6, 2012, 2:35 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <map>
#include <string.h>
#include <vector>

#include "jumppatching.h"
#include "text_symbols.h"



using namespace std;

vector<pair<string, pair< unsigned int, unsigned int> > > function_addresses; // function name, <start address, length>
map < string, unsigned int > current_address_map; // function name, current address

unsigned char * read_file(char* path, unsigned long* length)
{
    FILE* fd;

    /* Open the file */
    fd = fopen(path, "r");

    if (fd == NULL)
    {
        printf("Could not open libc file: %s\n", path);
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

    fd = fopen(path, "w");

    if (fd == NULL)
    {
        printf("Couldn't open file for writing out!");
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
        cout << "Usage: " << argv[0] << " filename" << endl;
        return -1;
    }

    // get the file as a buffer
    unsigned long length;
    unsigned char* buffer = read_file(argv[1], &length);

    // mark the start and end locations for the randomization
    unsigned int start = 0x8048454;
    unsigned int end = start + 10 * 0x1D + 10 * 0xA;

    // mark where we are starting our randomization
    int current_address = 0;

    // initialize the initial function addresses
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function1", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048454), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function2", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048471), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function3", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x804848e), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function4", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80484ab), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function5", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80484c8), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function6", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80484e5), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function7", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048502), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function8", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x804851f), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function9", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x804853c), 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function10", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048559), 0x1d)));

    
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function1", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048576), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function2", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048580), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function3", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x804858a), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function4", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x8048594), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function5", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x804859e), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function6", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80485a8), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function7", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80485b2), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function8", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80485bc), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function9", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80485c6), 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function10", pair<unsigned int, unsigned int>(TEXT_TO_FILE(0x80485d0), 0xa)));
    

    // initialise the current addresses
    current_address_map.insert(pair<string, unsigned int>("function1", TEXT_TO_FILE(0x8048454)));
    current_address_map.insert(pair<string, unsigned int>("function2", TEXT_TO_FILE(0x8048471)));
    current_address_map.insert(pair<string, unsigned int>("function3", TEXT_TO_FILE(0x804848e)));
    current_address_map.insert(pair<string, unsigned int>("function4", TEXT_TO_FILE(0x80484ab)));
    current_address_map.insert(pair<string, unsigned int>("function5", TEXT_TO_FILE(0x80484c8)));
    current_address_map.insert(pair<string, unsigned int>("function6", TEXT_TO_FILE(0x80484e5)));
    current_address_map.insert(pair<string, unsigned int>("function7", TEXT_TO_FILE(0x8048502)));
    current_address_map.insert(pair<string, unsigned int>("function8", TEXT_TO_FILE(0x804851f)));
    current_address_map.insert(pair<string, unsigned int>("function9", TEXT_TO_FILE(0x804853c)));
    current_address_map.insert(pair<string, unsigned int>("function10", TEXT_TO_FILE(0x8048559)));

    
    current_address_map.insert(pair<string, unsigned int>("jump_function1", TEXT_TO_FILE(0x8048576)));
    current_address_map.insert(pair<string, unsigned int>("jump_function2", TEXT_TO_FILE(0x8048580)));
    current_address_map.insert(pair<string, unsigned int>("jump_function3", TEXT_TO_FILE(0x804858a)));
    current_address_map.insert(pair<string, unsigned int>("jump_function4", TEXT_TO_FILE(0x8048594)));
    current_address_map.insert(pair<string, unsigned int>("jump_function5", TEXT_TO_FILE(0x804859e)));
    current_address_map.insert(pair<string, unsigned int>("jump_function6", TEXT_TO_FILE(0x80485a8)));
    current_address_map.insert(pair<string, unsigned int>("jump_function7", TEXT_TO_FILE(0x80485b2)));
    current_address_map.insert(pair<string, unsigned int>("jump_function8", TEXT_TO_FILE(0x80485bc)));
    current_address_map.insert(pair<string, unsigned int>("jump_function9", TEXT_TO_FILE(0x80485c6)));
    current_address_map.insert(pair<string, unsigned int>("jump_function10", TEXT_TO_FILE(0x80485d0)));
    
    current_address_map.insert(pair<string, unsigned int>("main", TEXT_TO_FILE(0x080485da)));
    
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
        cout << hex  << "Copying : " << item.first << ". File offset : " << address << " => " << function_length << endl;
        memcpy(tempbuffer + current_address, buffer + address, function_length);

        // record where this function now is (as far as current address)
        current_address_map[item.first] = TEXT_TO_FILE(start + current_address);

        cout << hex << "Putting " << item.first << " at " << current_address_map[item.first] << " to " << current_address_map[item.first] + function_length << endl;
                
        // increase the current address
        current_address += function_length;

        // remove the block from future consideration
        function_addresses.erase(it);
    }

    cout << endl;

    // write the temp buffer out
    //write_file((string(argv[1]) + ".TEMPBUFFER").c_str(), tempbuffer, end - start);

    // copy the temporary buffer back to the normal buffer
    cout << hex << "Temp buffer length: 0x" << end - start << endl;
    cout << hex << "First function at file offset: 0x" << TEXT_TO_FILE(start) << endl;

    memcpy(buffer + TEXT_TO_FILE(start), tempbuffer, end - start);

    /***
     * Jump Patching Stage
     ***/
    list<text_symbol> symbols;
    list<text_symbol>::iterator symbols_it;
    
    symbols.push_back(text_symbols::fun_function1);
    symbols.push_back(text_symbols::fun_function2);
    symbols.push_back(text_symbols::fun_function3);
    symbols.push_back(text_symbols::fun_function4);
    symbols.push_back(text_symbols::fun_function5);
    symbols.push_back(text_symbols::fun_function6);
    symbols.push_back(text_symbols::fun_function7);
    symbols.push_back(text_symbols::fun_function8);
    symbols.push_back(text_symbols::fun_function9);
    symbols.push_back(text_symbols::fun_function10);
    
    
    symbols.push_back(text_symbols::fun_jumpfunction1);
    symbols.push_back(text_symbols::fun_jumpfunction2);
    symbols.push_back(text_symbols::fun_jumpfunction3);
    symbols.push_back(text_symbols::fun_jumpfunction4);
    symbols.push_back(text_symbols::fun_jumpfunction5);
    symbols.push_back(text_symbols::fun_jumpfunction6);
    symbols.push_back(text_symbols::fun_jumpfunction7);
    symbols.push_back(text_symbols::fun_jumpfunction8);
    symbols.push_back(text_symbols::fun_jumpfunction9);
    symbols.push_back(text_symbols::fun_jumpfunction10);
    
    symbols.push_back(text_symbols::fun_main);

    prepare_patch_database();
    /*map<string, list<jumppatching*>* >::iterator patch_mit;
                for ( patch_mit=patch_database->begin() ; patch_mit != patch_database->end(); patch_mit++ )
                {
                        cout << (*patch_mit).first;
                        list<jumppatching*>* t_patches = patch_database->find((*symbols_it).symbolName)->second;
                        cout << ":" << t_patches->size() << endl;
                }
                cout << "----" << endl;
      */   

#ifndef NO_JUMP_PATCHING
   
    for (symbols_it = symbols.begin(); symbols_it != symbols.end(); symbols_it++)
    {
        cout << hex << "Processing " << symbols_it->symbolName << "() from 0x" << symbols_it->address << " length: 0x" << symbols_it->length << endl;
        cout << hex << "\t" << "Currently at: " << current_address_map[symbols_it->symbolName] << endl;
        // Check for if we need to patch jump instructions 
        if (symbols_it->jumppatching >= NOTDETERMINED)
        {

            if (patch_database->count((*symbols_it).symbolName) == 0)
            {
                cout << "No patches for symbol: " << (*symbols_it).symbolName << " in patch database" << endl;
                continue;
            }
            else
            {
                cout << "Patches found for symbol " << (*symbols_it).symbolName << endl;
            }

            bool patch_results = true;

            // hardcoded for rand()
            //patch_results = lowlevel_patch(buffer, current_addresses[(*it).symbolName] + 1, jumppatching::thunk_cx_offset);    
            //patch_results = lowlevel_patch(buffer, current_addresses[(*it).symbolName] + 22, current_addresses["do_rand"]);    
            //continue;

            //map<char*, list<jumppatching*> >::iterator temp_it = patch_database.find((*it).symbolName);
            //if(temp_it == NULL)
            //continue;
            list<jumppatching*>* patches = patch_database->find((*symbols_it).symbolName)->second;

            cout << "Size:" << patches->size() << endl;
            
                
            list<jumppatching*>::iterator patch_it;
            for (patch_it = patches->begin(); patch_it != patches->end(); patch_it++)
            {
                /* Grab the patches that refer to jumps to a function */
                list<patch> function_patches = (*patch_it)->function_call_patches;

                list<patch>::iterator function_it;
                for (function_it = function_patches.begin(); function_it != function_patches.end(); function_it++)
                {
                    //cout << "Funciton call patch" << endl;
                    cout << function_it->function_name << "->" << function_it->dest_function_name << endl;
                    
                    cout << hex << "DEST: " << current_address_map[function_it->dest_function_name] << ":" << 
                            FILE_TO_TEXT(current_address_map[function_it->dest_function_name]) << endl;
                    
                    lowlevel_patch(buffer,
                            current_address_map[function_it->function_name] + function_it->offset,
                            current_address_map[function_it->dest_function_name] + function_it->dest_offset);
                }

            }


            //patch_results = perform_patch(buffer, *it, new_location);
            if (patch_results == false)
            {
                cout << "Jump patching failed for randomized symbol :" << symbols_it->symbolName << endl;
                cout << endl;
                continue;
            }
            else
            {
                cout << "Jump patching succeeded for symbol: " << symbols_it->symbolName << endl;
            }
        }
        else if (symbols_it->jumppatching == NOTDETERMINED)
        {
            cout << "Could not determine if jump patching needed. Might experience errors in symbol:" << symbols_it->symbolName << endl;
            cout << endl;
            continue;
        }
        else
        {
            cout << "Jump patching not needed for symbol: " << symbols_it->symbolName << endl;
        }
        
        cout << endl;
    }

#endif



    // write out the file
    write_file((string(argv[1]) + ".MARLIN").c_str(), buffer, length);

    return 0;
}


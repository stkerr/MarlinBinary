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
    unsigned int end = start + 10 * 0x1D; // + 10 * 0xA;

    // mark where we are starting our randomization
    int current_address = 0;

    // initialize the initial function addresses
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function1", pair<unsigned int, unsigned int>(0x8048454 - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function2", pair<unsigned int, unsigned int>(0x8048471 - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function3", pair<unsigned int, unsigned int>(0x804848e - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function4", pair<unsigned int, unsigned int>(0x80484ab - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function5", pair<unsigned int, unsigned int>(0x80484c8 - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function6", pair<unsigned int, unsigned int>(0x80484e5 - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function7", pair<unsigned int, unsigned int>(0x8048502 - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function8", pair<unsigned int, unsigned int>(0x804851f - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function9", pair<unsigned int, unsigned int>(0x804853c - TEXT_OFFSET, 0x1d)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("function10", pair<unsigned int, unsigned int>(0x8048559 - TEXT_OFFSET, 0x1d)));
/*
    
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function1", pair<unsigned int, unsigned int>(0x8048576 - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function2", pair<unsigned int, unsigned int>(0x8048580 - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function3", pair<unsigned int, unsigned int>(0x804858a - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function4", pair<unsigned int, unsigned int>(0x8048594 - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function5", pair<unsigned int, unsigned int>(0x804859e - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function6", pair<unsigned int, unsigned int>(0x80485a8 - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function7", pair<unsigned int, unsigned int>(0x80485b2 - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function8", pair<unsigned int, unsigned int>(0x80485bc - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function9", pair<unsigned int, unsigned int>(0x80485c6 - TEXT_OFFSET, 0xa)));
    function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >("jump_function10", pair<unsigned int, unsigned int>(0x80485d0 - TEXT_OFFSET, 0xa)));
    */

    // initialise the current addresses
    current_address_map.insert(pair<string, unsigned int>("function1", 0x8048454 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function2", 0x8048471 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function3", 0x804848e - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function4", 0x80484ab - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function5", 0x80484c8 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function6", 0x80484e5 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function7", 0x8048502 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function8", 0x804851f - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function9", 0x804853c - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("function10", 0x8048559 - TEXT_START));

    
    current_address_map.insert(pair<string, unsigned int>("jump_function1", 0x8048576 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function2", 0x8048580 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function3", 0x804858a - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function4", 0x8048594 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function5", 0x804859e - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function6", 0x80485a8 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function7", 0x80485b2 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function8", 0x80485bc - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function9", 0x80485c6 - TEXT_START));
    current_address_map.insert(pair<string, unsigned int>("jump_function10", 0x80485d0 - TEXT_START));
    
    current_address_map.insert(pair<string, unsigned int>("main", 0x080485da - TEXT_START));
    
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
        memcpy(tempbuffer + current_address, buffer + address, function_length);

        // record where this function now is (as far as current address)
        current_address_map[item.first] = start + current_address - TEXT_START;

        // increase the current address
        current_address += function_length;

        // remove the block from future consideration
        function_addresses.erase(it);
    }


    // write the temp buffer out
    write_file((string(argv[1]) + ".TEMPBUFFER").c_str(), tempbuffer, end - start);

    // copy the temporary buffer back to the normal buffer
    cout << hex << "Temp buffer length: 0x" << end - start << endl;
    cout << hex << "File offset: 0x" << start - TEXT_OFFSET << endl;

    memcpy(buffer + start - TEXT_OFFSET, tempbuffer, end - start);

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
    
    //symbols.push_back(text_symbols::fun_main);

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
    for (symbols_it = symbols.begin(); symbols_it != symbols.end(); symbols_it++)
    {
        cout << hex << "Processing " << symbols_it->symbolName << "() from 0x" << symbols_it->address << " length: 0x" << symbols_it->length << endl;

        /* Check for if we need to patch jump instructions */
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
                    cout << "Funciton call patch" << endl;
                    cout << function_it->function_name << endl;
                    cout << function_it->dest_function_name << endl;
                    cout << hex << current_address_map[function_it->dest_function_name] + FILE_START << ":" << 
                            current_address_map[function_it->dest_function_name] + TEXT_START << endl;
                    
                    lowlevel_patch(buffer,
                            current_address_map[function_it->function_name] + function_it->offset + FILE_START,
                            current_address_map[function_it->dest_function_name] + function_it->dest_offset + FILE_START);
                }

            }


            //patch_results = perform_patch(buffer, *it, new_location);
            if (patch_results == false)
            {
                cout << "Jump patching failed for randomized symbol :" << symbols_it->symbolName << endl;
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
            continue;
        }
        else
        {
            cout << "Jump patching not needed for symbol: " << symbols_it->symbolName << endl;
        }
    }





    // write out the file
    write_file((string(argv[1]) + ".MARLIN").c_str(), buffer, length);

    return 0;
}


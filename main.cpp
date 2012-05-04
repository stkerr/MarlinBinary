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
#include <fstream>

#include "jumppatching.h"
#include "text_symbols.h"

using namespace std;

list<text_symbol> entry_list;

vector<pair<string, pair< unsigned int, unsigned int> > > function_addresses; // function name, <start address, length>
map < string, unsigned int > current_address_map; // function name, current address


unsigned int TEXT_START;
unsigned int FILE_START;

/**
 * Reads the given file and adds to the entry list
 * Format:
 *      SymbolName Address Length JumpPatching
 * @param path
 */
void process_entry_argument(char* path)
{
    ifstream file;
    file.open(path, ifstream::in);
    
    while(!file.eof())
    {
        char buffer[250];
        memset(buffer, 0, 250);
        file.getline(buffer, 250);
    
        if(strlen(buffer) == 0)
            break;
        
        text_symbol temp;
        
        char* toks = strtok(buffer, " \n");
        
        strcpy(temp.symbolName, toks);
        
        toks = strtok(NULL, " \n");
        
        temp.address = strtoul (toks,NULL,0);
        
        toks = strtok(NULL, " \n");
        
        temp.length = strtoul (toks,NULL,0);
        
        toks = strtok(NULL, " \n");
        
        temp.jumppatching = strtoul (toks,NULL,0);
        
        entry_list.push_back(temp);
    }
    
    file.close();   
}

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
    
            
    if (argc != 6)
    {
        cout << "Usage: " << argv[0] << " BinaryFilename  SymbolFilename PatchFilename FileStart TextStart" << endl;
        return -1;
    }

    process_entry_argument(argv[2]);
    read_patch_file(argv[3]);
    FILE_START = strtoul(argv[4], NULL, 0);
    TEXT_START = strtoul(argv[5], NULL, 0);
    
    list<text_symbol> symbols;
    list<text_symbol>::iterator symbols_it;
    
    // get the file as a buffer
    unsigned long length;
    unsigned char* buffer = read_file(argv[1], &length);

    // mark the start and end locations for the randomization
    unsigned int start = entry_list.front().address;
    unsigned int end = start;
    for(symbols_it = entry_list.begin(); symbols_it != entry_list.end(); symbols_it++)
    {
        if(strcmp(symbols_it->symbolName, "main") == 0)
            continue;
        
        end = end + symbols_it->length;
    }
    

    // mark where we are starting our randomization
    int current_address = 0;

    // initialize the initial function addresses
    list<text_symbol>::iterator s_it;
    cout << "Entry list: " << entry_list.size() << endl;
    for(s_it = entry_list.begin(); s_it != entry_list.end(); s_it++)
    {
        cout << (s_it->jumppatching & DONOTADD) << endl;
        if((s_it->jumppatching & DONOTADD) != 1 && strcmp(s_it->symbolName,"main")!= 0)
        {
            cout << "adding " << s_it->symbolName << endl;
                function_addresses.push_back(pair<string, pair<unsigned int, unsigned int> >(s_it->symbolName, pair<unsigned int, unsigned int>(s_it->address, s_it->length)));
        }
        
        current_address_map.insert(pair<string, unsigned int>(s_it->symbolName, s_it->address));
        
        symbols.push_back(*s_it);
    }
    
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
        memcpy(tempbuffer + current_address, buffer + TEXT_TO_FILE(address), function_length);

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
    
    /*
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
    */

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

//#define NO_JUMP_PATCHING
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
                    cout << function_it->function_name << "[" << current_address_map[function_it->function_name] << "]" <<
                            "->" << function_it->dest_function_name <<
                            "[" << current_address_map[function_it->dest_function_name] << "]" << endl;
                    
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


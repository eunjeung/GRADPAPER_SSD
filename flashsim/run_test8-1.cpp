/* Copyright 2009, 2010 Brendan Tauras */
 /* run_test.cpp is part of FlashSim. */
 /* FlashSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version. */
 /* FlashSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. */
 /* You should have received a copy of the GNU General Public License
 * along with FlashSim.  If not, see <http://www.gnu.org/licenses/>. */
 /****************************************************************************/
 /* Basic test driver
 * Brendan Tauras 2009-11-02
 *
 * driver to create and run a very basic test of writes then reads */
#include "ssd.h"
#include <string.h>
#include <math.h>

//#define USER_ADDRESS_SPACE (int)(ceil(NUMBER_OF_ADDRESSABLE_PAGES*0.6))

#define NUMBER_OF_ADDRESSABLE_PAGES (int)(NUMBER_OF_ADDRESSABLE_BLOCKS*BLOCK_SIZE)
#define FILE_SIZE_A (int)(ceil(BLOCK_SIZE*0.3))
#define FILE_SIZE_B (int)(ceil(BLOCK_SIZE*1.6))
#define FILE_SIZE_C (int)(ceil(BLOCK_SIZE*0.7))

 using namespace ssd;

int main()
{
	
	load_config();
	print_config(NULL);
	printf("\n");
 	Ssd *ssd = new Ssd();
	
	printf("\nPAGE_SIZE : %d\n", PAGE_SIZE);
	printf("NUMBER_OF_ADDRESSABLE_PAGES : %d\n", NUMBER_OF_ADDRESSABLE_PAGES);
	
 	double result;
	double start_time=0;
	int count1 = 0, count2 = 0, count3 =0;
	int ret, ret1, ret2;
	
	void *buff1 = malloc(sizeof(char)*PAGE_SIZE); 
	void *buff2 = malloc(sizeof(char)*PAGE_SIZE);
	void *buff3 = malloc(sizeof(char)*PAGE_SIZE);
	
 	memset(buff1, 1, sizeof(char)*PAGE_SIZE);
	memset(buff2, 2, sizeof(char)*PAGE_SIZE);
	memset(buff3, 3, sizeof(char)*PAGE_SIZE);
	
	//#TEST_CASE_5 - write FILE B(size : 160% block size (content : '2') ) and check how many times it should be overwritten. 


    // FIXME: This write is starting from LPN == 0, you should start at a different offset.
    // File B should be written at the ofsset as if File A was written, that is, at sizeof(FILE_A) offset.
	//for (int i = FILE_SIZE_A; i < (FILE_SIZE_A + FILE_SIZE_B); i++)
	for (int i = 0; i < FILE_SIZE_B; i++)
		result = ssd -> event_arrive(WRITE, i, 1, (double)(300*i), buff1);
	
	for(int i=0;i<NUMBER_OF_ADDRESSABLE_PAGES;i++){
		ret = memcmp((page_data+(i*PAGE_SIZE)),buff1,(sizeof(char)*PAGE_SIZE));
		if(ret==0)
            count1++;
	}

    // Make sure that count1 is the same as the FILE_SIZE_B
    assert(count1 == FILE_SIZE_B);

 	printf("\n--------- 1st test \n");
	printf("number of '1' : %d \n", count1);
	printf("number of '2' : %d \n", count2);	
 	ssd -> print_statistics();
	
	result = 0;

    // WRite from the beginning and check if the write resulted in the erase of
    // the old file. Stop when the old file is gone.
   
    int i = 0, it = 1;
    while (1) {
        printf("It: %d\n", ++it);
        // Check if we need to wrap the i back to the beginning
        if (i >= NUMBER_OF_ADDRESSABLE_PAGES)
            i = 0;

        // Write a single page at 'i' address
		result += ssd -> event_arrive(WRITE, i, 1, (double)(300*i), buff2);

        // Compare the memory to check if we still find the old file
        for(int i = 0, count1 = 0, count2 = 0; i < NUMBER_OF_ADDRESSABLE_PAGES; i++){
            ret = memcmp((page_data+(i*PAGE_SIZE)),buff1,(sizeof(char)*PAGE_SIZE));
            if(ret==0) {
                count1++;
                continue;
            }
            ret1 = memcmp((page_data+(i*PAGE_SIZE)),buff2,(sizeof(char)*PAGE_SIZE));
            if(ret1==0) {
                count2++;
                continue;
            }
        }

        // If the count1 is == 0, we erased the file! So, stop!
        if (count1 == 0)
            break;

        // Otherwise, we need to count again at the next iteration.
        count1 = count2 = 0;
    }

    assert(count1 == 0);
    assert(count2 == i);

 	printf("\n--------- 3rd test!! \n");
	printf("Number of iterations : %d \n", it);
	printf("number of '1' : %d \n", count1);
	printf("number of '2' : %d \n", count2);
	printf("number of '3' : %d \n", count3);
 	printf("\n\n---------- time : %lf\n\n", result);	
 	ssd -> print_statistics();
 	delete ssd;
	free(buff1);
	free(buff2);
	free(buff3);
	
	return 0;
}

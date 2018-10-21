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

	for (int i = 0; i < FILE_SIZE_B; i++)
	{
		//long int r = random()%SIZE;
		//printf("%d: %d\n", i, r);
		result = ssd -> event_arrive(WRITE, i, 1, (double)(300*i), buff1);
	}
	
	for(int i=0;i<NUMBER_OF_ADDRESSABLE_PAGES;i++){
		ret = memcmp((page_data+(i*PAGE_SIZE)),buff1,(sizeof(char)*PAGE_SIZE));
		if(ret==0) count1++;
		else{
			ret1 = memcmp((page_data+(i*PAGE_SIZE)), buff2, (sizeof(char)*PAGE_SIZE));
			if(ret1==0) count2++;
		}
	}
 	printf("\n--------- 1st test \n");
	printf("number of '1' : %d \n", count1);
	printf("number of '2' : %d \n", count2);	
 	ssd -> print_statistics();
	
	result = 0;
	for (int i = 0; i < (NUMBER_OF_ADDRESSABLE_PAGES-(BLOCK_SIZE*8)); i++)
	{
		result += ssd -> event_arrive(WRITE, i, 1, (double)(300*i), buff2);
	}
 	count1=0;
	count2=0;

 	for(int i=0;i<NUMBER_OF_ADDRESSABLE_PAGES;i++){
		ret = memcmp((page_data+(i*PAGE_SIZE)),buff1,(sizeof(char)*PAGE_SIZE));
		if(ret==0) count1++;
		
		else{
			ret1 = memcmp((page_data+(i*PAGE_SIZE)),buff2,(sizeof(char)*PAGE_SIZE));
			if(ret1==0) count2++;
		}
	}
 	printf("\n--------- 2nd test \n");
	printf("number of '1' : %d \n", count1);
	printf("number of '2' : %d \n", count2);
 	ssd -> print_statistics();
	

	for (int i = 0; i < (NUMBER_OF_ADDRESSABLE_PAGES-(BLOCK_SIZE*13)); i++)
	{
		//long int r = random()%SIZE;
		//printf("%d: %d\n", i, r);
		result += ssd -> event_arrive(WRITE, i, 1, (double)(300*i), buff2);
	}
 	count1=0;
	count2=0;
	count3=0;
 	for(int i=0;i<NUMBER_OF_ADDRESSABLE_PAGES;i++){
		ret = memcmp((page_data+(i*PAGE_SIZE)),buff1,(sizeof(char)*PAGE_SIZE));
		if(ret==0) count1++;
		else{
			ret1 = memcmp((page_data+(i*PAGE_SIZE)),buff2,(sizeof(char)*PAGE_SIZE));
			if(ret1==0) count2++;
			else{
				ret2 = memcmp((page_data+(i*PAGE_SIZE)),buff3,(sizeof(char)*PAGE_SIZE));
				if(ret2==0) count3++;
			}
		}
	}
 	printf("\n--------- 3rd test \n");
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

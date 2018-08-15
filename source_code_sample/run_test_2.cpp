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
#define SIZE 204800
//#define SIZE 131072
//#define SIZE 157600
//#define SIZE 183520

//#define SIZE 65536

using namespace ssd;

int main()
{
	load_config();
	print_config(NULL);
	printf("\n");

	Ssd *ssd = new Ssd();

	double result;
	double start_time=0, afterFormatStartTime = 19200;
	int count = 0, value=1;
	
	for (int i = 0; i < SIZE; i++)
	{
		long int r = random()%SIZE;
		result = ssd -> event_arrive(WRITE, r, 1, afterFormatStartTime, &value);
		//printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}
	value++;	
	int tmp;
	int count2=0;

	for (int i = 0; i < SIZE; i++)
	{
		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
		//printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++)
	{
		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
		//printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++)
	{

		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
	//	printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++){
		long int r = random()%SIZE;
		result = ssd -> event_arrive(WRITE, r, 1, afterFormatStartTime, &value);		
		//	printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}
/*
		for (int i = 0; i < SIZE; i++)
	{
		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
		//printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++)
	{

		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
	//	printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++){
		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);		
		//	printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++)
	{
		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
		//printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++)
	{

		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);
	//	printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}

	for (int i = 0; i < SIZE; i++){
		result = ssd -> event_arrive(WRITE, i, 1, afterFormatStartTime, &value);		
		//	printf("Write time: %.20lf\n", result);
		afterFormatStartTime += result;
	}
*/
	start_time += afterFormatStartTime;	
	/*
	for (int i = 0; i < SIZE-64; i++)
	{
		result = ssd -> event_arrive(READ, 64+i, 1, start_time);
		if((*(int*) global_buffer)==1) count++;
		else if ((*(int*) global_buffer)==2) count2++; 
		start_time += result;
		// printf("Read time : %.20lf\tRead : %d\n", result, *(int*) global_buffer);
	}
	*/
	value = 1;
	int value1 = 2;
	int ret, ret1;
	for(int i=0;i<262144;i++){
		ret = memcmp(page_data+i*PAGE_SIZE,&value,4);
		if(ret==0) count++;
		else{
			ret1 = memcmp(page_data+i*PAGE_SIZE,&value1,4);
			if(ret1==0) count2++;
		}
	}

	printf("number of '1' : %d \n", count);
	printf("number of '2' : %d \n", count2);
	ssd -> print_statistics();
	delete ssd;
	return 0;
}

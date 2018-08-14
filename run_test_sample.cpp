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
#define SIZE 24
//#define SIZE 262144
#define TOTAL_SIZE ((int)(SIZE+SIZE*0.2))

using namespace ssd;

int main()
{
	load_config();
	print_config(NULL);
	printf("\n");

	Ssd *ssd = new Ssd();

	double result;
	double start_time=0;
	//double afterFormatStartTime = 19200;
	int count1 = 0, count2 = 0, count3 =0;
	//int value=1;
	char *buff1, *buff2, *buff3;
	int ret, ret1, ret2;

	buff1 = malloc(sizeof(char)*PAGE_SIZE);
	buff2 = malloc(sizeof(char)*PAGE_SIZE);
	buff3 = malloc(sizeof(char)*PAGE_SIZE);

	memset(buff1, 1, sizeof(char)*PAGE_SIZE);
	memset(buff2, 2, sizeof(char)*PAGE_SIZE);
	memset(buff3, 3, sizeof(char)*PAGE_SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		//long int r = random()%SIZE;
		result = ssd -> event_arrive(WRITE, i, 1, (double)(300*i), buff1);
		//printf("Write time: %.20lf\n", result);
		//start_time += result;
	}

	for(int i=0;i<TOTAL_SIZE;i++){
		ret = memcmp(page_data+i*PAGE_SIZE,buff1,(sizeof(char)*PAGE_SIZE));
		if(ret==0) count1++;
	}

	/*
	for (int i = 0; i < SIZE; i++)
	{
		result = ssd -> event_arrive(WRITE, i, 1, start_time, buff2);
		//printf("Write time: %.20lf\n", result);
		start_time += result;
	}


	for (int i = 0; i < SIZE; i++)
	{
		result = ssd -> event_arrive(WRITE, i, 1, start_time, buff3);
		//printf("Write time: %.20lf\n", result);
		start_time += result;
	}
	*/
	/*
	for(int i=0;i<TOTAL_SIZE;i++){
		ret = memcmp(page_data+i*PAGE_SIZE,&value,4);
		if(ret==0) count1++;
		else{
			ret1 = memcmp(page_data+i*PAGE_SIZE,&value1,4);
			if(ret1==0) count2++;
			else{
				ret2 = memcmp(page_data+i*PAGE_SIZE, &value2, 4);
				if(ret2==0) count3++;
			}
		}
	}
	*/
	printf("number of '1' : %d \n", count1);
	//printf("number of '2' : %d \n", count2);
	//printf("number of '3' : %d \n", count3);
	ssd -> print_statistics();
	delete ssd;
	return 0;
}

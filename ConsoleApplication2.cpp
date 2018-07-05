// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

uint8_t* loadFile(char* filename_t, uint32_t& filelength);
uint32_t checkSum(const uint8_t *data, uint32_t length);
uint32_t uint8to32(const uint8_t *data, int i, int max);

int main()
{
	uint32_t length = 0;
	auto buf = loadFile("recv_",length);
	
	checkSum(buf,length);
	system("pause");
    return 0;
}



uint8_t* loadFile(char* filename_t, uint32_t& length) {
	char filename[20];
	uint8_t* buf_all = new uint8_t[0];
	for (int i = 0; i < 4; i++) {
		FILE *fp;
		sprintf(filename, "%s%d%s", filename_t, i, ".log");
		if (fp = fopen(filename, "rb+")) {
			printf("open file success!/n");
		}
		else {
			printf("open file fail!/n");
		};
		fseek(fp, 0, SEEK_END);
		int filelength = ftell(fp);
		auto buf = new uint8_t[filelength];
		fseek(fp, 0, SEEK_SET);
		int eleNum;
		eleNum = fread(buf, 1, filelength, fp);
		int datalength = buf[1252];

		auto buf2 = new uint8_t[length];
		memcpy(buf2, buf_all, length* sizeof(uint8_t));
		length += filelength;
		buf_all = new uint8_t[length];
		memcpy(buf_all, buf2, (length- filelength) * sizeof(uint8_t));
		memcpy(&buf_all[length - filelength], buf, filelength * sizeof(uint8_t));		
		delete buf;
		delete buf2;
		fclose(fp);
	}
//	auto buf1 = new uint32_t[length];
//	for (int i = 0; i < length; i++) {
//		buf1[i] = (buf_all[i * 4] << 24) + (buf_all[i * 4 + 1] << 16) + (buf_all[i * 4 + 2] << 8) + buf_all[i * 4 + 3];
//	}
	return buf_all;
}


uint32_t checkSum(const uint8_t *data, uint32_t alllength)
{
	uint32_t offset = 0;
	uint32_t error_num = 0;
	uint32_t packets_num = 0;
	uint32_t correct_index = 0;
	while (offset < alllength-1) {
		uint32_t length_t = uint8to32(data, offset + 2 * 4, alllength);
		uint32_t checksum = uint8to32(data, offset + 1 * 4, alllength);
		offset += (length_t + 3*4);
		//length += length_t + 1;
		packets_num++;

		uint32_t retval = 0;
		retval ^= uint8to32(data, offset, alllength);
		printf("%s%d%s", "packet index: ", uint8to32(data, offset, alllength), "packet content: ");
		for (int i=0;i<length_t/4;i++)
		{
			retval ^= uint8to32(data, offset + 3 * 4 + i * 4, offset + length_t);
			//sprintf()
			//printf("%d\t", uint8to32(data, offset + 3 * 4 + i * 4, offset + length_t));
		}
		printf(" \n");
		if (retval != checksum) {
			error_num++;
		}
		else {
			correct_index = uint8to32(data, offset, alllength);
		}
	}	
	printf("%s%d\n%s%d\n", "total packet number: ", packets_num, "error number: ", error_num);
	printf("%s%d\n", "correct packet index: ", correct_index);
	return 1;
}

uint32_t uint8to32(const uint8_t *data, int i, int max) {
	int dis = i + 4 - max;
	uint32_t test;
	switch (dis) {
	case 1:	
		test = (data[i] << 24) + (data[i + 1] << 16) + (data[i + 2] << 8) + 0xAB;
		return test;
	case 2: 
		test = (data[i] << 24) + (data[i + 1] << 16) + (0xAB << 8) + 0xAB;
		return test;
	case 3: 
		test = (data[i] << 24) + (0xAB << 16) + (0xAB << 8) + 0xAB;
		return test;
	default: 
		test = (data[i] << 24) + (data[i + 1] << 16) + (data[i + 2] << 8) + data[i + 3];
		return test;
	}

}


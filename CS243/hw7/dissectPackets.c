#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

typedef struct IPStruct{//structure consisting of the ip header parts
	unsigned int IHL:4;
	unsigned int version:4;
	unsigned int TOS:8;
	char length[2];
	char ID[2];
	char funky[2];
	//unsigned int flag:3;
	//unsigned int offSet:13;
	unsigned int TTL:8;
	unsigned int protocol:8;
	char checksum[2];
	char source[4];
	char dest[4];
}IPHead;

//print the protocol and its correct acronym
//@param prot the integer value of the protocol
void printProt(int prot){
	printf("Protocol:\t\t");
	switch(prot){
		case 1:
			printf("ICMP ");
			break;
		case 2:
			printf("IGMP ");
			break;
		case 6:
			printf("TCP ");
			break;
		case 9:
			printf("IGRP ");
			break;
		case 17:
			printf("UDP ");
			break;
		case 47:
			printf("GRE ");
			break;
		case 50:
			printf("ESP ");
			break;
		case 51:
			printf("AH ");
			break;
		case 57:
			printf("SKIP ");
			break;
		case 88:
			printf("EIGRP ");
			break;
		case 89:
			printf("OSPF ");
			break;
		case 115:
			printf("L2TP ");
			break;
	}
	printf("0x%X (%d)\n", prot, prot);
}

//print a source or destination address in octet form
//@param add the address in character array form
//@param type a character string ("Source" or "Destination) to print
void printAdd(char add[4], char *type){
	printf("%s Address:\t\t", type);
	for(int i=0;i<4;i++){
		printf("%hhu", add[i]);
		if(i!=3){
			printf(".");
		}else{
			printf("\n");
		}
	}
}

//print the information in the IP header
//@param head an IPHead struct
//@param num the packet number being printed
static void printIP(IPHead *head, int num){
	printf("==>Packet %d\n", num);
	printf("Version:\t\t0x%X (%d)\n", head->version, head->version);
	printf("IHL (Header Length):\t\t0x%X (%d)\n", head->IHL, head->IHL);
	printf("Type of Service (TOS):\t\t0x%X (%d)\n", head->TOS, head->TOS);
	printf("Total Length:\t\t0x%hX (%hu)\n", *(short *)&(head->length), 
		*(short *)&(head->length));
	printf("Identification:\t\t0x%hX (%hu)\n", *(short *)&(head->ID), 
		*(short *)&(head->ID));
	printf("IP Flags:\t\t0x%X (%d)\n", head->funky[0], head->funky[0]);
	printf("Fragment Offset:\t\t0x%X (%d)\n", head->funky[1], head->funky[1]);
	printf("Time To Live (TTL):\t\t0x%X (%d)\n", head->TTL, head->TTL);
	printProt(head->protocol);
	printf("Header Checksum:\t\t0x%hX (%hu)\n", *(short *)&(head->checksum), 
		*(short *)&(head->checksum));
	printAdd(head->source, "Source");
	printAdd(head->dest, "Destination");
}

//swap the endianness of a character array
//@param data array to be swapped
//@param bytes number of bytes in the data
//@return the endian swapped data
static char* endSwap(char *data, int bytes){//helper for endian-ness
	char *ret = malloc(bytes);
	for(int i=0,j=(bytes-1);i<bytes;i++, j--){
		ret[j]=data[i];
	}
	return ret;
}

//the main program that validates input and calls printIP after copying
//binary data into an IPHead struct
//@param argc number of arguments
//@param argv the arguments
//@return success or failure
int main(int argc, char *argv[]){
	FILE *fp = NULL;
	if(argc!=2){//wrong number of inputs
		printf("correct usage is [program] [filename]\n");
		return EXIT_FAILURE;
	}
	fp = fopen(argv[1], "rb");
	if(fp == NULL){//cannot open file
		perror(argv[1]);
		return EXIT_FAILURE;
	}
	int numPack=0;
	int packSize = 0;
	int read = 0;
	read = fread(&numPack, 4, 1, fp);//read number of packets
	if(read!=1){//make sure there was a number
		printf("failed to read the number of packets\n");
		return EXIT_FAILURE;
	}
	printf("==== File %s contains %d Packets.\n", argv[1], numPack);
	for(int i=0;i<numPack;i++){//process packets
		read = fread(&packSize, 4, 1, fp);//read size
		if(read!=1||packSize<1){
			printf("failed to read size of packet\n");
			return EXIT_FAILURE;
		}
		IPHead *head=NULL;
		char *temp = malloc(packSize);
		head = (IPHead*)temp;
		read = fread(head, packSize, 1, fp);//read packet
		if(read!=1){
			printf("error reading packet data\n");
			return EXIT_FAILURE;
		}
		char *a=endSwap(head->length, 2);
		char *b= endSwap(head->ID, 2);
		char *c= endSwap(head->checksum, 2);
		//char *d=endSwap(head->funky, 2);
		strncpy(head->length, a, 2);
		strncpy(head->ID, b, 2);
		strncpy(head->checksum, c, 2);
		//strncpy(head->funky, d, 2);
		printIP(head, i+1);
		free(a);
		free(b);
		free(c);
		//free(d);
		free(head);
	}
	fclose(fp);
}
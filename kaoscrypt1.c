//     __                                      __ 
//    / /_____ ____  _________________ _____  / /_
//   /  '_/ _ `/ _ \(_-<___/ __/ __/ // / _ \/ __/
//  /_/\_\\_,_/\___/___/   \__/_/  \_, / .__/\__/ 
//                                /___/_/         
//
//
// Algorithm idea and assembly of code by:
// 	Mark C.	-	markc@sec-1.com
// 			mm13mrc@leeds.ac.uk
//
//	With thanks to friends and colleagues:
//		Edward	@edwardbowles
//		Holly	@HollyGraceful
//		Matt	@_castleinthesky
//		Olly	@grimhacker
//
//	First Version Completed:
//		Something-th Aug 2015
//	in Leeds, UK.
//
// Released under GPLv2 licence.
// 
// This is a reimplementation of the kaos-hash
// algorithm, only to generate a keystream that is
// then used to XOR encrypt something.
//
// Remember:
//
// 	NEVER ROLL YOUR OWN CRYPTO!!
// 
// [[ Unless, you're a mathematician, that is! ;-) ]]
//
// Let's do it.

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <math.h>
 
typedef unsigned long ul;
#define N  32 //(sizeof(ul) * CHAR_BIT)
#define B(x) (1UL << (x))

ul kaos_iter(ul state)
{
	int i;
	if (state == 0)
	{state=2155905152;} 	// adds a 1 in the ul, so null becomes [10000000 10000000 10000000 10000000]
	ul st = state;		// ... The devil's seed...
	for (state = i = 0; i < N; i++) //opera tenebrarum - this segment was found on RosettaCode, but was fixed here.
		if (30 & B(7 & (st>>(i-1) | st<<(N-i))))//ars tenebrae - the only change from 64 to 32bit was (N+1-i) became (N-i)
			state |= B(i); 			//this fixes the periodic state. RosettaCode isn't perfect...
	return state;
}
 
ul kaos_evolve(ul state, int steps)
{
	int j; 
	for(j=0;j<steps;j++) {
		state = kaos_iter(state);
	};
	return state;
}

int librum_memoriae(const char *filename, unsigned char **output)
{
	int size=0;
	FILE *fp = fopen(filename,"rb");
	if (fp == NULL)
	{
		*output = NULL;
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*output = (char *)malloc(size+1); // semper vigilens
	if (size != fread(*output, sizeof(char), size, fp))
	{
		free(*output);
		return -2;
	}
	fclose(fp);
	(*output)[size]=0;
	return size;
}

int buffer_quatuor(unsigned char *inputArray, unsigned char *outputArray, int pos, int iter, int size)
{
	ul locus,output;
	locus = (inputArray[pos]<<24) | (inputArray[pos+1]<<16) | (inputArray[pos+2]<<8) | inputArray[pos+3];        
        output = kaos_evolve(locus,iter/*1*/);
        outputArray[pos]=(char)((output >> 24) & 0xFF);
        outputArray[pos+1]=(char)((output >> 16) & 0xFF);
        outputArray[pos+2]=(char)((output >> 8) & 0xFF);
        outputArray[pos+3]=(char)((output >> 0) & 0xFF);
	return 0;
}

int obscura(const char *filename, int iter, int length) // This is only included as I want to make 'kaos.h' eventually.
{
        int i,s;
	unsigned char *content;
	unsigned char *decorum;
        int size;
        size = librum_memoriae(filename,&content);
        int size2;
	if (size<0)
        {
                puts("File is not a thing");
                return 1;
        }
        int tail = size % 4;
        printf("\ntail: %d\n",tail);
	decorum=malloc(size+8);
	if (length>(size-1)){puts("lengthening function not implemented, yet!");return -1;}
	//for (m=1;m<=iter;m++){ 
	for(size2=size; size2 > (length/* *4 */); size2 -= 4)
	{
		//puts("\nMade it!\n");
		buffer_quatuor(content,decorum,0,iter,size2);
        	for (s = 1; s < (size2/4); s++)
		{
			buffer_quatuor(content,decorum,(s*4),iter,size2);
		}
		memmove(content,decorum,size*sizeof(unsigned char));
		//Until I can trust memcpy. Dreams are forever broken...
		//for (size_t m = 0; m < size; m++){set(content, m, get(decorum,m));}
	}	//}
	puts("kaos hash is:");
	for(i=0; i < (length); i++) // dirty hack until I know why the third character changes
        	{printf("%02X", decorum[i]);}
	free(decorum);
	printf("\nnumber of times: %d",(int)(size/4));
	return 0;
}

int kaos_nascitur(unsigned char *terrae, ul ovum, int size)
{
	int pos; ul hecate = kaos_evolve(ovum,16);	// May her daughters fill the earth...
        if (size < 4){puts("\nThis is not a gift.\n");return -1;}
	for (pos = 0; pos < size; pos += 4){
		hecate = kaos_evolve(hecate,16);
		terrae[pos]=(char)((hecate >> 24) & 0xFF);
        	terrae[pos+1]=(char)((hecate >> 16) & 0xFF);
        	terrae[pos+2]=(char)((hecate >> 8) & 0xFF);
        	terrae[pos+3]=(char)((hecate >> 0) & 0xFF);
	}
	return 0;
}

int occultum(const char *filename, ul ovum)	// "Wovon man nicht sprechen kann, darüber muss man schweigen."
{
        int i,j;
        unsigned char *content;
	unsigned char *terrae;
        unsigned char *decorum;
        int size;
        size = librum_memoriae(filename,&content);
        if (size<0)
        {
                puts("File is not a thing");
                return 1;
        }
	decorum=malloc(size+8);
        terrae=malloc(size+8);
        int nasc = kaos_nascitur(terrae, ovum, size);
        if (nasc < 0){puts("\nSomething is rotten in the state of Denmark...\n");return -1;}
	// ...occultare aut revelare...
	for(i=0;i<size;i++){
		decorum[i] = content[i] ^ terrae[i];
	}
	// this is a debug showing the input file in hex
        //putchar('\n');puts("Content:\t");
        //for(j=0; j < size; j++)
        //        {printf("%02X", content[j]);}
        // tihs is a debug showing the generated keystream
	//putchar('\n');puts("Terrae:\t");
        //for(j=0; j < size; j++)
        //        {printf("%02X", terrae[j]);}
	putchar('\n');puts("Encrypted:\t\n");
	for(j=0; j < size; j++)
                {printf("%02X", decorum[j]);}
	return 0;
}

int apertus(const char *filename, ul ovum)	// "Wir müssen wissen - Wir werden wissen!"
{
	return 0;
}
 
int main(int argc, char **argv)
{
	long ovum;
	if (argc < 2){puts("\n     __                                      __ \n    / /_____ ____  _________________ _____  / /_\n   /  '_/ _ `/ _ \\(_-<___/ __/ __/ // / _ \\/ __/\n  /_/\\_\\\\_,_/\\___/___/   \\__/_/  \\_, / .__/\\__/ \n                                /___/_/    \n\nusage: kaoscrypt <filename> <seed>");return 0;}
	if (!argv[2]){ ovum = 15;} else { ovum = strtol(argv[2], NULL, 16);}
	// This is the hashing func:
	// int worked = obscura(argv[1],iter,length);
	// Now for the crypter:
	int worked = occultum(argv[1],ovum);
	return 0;
}

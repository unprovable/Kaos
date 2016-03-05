//     __                    __            __ 
//    / /_____ ____  _______/ /  ___ ____ / / 
//   /  '_/ _ `/ _ \(_-<___/ _ \/ _ `(_-</ _ \
//  /_/\_\\_,_/\___/___/  /_//_/\_,_/___/_//_/
//
//	Kaos Digest v1 
//	And essentially cut down version of Kaos Hash
//	designed for speed. We lose a parameter, but we
//	gain many friends! (Namely, speed...)
//
// Algorithm idea and assembly of code by:
// 	Mark C.	-	markc@sec-1.com
// 			mm13mrc@leeds.ac.uk
//
//      With thanks to friends and colleagues:
//              Edward  @edwardbowles
//              Holly   @HollyGraceful
//              Matt    @_castleinthesky
//              Olly    @grimhacker
//
//	First Version Completed:
//		17th October  2015
//	in Leeds, UK.
//
// Released under GPLv2 licence.
//
// Remember:
//
// 	NEVER ROLL YOUR OWN CRYPTO!!
// 
// [[ Unless, you're a mathematician, that is! ;-) ]]
//
// Let's do it...

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
	for (state = i = 0; i < N; i++) //opera tenebrarum - taken from RosettaCode - good stuff, but was broken.
		if (30 & B(7 & (st>>(i-1) | st<<(N-i))))//ars tenebrae - the only change from 64 to 32bit was (N+1-i) became (N-i)
			state |= B(i); 			//this fixes the periodic state. RosettaCode is not perfect!
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
	size = ftello(fp); // non *nix use; size = ftell(fp);
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

int buffer_quatuor(unsigned char *inputArray, int pos, int iter)
{
	ul locus,output;
	locus = (inputArray[pos]<<24) | (inputArray[pos+1]<<16) | (inputArray[pos+2]<<8) | inputArray[pos+3];        
        output = kaos_evolve(locus,iter/*1*/);
        inputArray[pos]=(char)((output >> 24) & 0xFF);
        inputArray[pos+1]=(char)((output >> 16) & 0xFF);
        inputArray[pos+2]=(char)((output >> 8) & 0xFF);
        inputArray[pos+3]=(char)((output >> 0) & 0xFF);
	return 0;
}

// Didn't end up using these... kept them anyway.
//int get( unsigned char *src, const size_t index ) { return src[index]; }
//int set( unsigned char *dst, const size_t index, const int value ) { dst[index] = value; }

int concoquo(const char *filename, int iter, int length)
{
        int i,s;
	unsigned char *content;
        int size;
        size = librum_memoriae(filename,&content);
	if (size<0)
        {
                puts("File is not a thing");
                return 1;
        }
        printf("Generating hash...");
	if (length>(size-1)){puts("lengthening function not implemented, yet!");return -1;}
	buffer_quatuor(content,0,iter);
        for (s = 1; s < (size-4); s++) // iterate the hashing function
	{
		buffer_quatuor(content,s,iter);
	}
	for (s = (size-4); s > (size - length); s--) 	// double back on ourselves so that if the last 
	{						// bytes are different, it also changes the hash.
		buffer_quatuor(content,s,iter);
	}
	puts("\nkaos hash is:");
	// The next three lines are just a bit of debug... I keep them here as it's something I always need to heck.
	//for(i=0;i < size; i++)
	//	{printf("%02X", content[i]);}
	//printf("\n\n");
	int outstart = size - length -1;
	for(i=0; i < length; i++)
        	{printf("%02X", content[outstart+i]);}
	free(content);
	return 0;
}
 
int main(int argc, char **argv)
{
	int iter,length;
	if (argc < 2){puts("\n     __                    __            __ \n    / /_____ ____  _______/ /  ___ ____ / / \n   /  '_/ _ `/ _ \\(_-<___/ _ \\/ _ `(_-</ _ \\\n  /_/\\_\\\\_,_/\\___/___/  /_//_/\\_,_/___/_//_/\n\nusage: kd1 <filename> <iterations> <length>");return 0;}
	if (!argv[2]){ iter = 15;} else { iter = strtol(argv[2],NULL,10); } //  I've judged this to be a good same minimum number 
									//of starting iterations... testing will prove this, though!
	if (!argv[3]){length = 16;} else { length = strtol(argv[3],NULL,10);}
	int worked = concoquo(argv[1],iter,length);
	return 0;
}

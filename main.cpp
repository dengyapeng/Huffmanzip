#include<iostream>
#include<cstdio>
#include"Huffman.h"
using namespace std;

int main(int argc,char *argv[])
{
    if(argc!=4)
    {
        printf("compress example ./a.out -c inputfile outfile\n");
        printf("docompress example ./a.out -x inputfile outfile\n");

    }

    if(0==strcmp("-c",argv[1]))
    {
        Huffman h (argv[2],argv[3]);
        h.compress();
    }
    else if(0==strcmp("-x",argv[1]))
    {
        Huffman h(argv[2],argv[3]);
        h.decompress();
    }
    else{
        printf("unkonwn command\n");
    }
    return 0;
}

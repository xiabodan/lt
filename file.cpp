#include<stdio.h>
int main()
{
    FILE *fp;
    int c1;
    int filesize = 0;
    fp=fopen("./sar.308","r");
    while((c1=fgetc(fp))!=EOF){
	    int c2 = fgetc(fp);
	    int c3 = fgetc(fp);
	    int c4 = fgetc(fp);
	    if(0x08 != c4)
	    	filesize++;
	    else
	    	filesize = 0;
    }
    fclose(fp);
}

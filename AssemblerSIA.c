#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *ltrim(char *s) {
	while (*s == ' ' || *s == '\t') s++;
	return s;
}

char getRegister(char *text) {
	if (*text == 'r' || *text=='R') text++;
	return atoi(text);
}

int assembleLine(char *text, unsigned char* bytes) {
	text = ltrim(text);
	char *keyWord = strtok(text," ");
	if (strcmp("add",keyWord) == 0) {
		bytes[0] = 0x10;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}
    if (strcmp("subtract",keyWord) == 0) {
		bytes[0] = 0x50;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}

    if (strcmp("multiply",keyWord) == 0) {
		bytes[0] = 0x40;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}

    if (strcmp("divide",keyWord) == 0) {
		bytes[0] = 0x30;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}

    if (strcmp("and",keyWord) == 0) {
		bytes[0] = 0x20;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}

    if (strcmp("or",keyWord) == 0) {
		bytes[0] = 0x60;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4 | getRegister(strtok(NULL," "));
		return 2;
	}

    if (strncmp("halt",keyWord, 4) == 0) {
		bytes[0] = 0x00;
		bytes[1] = 0x00;
		return 2;
	}

    	if (strcmp("jump",keyWord) == 0) {
		bytes[0] = 0xC0;
        int x = atoi( strtok(NULL," "));
        bytes[0] |= (x >> 24);
		bytes[1] = x >> 16;
		bytes[2] = x >> 8;
		bytes[3] = x;
	    return 4;
	}

    	if (strcmp("leftshift",keyWord) == 0) {
		bytes[0] = 0x70;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," "));
		return 2;
	}

    	if (strcmp("rightshift",keyWord) == 0) {
		bytes[0] = 0x70;
        bytes[0] |= getRegister(strtok(NULL," "));
        int x = atoi(strtok(NULL, " "));
        bytes[1] = 0x20 | x;
		return 2;
	}

    	if (strcmp("addimmediate",keyWord) == 0) {
		bytes[0] = 0x90;
		bytes[0] |= getRegister(strtok(NULL," "));
        int x = atoi( strtok(NULL," "));
        bytes[1] = x;
		return 2;
	}

    	if (strcmp("interrupt",keyWord) == 0) {
		bytes[0] = 0x80;
		int x = getRegister(strtok(NULL," ")) ;
		bytes[0] |= x >> 8;
		bytes[1] = x;
		return 2;
	}

    	if (strcmp("load",keyWord) == 0) {
		bytes[0] = 0xE0;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4;
        int x = atoi( strtok(NULL," "));
		bytes[1] |= x;
		return 2;
	}

    	if (strcmp("store",keyWord) == 0) {
		bytes[0] = 0xF0;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4; 
        int x = atoi( strtok(NULL," "));
		bytes[1] |= x;
		return 2;
	}

    	if (strcmp("iterateover",keyWord) == 0) {
		bytes[0] = 0xd0;
        bytes[0] |= getRegister(strtok(NULL," "));;
        int x = atoi( strtok(NULL," "));
		bytes[1] = x;
        int x2 = atoi( strtok(NULL," "));
		bytes[2] = x2 >> 8;
		bytes[3] = x2;
	    return 4;
	}


	if (strcmp("branchifequal",keyWord) == 0) {
		
		bytes[0] = 0xA0;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4;
		int x = atoi( strtok(NULL," "));
		bytes[1] |= (x >> 16) & 0x0F;
		bytes[2] = x >> 8;
		bytes[3] = x;
	    return 4;
	}

    if (strcmp("branchifless",keyWord) == 0) {
		bytes[0] = 0xB0;
		bytes[0] |= getRegister(strtok(NULL," "));
		bytes[1] = getRegister(strtok(NULL," ")) << 4;
		int x = atoi( strtok(NULL," "));
		bytes[1] |= (x >> 16) & 0xF0;
		bytes[2] = x >> 8;
		bytes[3] = x;
	    return 4;
	}
}

int main(int argc, char **argv) {
	FILE *src = fopen(argv[1],"r");
	FILE *dst = fopen(argv[2],"w");
	while (!feof(src)) {
		unsigned char bytes[4];
		char line[1000];
		printf ("about to read\n");
		if (NULL != fgets(line, 1000, src)) {
			printf ("read: %s\n",line);
			int byteCount = assembleLine(line,bytes);
			fwrite(bytes,byteCount,1,dst);
		}
	}
	fclose(src);
	fclose(dst);
	return 0;
}
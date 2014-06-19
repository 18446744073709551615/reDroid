#include <stdio.h>
#include <string.h>
#include <alloca.h>

unsigned long
elf_hash(const unsigned char *name)
{
	unsigned long h = 0 , g ;
	while (*name)
	{
		h = ( h << 4 ) + * name ++ ;
		if (g = h & 0xf0000000) {
			h ^= g >> 24 ;
		}
		h &= ~g ;
	}
	return h ;
}
int isValid(unsigned char c) {
	return '0'>=c && c<='9' || 'A'<=c && c <= 'Z' || 'a'<=c && c <= 'z';
}
#define allocacpy(s) (strcpy(alloca(4+strlen(s)),s))
char* samehash(unsigned char*name) {
	unsigned long hash = elf_hash(name);
	int l = strlen(name);
	char cy = name[l-2], cz = name[l-1];
	int i,j;
	for (i=0x20; i<0x80; i++) if (isValid(i) && i != cy) {
		for (j=0x20; j<0x80; j++) if (isValid(j) && j != cz) {
			name[l-2] = i, name[l-1] = j;
			if (hash == elf_hash(name)) {
				return name;
			}
		}
	}
	return "";
}

/*
Why:
If you want to change a function name in an existing .so library,
you cannot use just any name, you need a name with the same elf hash value,
otherwise the name will not be found.

Usage:
    func
    -- output the elf hash value and a modified function name having the same elf hash

    func1 func2...
    -- check if func2 (and func3 etc. if present) have the same elf hash as func1
*/

int main(int argc, char**argv) {
	unsigned long firsthash = 0;
	int i;
	for (i=1; argv[i]; i++) {
		char* name = argv[i];
		printf("[%s]\n",name);
		unsigned long hash = elf_hash(name);
		if (i==1) {
			firsthash = hash;
			printf("0x%lx\n",hash);
		} else {
			printf("0x%lx %s\n",hash,hash==firsthash ? "OK" : "mismatch");
		}
	}
	if (argc == 2) {
		printf("[%s] -- same hash\n", samehash(allocacpy(argv[1])));
	}
	return 0;
}



#include <stdio.h>

#define VERSION "0.0.1"

void BasicFunc1() {
	printf ("Called basic function dll version %s\n", VERSION);
}

void BasicFunc2() {
	__asm__("jmp *%esp\n\t"
		"jmp *%eax\n\t"
		"pop %eax\n\t"
		"pop %eax\n\t"
		"ret");
}

void BasicFunc3() {
	__asm__("jmp *%esp\n\t"
		"jmp *%ecx\n\t"
		"pop %ebx\n\t"
		"pop %ebx\n\t"
		"ret");
}

void BasicFunc4() {
	__asm__("jmp *%esp\n\t"
		"jmp *%ebx\n\t"
		"pop %ebp\n\t"
		"pop %ebp\n\t"
		"ret");
}

void BasicFunc5() {
	__asm__("jmp *%esp\n\t"
		"jmp *%edi\n\t"
		"pop %ebx\n\t"
		"pop %ebx\n\t"
		"ret");
}

void BasicFunc6() {
	__asm__("jmp *%esp\n\t"
		"jmp *%edx\n\t"
		"pop %ecx\n\t"
		"pop %edx\n\t"
		"ret");
}

void BasicFunc7() {
	__asm__("jmp *%esp\n\t"
		"jmp *%esi\n\t"
		"pop %ecx\n\t"
		"pop %eax\n\t"
		"ret");
}


void BasicFunc8() {
	__asm__("jmp *%esp\n\t"
		"jmp *%ebp\n\t"
		"pop %eax\n\t"
		"pop %edx\n\t"
		"ret");
}


void BasicFunc9() {
	__asm__("jmp *%esp\n\t"
		"jmp *%esp\n\t"
		"jmp *-12(%esp)\n\t"
		"pop %ecx\n\t"
		"pop %ecx\n\t"
		"ret");
}


void BasicFunc10(char *Input) {
	char Buffer2S[140];
	strcpy(Buffer2S, Input);
}

void BasicFunc11(char *Input) {
	char Buffer2S[60];
	strcpy(Buffer2S, Input);
}


void BasicFunc12(char *Status, char *Input) {
	char Buffer2S[2000];
	strcpy(Buffer2S, Input);
	printf("%s", Status);
}

void BasicFunc13(char *Input) {
	char Buffer2S[2000];	
	strcpy(Buffer2S, Input);
}

void BasicFunc14(char *Input) {
	char Buffer2S[1000];
	strcpy(Buffer2S, Input);
}

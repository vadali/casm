/*
 * lists.h
 *
 *  Created on: 18 ���� 2012
 *      Author: ypsw
 */
#ifndef LISTS_H
#define LISTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "DoubleLinkedList.h"


typedef struct {
	int op_code;
	addrVal word[4];
	byte size;
	int offset;
} asm_node ;

typedef struct {
	int (*f)(Context* l, addrVal*, asm_node*, char*, int, char*);
	addrVal* into;
	Context* cont;
	int lineNumber;
	char origLine[100];
	char label[MAX_LABEL];
	asm_node* asmNode;
}defered_node;

typedef enum {
	STRING_KIND,
	DATA_KIND ,
	ASM_KIND ,
	NOT_INIT
} label_kind;

typedef struct {
	label_kind kind;
	union {
		struct {
			int* nums;
			int size;
		} data;
		char* str;
	}getData;
	int offset;
} data_node;

typedef struct {
	char label[MAX_LABEL];
	int offset;
	byte isExtern;
	byte isEntry;
	label_kind kind;
	char origLine[1000];
	int origLineNuber;
	union {
		asm_node* code;
		data_node data;
	}get;
}label_node;

typedef struct {
	char label[MAX_LABEL];
	int offset;
}entry_node;



#define LABEL(n) ((label_node*)n->data)
#define ASM(n) ((asm_node*)n->data)
#define DATA(n) ((data_node*)n->data)
#define DEFERRED(n) ((defered_node*)n->data)
#define ENTRY(n)  ((entry_node*)n->data)

#define INVOKE(n) DEFERRED(n)->f(((Context*)(DEFERRED(n)->cont)), DEFERRED(n)->into,DEFERRED(n)->asmNode, DEFERRED(n)->label, DEFERRED(n)->lineNumber, DEFERRED(n)->origLine)

node* newDeferedNode(int  (*f)(Context* l, addrVal*, asm_node*, char*, int, char*), Context* l, addrVal* into, asm_node* asmNode, char* label, int lineNumber, char* origLine);
node* newAsmNode();
node* newLabel(char* label , int origLineCount, char* origLine);
node* newExtern(char* label, int origLineCount, char* origLine);
node* newEntry(char* label, int origLineCount, char* origLine);
node* newEntryWord(char* label, int offset);
void writeEntry(list* l, FILE *f) ;


int findLabelText(node* n, void* label);
int findLabelEntry(node* n, void* label);
int findLabelExtern(node* n, void* label);
int findLabelEntryText(node* n, void* label);
int computeAsmOffset(list* l, int initial);
int computeLabelOffset(list* l, int lastAsmOffset);
int execDeffered(list* l);
void printAsm(list* l);
void printData(list* l);
void writeAsm(Context* c, FILE *f);


#endif

/*
 * gendsl.h
 *
 *  Created on: Mar 17, 2012
 *      Author: shlomi
 */

#ifndef GENDSL_H_
#define GENDSL_H_

#include "commons.h"
#include "asmdsl.h"
#include "lists.h"
#include "DoubleLinkedList.h"

void deferLabelDistanceResolution(list* l, int* into, char* label);
void deferLabelAddrResolution(list* l, int* into, char* label);

#define REGISTER(T) \
	code.bit.T ## Kind = theOperand->kind; /* REGISTER */ \
	code.bit.T ## Reg  = theOperand->get.reg;

#define USE_EXTRA_WORD size++;theWord++;

#define CONSTANT(T)  \
	code.bit.T ## Kind = theOperand->kind; /* CONSTANT */\
	code.bit.T ## Reg  = 0;\
	*theWord=theOperand->get.constant; \
	USE_EXTRA_WORD

#define GET_LABEL_OFFSET { \
		node* label = find(context->allLabels, findLabelText, theOperand->get.direct); /* GET_LABEL_OFFSET */ \
		if (label == NULL) { \
			context->deferred = append(context->deferred, newDeferedNode(deferLabelAddrResolution,&context->allLabels, theWord, theOperand->get.direct)); \
		} else *theWord = LABEL(label)->offset; \
	}


#define DIRECT_LABEL(T) \
	code.bit.T ## Kind = theOperand->kind; /* DIRECT_LABEL*/ \
	code.bit.T ## Reg  = 0;\
	GET_LABEL_OFFSET;\
	USE_EXTRA_WORD

#define MOV(OP1, OP2) { \
	OpCode code;\
	Operand* theOperand;\
	int *theWord;\
	node* n;\
	byte size=1; \
	code.code=0;\
	code.bit.op=0;\
	theOperand=&operand1;\
	n  = newAsmNode(); \
	theWord = ASM(n)->word;\
	OP1; \
	theOperand=&operand2;\
	OP2; \
	\
	context->codeList = append(context->codeList, n);\
	ASM(n)->op_code = code.code; \
	ASM(n)->size=size; \
}

void init(void(*fs[5][4])(Context*, Operand, Operand, char*), void(*f)(Context*, Operand, Operand, char*), addr from, addr to);

#define GEN(name) void name(Context* context, Operand operand1, Operand operand2, char* label)
#define DEFAULT(name) void name(Operand operand1, Operand operand2, char* label)
#define ASSIGN2(mat, name, code, from, to) GEN(name##from##to) code \
		init(movs, name##from##to, from, to);



void genExtern(char* label, Operand oper, Context* context);
void genEntry(char* label, Operand oper, Context* context);
void genData(char* label, int* nums, int count, Context* context);
#endif /* GENDSL_H_ */

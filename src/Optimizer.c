#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

int main() {
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
	/* YOUR CODE GOES HERE */

	Instruction* load1 = head; // access head without changing, first loadi
	Instruction* load2 = load1->next; //points to second loadi
	Instruction* op = load2->next; //points to op instruction
	int optimized = 0; //keep track of whether optimization is done, 1 = done 0 = failed
	int c1 = 0, c2 = 0, c3 = 0; //constants for load1 load2, and the optimized load

	//  PATTERN
	//	LOADI Rx #c1
	//	LOADI Ry #c2
	//	op Rz Rx Ry

	//if no op exists, pattern doesn't fit anymore
	while (op != NULL) {

		OpCode operation = op->opcode;
		if ((load1->opcode == LOADI && load2->opcode == LOADI)
				&& (operation == ADD || operation == SUB || operation == MUL)) {

			//	LOADI Rx #c1
			c1 = load1->field2;
			//	LOADI Ry #c2
			c2 = load2->field2;
			//  LOADI Rz #c1
			load1->field1 = op->field1;

			switch (operation) {
			case ADD:
				c3 = c1 + c2;
				break;
			case SUB:
				c3 = c1 - c2;
				break;
			case MUL:
				c3 = c1 * c2;
				break;
			default:
				ERROR("Op %s unknown\n", operation);
				exit(EXIT_FAILURE);
			}

			//LOADI Rz #c3
			load1->field2 = c3;

			//optimization done for these instructions
			optimized = 1;
		}

		if (optimized == 1) {
			// We no longer need these two so we must free memory
			Instruction* temp_load = load2;
			Instruction* temp_op = op;
			//skip over the two nodes
			load1->next = op->next;
			//free nodes no longer in instruction list
			free(temp_load);
			free(temp_op);

			//load next instruction set, start at top
			optimized = 0;
			load1 = head;
			load2 = load1->next;
			op = load2->next;
		} else {
			//load next instruction set if no optimization, keep moving down
			load1 = load1->next;
			load2 = load2->next;
			op = op->next;
		}
	}

	if (head) {
		PrintInstructionList(stdout, head);
		DestroyInstructionList(head);
	}
	return EXIT_SUCCESS;
}


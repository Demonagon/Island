#include "util/util_debug.h"
#include <stdio.h>

int indent = 0;

void printIndent(void) {
	for(int k = 0; k < indent; k++)
		printf("\t");
}

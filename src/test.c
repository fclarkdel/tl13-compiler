#include <stdio.h>
#include <stdlib.h>

int read_int() {
	int value;

	if (scanf("%d", &value) != 1)
		exit(1);

	return value;
}

void write_int(int value) {
	printf("%d", value);
}

int main() {
	int SMALLER = 0;
	int BIGGER = 0;
	int TEMP = 0;
	BIGGER =  read_int();
	SMALLER =  read_int();
	if (SMALLER > BIGGER) {
		TEMP = SMALLER;
		SMALLER = BIGGER;
		BIGGER = TEMP;
	}
	while (SMALLER > 0) {
		BIGGER = BIGGER - SMALLER;
		if (SMALLER > BIGGER) {
			TEMP = SMALLER;
			SMALLER = BIGGER;
			BIGGER = TEMP;
		}
	}
	write_int(BIGGER);
	return 0;
}
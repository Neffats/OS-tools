#include <stdio.h>
#include "wish.h"

int main(int argc, char *argv[]){
	//char* path = "/usr/bin/";
	int result = 0;
	
	switch(argc) {
		// No arg given so run in interactive mode.
		case 1:
			result = interactive_mode();
		case 2:
			result = batch_mode(argv[1]);
	}

	if (result != 0) {
		printf("\nfailed with: %d\n", result);
		return 1;
	}
}

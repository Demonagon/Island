#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <time.h>
#include <unistd.h> 
#include "util/util_math.h"
#include "core/global.h"
#include "core/update.h"
#include "util/list.h"
#include "core/grid.h"
#include "core/main_memory.h"
#include "util/sorted_tree.h"
#include "build/gl_graphics.h"

int programIsRunning;

void * updateLoop(void * vargp) {
	int frames = 20;
	double frame_time = 1.0 / frames;
	long int nano_frame_time = frame_time * 1000000000;

	while(programIsRunning) {
		printf("[%5ld] :\n", UPDATE_REGISTER.clock);

		updateRegisterUpdate(&UPDATE_REGISTER);

		nanosleep((const struct timespec[]){{0, nano_frame_time}}, NULL);
	}

	return NULL;
}

void mainGlGraphicsTest(int argc, char ** argv) {
	glArtistInit(&argc, argv, &openGLArtist, 500, 500);
	
	globalInit(&openGLArtist, glArtistFactory);

	//TODO ajouter des objects

	programIsRunning = 1;

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, updateLoop, NULL);

	glutMainLoop();

	programIsRunning = 0;

	pthread_join(thread_id, NULL);

	globalFree();
}

int main(int argc, char **argv) {

	mainGlGraphicsTest(argc, argv);

	return 0;
}

/*
 * machineController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "defines.h"
#include "timer.h"
#include "hardwareController.h"
#include "machineController.h"

static TIMER timer;
static Mix_Music *music; /* Pointer to our music, in memory	*/
static int isMachineControllerSetUp = FALSE;

typedef struct {
	enum Ingredient ing;
	char file[50];
} Sounds;

/* Define soundfiles */
Sounds ingredientSounds[] = {
	{ .ing = ingredient_coffee, .file = "/usr/local/share/yacm/coffeeDelivery.mp3" },
	{ .ing = ingredient_milk, .file = "/usr/local/share/yacm/milkDelivery.mp3" }
};
int soundsCount = 2;

int setUpMachineController(void)
{

	// check if machine controller is already set up:
	if (isMachineControllerSetUp) {
		return FALSE;
	}

	// check if hardware is already initialized:
	if (!getHardwareSetUpState()) {
		if (!setUpHardwareController()) {
			return FALSE;
		}
	}

	isMachineControllerSetUp = TRUE;
	return TRUE;
}

int tearDownMachineController(void) {
	// check if machine controller was already torn down:
	if (!isMachineControllerSetUp) {
		return FALSE;
	}

	isMachineControllerSetUp = FALSE;
	return TRUE;
}

static int playSound(enum Ingredient ing)
{
	int audio_rate = 44100; /* Frequency of audio playback in [Hz]	*/
	Uint16 audio_format = AUDIO_S16SYS; /* Format of the audio we're playing	*/
	int audio_channels = 2; /* 2 channels = stereo			*/
	int audio_buffers = 4096; /* Size of the audio buffers in memory	*/
	char musicFile[50];

	/* Get path of ingredient specify sound file */
	for (int i = 0; i < soundsCount; i++) {
		if (ingredientSounds[i].ing == ing) {
			strcpy(musicFile, ingredientSounds[i].file);
		}
	}

	/* Initialize SDL audio	*/
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return FALSE;
	}

	/* Initialize SDL_mixer with our chosen audio settings */
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)
			!= 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		return FALSE;
	}

	/* Get the sound file */
	music = Mix_LoadMUS(musicFile);
	if (music == NULL) {
		printf("Unable to load music file: %s\n", Mix_GetError());
		return FALSE;
	}

	/* Play the music!	*/
	if (Mix_PlayMusic(music, 0) == -1) {
		printf("Unable to play music file: %s\n", Mix_GetError());
		return FALSE;
	}
	return TRUE;
}

static int stopSound() {
	/* Release the memory allocated to our music	*/
	Mix_HaltMusic();
	Mix_FreeMusic(music);

	/* Clean up SDL_mixer and SDL */
	Mix_CloseAudio();
	SDL_Quit();

	/* Return success!	*/
	return TRUE;
}

int startMachine(enum Ingredient ing, unsigned int time)
{
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	if (ing == ingredient_coffee) {
		printf("Delivering coffee...\n");
		playSound(ing);
	} else if (ing == ingredient_milk) {
		printf("Delivering milk...\n");
		playSound(ing);
	} else {
		printf("Unknown ingredient selected!\n");
	}
	timer = setUpTimer(time);
	return TRUE;
}

int stopMachine(void)
{
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	if (timer) {
		abortTimer(timer);
		timer = NULL;
	}
	stopSound();
	return TRUE;
}

int machineRunning(void)
{
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	if (timer) {
		if (!isTimerElapsed(timer)) {
			return TRUE;
		} else {
			stopSound();
			timer = NULL;
		}
	}
	return FALSE;
}

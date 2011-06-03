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
static Mix_Music *sound; /* Pointer to our sound, in memory	*/
static int isMachineControllerSetUp = FALSE;

/**
 * Structure defining ingredient specific soundfile
 */
typedef struct {
	enum Ingredient ing; /**< Ingredient */
	char file[50];       /**< Filename   */
} SoundFile;

/**
 *  Ingredient specific soundfiles
 */
SoundFile ingredientSounds[] = {
	{ .ing = ingredient_coffee, .file = "/usr/local/share/yacm/coffeeDelivery.mp3" },
	{ .ing = ingredient_milk, .file = "/usr/local/share/yacm/milkDelivery.mp3" }
};
int soundsCount = 2;

/**
 * @copydoc setUpMachineController
 */
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

	// Frequency of audio playback in [Hz]
	int audioRate = 44100;
	// Format of the audio we're playing
	Uint16 audioFormat = AUDIO_S16SYS;
	// 2 channels = stereo
	int audioChannels = 2;
	// Size of the audio buffers in memory
	int audioBuffers = 4096;

	// Initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return FALSE;
	}

	// Initialize SDL_mixer with our chosen audio settings
	if (Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		return FALSE;
	}

	isMachineControllerSetUp = TRUE;
	return TRUE;
}

/**
 * @copydoc tearDownMachineController
 */
int tearDownMachineController(void) {
	// check if machine controller was already torn down:
	if (!isMachineControllerSetUp) {
		return FALSE;
	}

	// Clean up SDL_mixer and SDL
	Mix_CloseAudio();
	SDL_Quit();

	isMachineControllerSetUp = FALSE;
	return TRUE;
}

/**
 * @copydoc playSound
 */
static int playSound(enum Ingredient ing)
{
	// char buffer to hold the sound filename
	char soundFile[50];

	// Get path of the ingredient specific sound file
	for (int i = 0; i < soundsCount; i++) {
		if (ingredientSounds[i].ing == ing) {
			strcpy(soundFile, ingredientSounds[i].file);
		}
	}

	// Load the sound file
	sound = Mix_LoadMUS(soundFile);
	if (sound == NULL) {
		printf("Unable to load sound file: %s\n", Mix_GetError());
		return FALSE;
	}

	// Play the sound
	if (Mix_PlayMusic(sound, 0) == -1) {
		printf("Unable to play sound file: %s\n", Mix_GetError());
		return FALSE;
	}
	return TRUE;
}

/**
 * @copydoc stopSound
 */
static int stopSound()
{
	// Stop the sound
	Mix_HaltMusic();
	// Release the memory allocated for the sound file
	Mix_FreeMusic(sound);

	// Return success
	return TRUE;
}

/**
 * @copydoc startMachine
 */
int startMachine(enum Ingredient ing, unsigned int time)
{
	// check if the machine controller is initialized:
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	// print a ingredient specific message:
	if (ing == ingredient_coffee) {
		printf("Delivering coffee...\n");
	} else if (ing == ingredient_milk) {
		printf("Delivering milk...\n");
	} else {
		printf("Unknown ingredient selected!\n");
		return FALSE;
	}
	// play ingredient specific sound:
	playSound(ing);
	// start timer:
	timer = setUpTimer(time);
	return TRUE;
}

int stopMachine(void)
{
	// check if machine was initialized
	if (!isMachineControllerSetUp) {
		return FALSE;
	}

	// if timer is still running stop it and stop sound:
	if (timer) {
		stopSound();
		abortTimer(timer);
		timer = NULL;
	}
	return TRUE;
}

/**
 * @copydoc machineRunning
 */
int machineRunning(void)
{
	// check if the machine controller is initialized
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	// check for a timer pointer:
	if (timer) {
		// check if timer is still running:
		if (!isTimerElapsed(timer)) {
			return TRUE;
		} else {
			// stop sound and set timer pointer to NULL:
			stopSound();
			timer = NULL;
		}
	}
	return FALSE;
}

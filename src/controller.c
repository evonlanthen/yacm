/**
 * @mainpage yacm - yet another coffee maker
 * @section intro Introduction
 *
 * This software implements the logic for a coffee maker.
 *
 * @section usage Usage
 * @subsection step1 Step 1: Compile project
 * @code
 * # for ORCHID:
 * <user>@<host> $ make orchid
 * # for CARME:
 * <user>@<host> $ make carme
 * @endcode
 * @subsection step2 Step 2: Install files
 * @code
 * # for ORCHID:
 * <user>@<host> $ sudo make orchid-install
 * # for CARME:
 * <user>@<host> $ sudo make carme-install
 * @endcode
 * @subsection step3 Step 3: Start program
 * @code
 * root@<target> # /usr/local/bin/yacm
 * @endcode
 *
 * @section authors Authors
 * @arg Toni Baumann (bauma12@bfh.ch)
 * @arg Ronny Stauffer (staur3@bfh.ch)
 * @arg Elmar Vonlanthen (vonle1@bfh.ch)
 *
 * @section knownProblems Known Problems
 * @arg Sound is not working on CARME board (mplayer doesn't work too!)
 * @arg Blinking of leds with same frequency is not synchronized
 * @arg Vncserver (fbvncserver &) on ORCHID board should be started manually before executing this application
 *
 * @section architecture Architecture
 * @image html "architecture.jpg"
 * @section stateEvent State Event Diagramm
 * @image html "coffee_maker_state.jpg"
 * @section model Model
 * @image html "model.jpg"
 * @section makingProcess Making Process
 * @image html "coffee_making_process.jpg"
 *
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the application.
 *
 * Initializes and controls the application. Contains the entry point.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "defines.h"
#include "logic.h"
#include "userInterface.h"
#include "hardwareController.h"
#include "machineController.h"
#include "inputController.h"
#include "ledController.h"
#include "sensorController.h"
#include "timer.h"

static volatile int ctrlCPressed = FALSE;

static void sigCtrlC(int sig)
{
	ctrlCPressed = TRUE;
	(void) signal(SIGINT, SIG_DFL);
}

static void setUpSubsystems();
static void tearDownSubsystems();

#ifndef TEST
/**
 * The entry point of the application.
 */
int main(int argc, char* argv[]) {
	setUpSubsystems();

	// set CTRL-C interrupt handler:
	(void) signal(SIGINT, sigCtrlC);

	while (!ctrlCPressed) {
		// Propagate "heartbeat"
		runUserInterface();
		runBusinessLogic();
	}

	printf("\nShutting down system...\n");
	tearDownSubsystems();
#ifdef DEBUG
	printf("Shutdown successful, terminating!\n");
#endif
	exit(0);
}

void setUpSubsystems() {
	setUpHardwareController();
	setUpMachineController();
	setUpInputController();
	setUpLedController();
	setUpSensorController();
	setUpBusinessLogic();
	setUpDisplay();
}

void tearDownSubsystems() {
	tearDownDisplay();
	tearDownBusinessLogic();
	tearDownSensorController();
	tearDownLedController();
	tearDownInputController();
	tearDownMachineController();
	tearDownHardwareController();
}

#endif

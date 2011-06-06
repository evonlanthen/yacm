/**
 * @mainpage yacm - yet another coffee maker
 * @section intro Introduction
 *
 * This software implements the logic for a coffee maker.
 *
 * @section usage Usage
 * @subsection step1 Step 1: Compile project
 * for ORCHID:
 * @code
 * <user>@<host> $ make orchid
 * @endcode
 * for CARME:
 * @code
 * <user>@<host> $ make carme
 * @endcode
 * @subsection step2 Step 2: Install files
 * for ORCHID:
 * @code
 * <user>@<host> $ sudo make orchid-install
 * @endcode
 * for CARME:
 * @code
 * <user>@<host> $ sudo make carme-install
 * @endcode
 * @subsection step3 Step 3: Start program
 * for ORCHID:
 * @code
 * root@<target> # fbvncserver &
 * root@<target> # /usr/local/bin/yacm
 * <user>@<host> $ xtightvncviewer <target-ip-address>
 * @endcode
 * for CARME:
 * @code
 * root@<target> # /usr/local/bin/yacm
 * @endcode
 * @subsection step4 Step 4: Simulate sensors
 * for ORCHID:
 * @arg @b S3: Coffee tank is empty
 * @arg @b S4: Milk tank is empty
 *
 * for CARME:
 * @arg @b S6: Coffee tank is empty
 * @arg @b S7: Milk tank is empty
 * @endverbatim
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
 * @section stateEvent State Event Diagram
 * @image html "coffee_maker_state.jpg"
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

#ifdef DEBUG
	printf("\nShutting down system...\n");
#endif
	tearDownSubsystems();
#ifdef DEBUG
	printf("Shutdown successful, terminating!\n");
#endif
	exit(0);
}

/**
 * Sets up all subsystems.
 */
void setUpSubsystems() {
	setUpHardwareController();
	setUpMachineController();
	setUpInputController();
	setUpLedController();
	setUpSensorController();
	setUpBusinessLogic();
	setUpDisplay();
}

/**
 * Tears down all subsystems.
 */
void tearDownSubsystems() {
	tearDownDisplay();
	tearDownBusinessLogic();
	tearDownSensorController();
	tearDownLedController();
	tearDownInputController();
	tearDownMachineController();
	tearDownHardwareController();
}

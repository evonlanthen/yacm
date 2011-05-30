/*
 * @file   uiViewOff.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Off view for userInterface.c
 */

#define MWINCLUDECOLORS
#include "defines.h"
#include "userInterface.h"
#include "nano-X.h"

static void run(void) {

}

static void activate(void) {
	struct DisplayState *displaystate = getDisplayState();
	displaystate->gContextID = GrNewGC();
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate->gContextID, YELLOW);
	GrSetGCUseBackground(displaystate->gContextID, GR_FALSE);
	/* Select fonts */
	displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate->gContextID, displaystate->font);
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Power Off!", -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate->font);

}

static void deactivate(void) {

}

static void update(void) {

}


struct callViewActions getViewOffActions(void) {
	struct callViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}



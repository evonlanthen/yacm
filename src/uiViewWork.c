/*
 * @file   uiViewWork.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Work view for userInterface.c
 */

#include "userInterface.h"

static void run(void) {

}

static void activate(void) {

}

static void deactivate(void) {

}

static void update(void) {

}


callViewAction getViewWorkActionRun(void) {
	return &run;
}

callViewAction getViewWorkActionActivate(void) {
	return &activate;
}

callViewAction getViewWorkActionDeactivate(void) {
	return &deactivate;
}

callViewAction getViewWorkActionUpdate(void) {
	return &update;
}


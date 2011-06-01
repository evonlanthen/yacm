/**
 * Initialize memory mapping
 *
 * @file    mmap.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 27, 2011
 */

#ifndef MMAP_H_
#define MMAP_H_

int setUpMmap(void);

int tearDownMmap(void);

int getMmapSetUpState(void);

#endif /* MMAP_H_ */

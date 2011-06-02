/**
 * Definitions for board CARME
 *
 * @file    carme.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef CARME_H_
#define CARME_H_

/****************************************************************************
 * Definitions General Purpose I/O
 ****************************************************************************/

#define GPLR0	0x40E00000  /* GPIO Pin-Level Register GPIO<31:0> */
#define GPLR1	0x40E00004  /* GPIO Pin-Level Register GPIO<63:32> */
#define GPLR2	0x40E00008  /* GPIO Pin-Level Register GPIO<95:64> */

#define GPDR0	0x40E0000C  /* GPIO Pin Direction Register GPIO<31:0> */
#define GPDR1	0x40E00010  /* GPIO Pin Direction Register GPIO<63:32> */
#define GPDR2	0x40E00014  /* GPIO Pin Direction Register GPIO<80:64> */

#define GPSR0	0x40E00018  /* GPIO Pin Output Set Register GPIO<31:0> */
#define GPSR1	0x40E0001C  /* GPIO Pin Output Set Register GPIO<63:32> */
#define GPSR2	0x40E00020  /* GPIO Pin Output Set Register GPIO<80:64> */

#define GPCR0	0x40E00024  /* GPIO Pin Output Clear Register GPIO<31:0> */
#define GPCR1	0x40E00028  /* GPIO Pin Output Clear Register GPIO <63:32> */
#define GPCR2	0x40E0002C  /* GPIO Pin Output Clear Register GPIO <80:64> */

#define GRER0	0x40E00030  /* GPIO Rising-Edge Detect Register GPIO<31:0> */
#define GRER1	0x40E00034  /* GPIO Rising-Edge Detect Register GPIO<63:32> */
#define GRER2	0x40E00038  /* GPIO Rising-Edge Detect Register GPIO<80:64> */

#define GFER0	0x40E0003C  /* GPIO Falling-Edge Detect Register GPIO<31:0> */
#define GFER1	0x40E00040  /* GPIO Falling-Edge Detect Register GPIO<63:32> */
#define GFER2	0x40E00044  /* GPIO Falling-Edge Detect Register GPIO<80:64> */

#define GEDR0	0x40E00048  /* GPIO Edge Detect Status Register GPIO<31:0> */
#define GEDR1	0x40E0004C  /* GPIO Edge Detect Status Register GPIO<63:32> */
#define GEDR2	0x40E00050  /* GPIO Edge Detect Status Register GPIO<80:64> */

#define GAFR0_L	0x40E00054  /* GPIO Alternate Function Select Register GPIO<15:0> */
#define GAFR0_U	0x40E00058  /* GPIO Alternate Function Select Register GPIO<31:16> */
#define GAFR1_L	0x40E0005C  /* GPIO Alternate Function Select Register GPIO<47:32> */
#define GAFR1_U	0x40E00060  /* GPIO Alternate Function Select Register GPIO<63:48> */
#define GAFR2_L	0x40E00064  /* GPIO Alternate Function Select Register GPIO<79:64> */
#define GAFR2_U	0x40E00068  /* GPIO Alternate Function Select Register GPIO<95-80> */
#define GAFR3_L	0x40E0006C  /* GPIO Alternate Function Select Register GPIO<111:96> */
#define GAFR3_U	0x40E00070  /* GPIO Alternate Function Select Register GPIO<127:112> */

#define GPLR3	0x40E00100  /* GPIO Pin-Level Register GPIO<127:96> */
#define GPDR3	0x40E0010C  /* GPIO Pin Direction Register GPIO<127:96> */
#define GPSR3	0x40E00118  /* GPIO Pin Output Set Register GPIO<127:96> */
#define GPCR3	0x40E00124  /* GPIO Pin Output Clear Register GPIO<127:96> */
#define GRER3	0x40E00130  /* GPIO Rising-Edge Detect Register GPIO<127:96> */
#define GFER3	0x40E0013C  /* GPIO Falling-Edge Detect Register GPIO<127:96> */
#define GEDR3	0x40E00148  /* GPIO Edge Detect Status Register GPIO<127:96> */

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define IN 		250
#define OUT 	251
#define GPIO 	0
#define AF0 	0
#define AF1 	1
#define AF2 	2
#define AF3 	3
#define SET 	252
#define CLEAR 	253

#define BIT_SIZE_32	32
#define BIT_SIZE_16	16

#define	GPIO_PIN_35	35
#define	GPIO_PIN_37	37
#define	GPIO_PIN_36	36
#define	GPIO_PIN_79	79
#define	GPIO_PIN_15	15
#define	GPIO_PIN_80	80
#define	GPIO_PIN_52	52
#define	GPIO_PIN_19	19

#define MAP_SIZE		(6*4096)		/* Six pages */
#define MAP_MASK		(MAP_SIZE - 1)

#define IO_BASE			(0xc000000 )	/* IO base in physical memory */
#define LED_OFFSET		(0x3000)		/* Offset of led register in IO space */
#define SWITCH_OFFSET	(0x3200)		/* Offset of switch register in IO space */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

void  GPIO_init(void);
void  GPIO_write_led(UINT8 pattern);
UINT8 GPIO_read_switch(void);
UINT8 GPIO_read_button(void);

#endif /* CARME_H_ */

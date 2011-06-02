/*
 ***************************************************************************
 * \brief   GPIO dir/set/clear functions for the Orchid-IO Board
 * \file    gpio.c
 * \version 1.0
 * \date    28.09.2008
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.08.07   Initial release
 * \remark  02.06.2011             Support for buttons of CARME Board added
 *
 ***************************************************************************
 */

#include "defines.h"
#include "types.h"
#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#endif

/*
 ***************************************************************************
 * Public Variables
 ***************************************************************************
 */

extern void *mmap_base;

/*
 ***************************************************************************
 * Module Variables
 ***************************************************************************
 */

void *regaddr;

/*
 ***************************************************************************
 * Set register address
 ***************************************************************************
 */

static void GPIO_putmem(UINT32 addr, UINT32 val)
 {
   regaddr = (void*) ((UINT32) mmap_base2 + (addr & MAP_MASK));
   *(UINT32*) regaddr = val;
 }

/*
 ***************************************************************************
 * Get register address
 ***************************************************************************
 */

static UINT32 GPIO_getmem(UINT32 addr)
 {
    UINT32 val;

    regaddr = (void*) ((UINT32) mmap_base2 + (addr & MAP_MASK));
    val = *(UINT32*) regaddr;
    return val;
 }

/*
 ***************************************************************************
 * Set a GPIO to a logic 1
 ***************************************************************************
 */

static void GPIO_set(UINT32 gpio)
 {
    UINT32 pos;
    UINT32 bit = 1;

    pos = gpio / BIT_SIZE_32;
    bit <<= gpio % BIT_SIZE_32;
    if (gpio > 84)
     GPIO_putmem(GPSR3, bit);
    else
     GPIO_putmem(GPSR0 + (pos * 4), bit);
 }

/*
 ***************************************************************************
 *  Set a GPIO to a logic 0
 ***************************************************************************
 */

static void GPIO_clear(UINT32 gpio)
 {
    UINT32 pos;
    UINT32 bit = 1;

    pos = gpio / BIT_SIZE_32;
    bit <<= gpio % BIT_SIZE_32;
    if (gpio > 84)
     GPIO_putmem(GPCR3, bit);
    else
     GPIO_putmem(GPCR0 + (pos * 4), bit);
 }

/*
 ***************************************************************************
 * Get the logic level of GPIO
 ***************************************************************************
 */

static UINT32 GPIO_status(UINT32 gpio)
 {
    UINT32 pos;
    UINT32 bit = 1;
    UINT32 data;

    pos = gpio / BIT_SIZE_32;
    bit <<= gpio % BIT_SIZE_32;
    if (gpio > 84)
     data = GPIO_getmem(GPLR3);
    else
     data = GPIO_getmem(GPLR0 + (pos * 4));
    data &= bit;
    if (data == 0)
      return(0);
    else
      return(1);
 }

/*
 ***************************************************************************
 * Set the dircetion of a GPIO
 ***************************************************************************
 */

static void GPIO_direction(UINT32 gpio, UINT32 dir)
 {
    UINT32 pos;
    UINT32 bit = 1;
    UINT32 data;

    pos = gpio / BIT_SIZE_32;
    bit <<= gpio % BIT_SIZE_32;
    if (gpio > 84)
     data = GPIO_getmem(GPDR3);
    else
     data = GPIO_getmem(GPDR0 + (pos * 4));
    data &= ~bit;
    if (dir == OUT)
      data |= bit;
    if (gpio > 84)
     GPIO_putmem(GPDR3, data);
    else
     GPIO_putmem(GPDR0 + (pos * 4), data);
 }

/*
 ***************************************************************************
 * Set as a GPIO funct
 ***************************************************************************
 */

static void GPIO_function(UINT32 gpio, UINT32 fun)
 {
    UINT32 pos;
    UINT32 bit = 3;
    UINT32 data;

    pos = gpio / BIT_SIZE_16;
    bit <<= (gpio % BIT_SIZE_16) * 2;
    fun <<= (gpio % BIT_SIZE_16) * 2;
    data = GPIO_getmem(GAFR0_L + (pos * 4));
    data &= ~bit;
    data |= fun;
    GPIO_putmem(GAFR0_L + (pos * 4), data);
 }

/*
 ***************************************************************************
 * Init the GPIOs
 ***************************************************************************
 */

void GPIO_init(void)
 {
#ifdef CARME
  /* Initialize buttons */
  GPIO_function(99, GPIO);
  GPIO_function(100, GPIO);
  GPIO_function(101, GPIO);
  GPIO_function(102, GPIO);
  GPIO_direction(99, IN);
  GPIO_direction(100, IN);
  GPIO_direction(101, IN);
  GPIO_direction(102, IN);
#elif defined(ORCHID)
  /* Set GPIO functions of the LED and 7-Segment port first */
  GPIO_function(35, GPIO);
  GPIO_function(37, GPIO);
  GPIO_function(36, GPIO);
  GPIO_function(79, GPIO);
  GPIO_function(15, GPIO);
  GPIO_function(80, GPIO);
  GPIO_function(52, GPIO);
  GPIO_function(19, GPIO);

  /* Set GPIO functions 7-Segment enable */
  GPIO_function(23, GPIO);
  GPIO_function(24, GPIO);
  GPIO_function(25, GPIO);
  GPIO_function(26, GPIO);

  /* Buttons and Switches */
  GPIO_function(12, GPIO);
  GPIO_function(11, GPIO);
  GPIO_function(16, GPIO);
  GPIO_function(17, GPIO);
  GPIO_function(117, GPIO);
  GPIO_function(118, GPIO);

  /* Then set the direction for the LED and 7-Segment port */
  GPIO_direction(35, OUT);
  GPIO_direction(37, OUT);
  GPIO_direction(36, OUT);
  GPIO_direction(79, OUT);
  GPIO_direction(15, OUT);
  GPIO_direction(80, OUT);
  GPIO_direction(52, OUT);
  GPIO_direction(19, OUT);

  /* Enable 7-Segment Digits */
  GPIO_direction(23, OUT);
  GPIO_direction(24, OUT);
  GPIO_direction(25, OUT);
  GPIO_direction(26, OUT);

  /* Turn all off */
  GPIO_clear(35);
  GPIO_clear(37);
  GPIO_clear(36);
  GPIO_clear(79);
  GPIO_clear(15);
  GPIO_clear(80);
  GPIO_clear(52);
  GPIO_clear(19);

  GPIO_clear(23);
  GPIO_clear(24);
  GPIO_clear(25);
  GPIO_clear(26);

  /* Buttons and Switches */
  GPIO_direction(12, IN);
  GPIO_direction(11, IN);
  GPIO_direction(17, IN);
  GPIO_direction(16, IN);

  /* Mux selection */
  GPIO_direction(117, OUT);
  GPIO_direction(118, OUT);

  /* Select LED */
  GPIO_set(118);
#endif
 }

/*
 ***************************************************************************
 * Write LED pattern
 ***************************************************************************
 */

void GPIO_write_led(UINT8 pattern)
 {
  if (pattern & 0x01)
   GPIO_set(35);
  else
   GPIO_clear(35);

  if (pattern & 0x02)
   GPIO_set(37);
  else
   GPIO_clear(37);

  if (pattern & 0x04)
   GPIO_set(36);
  else
   GPIO_clear(36);

  if (pattern & 0x08)
   GPIO_set(79);
  else
   GPIO_clear(79);

  if (pattern & 0x10)
   GPIO_set(15);
  else
   GPIO_clear(15);

  if (pattern & 0x20)
   GPIO_set(80);
  else
   GPIO_clear(80);

  if (pattern & 0x40)
   GPIO_set(52);
  else
   GPIO_clear(52);

  if (pattern & 0x80)
   GPIO_set(19);
  else
   GPIO_clear(19);
 }

/*
 ***************************************************************************
 * Read the switches
 ***************************************************************************
 */

UINT8 GPIO_read_switch(void)
 {
  UINT8 sw1, sw2, sw3, sw4;

  GPIO_set(117);
  sw1 = GPIO_status(12);
  sw2 = GPIO_status(11);
  sw3 = GPIO_status(17);
  sw4 = GPIO_status(16);
  return ((sw4 << 3) | (sw3 << 2) | (sw2 << 1) | (sw1));
 }

/*
 ***************************************************************************
 * Read the buttons
 ***************************************************************************
 */

UINT8 GPIO_read_button(void)
 {
  UINT8 bt1, bt2, bt3, bt4;

#ifdef CARME
  bt1 = GPIO_status(99);
  bt2 = GPIO_status(100);
  bt3 = GPIO_status(101);
  bt4 = GPIO_status(102);
#elif defined(ORCHID)
  GPIO_clear(117);
  bt1 = GPIO_status(12);
  bt2 = GPIO_status(11);
  bt3 = GPIO_status(17);
  bt4 = GPIO_status(16);
#endif

  return ((bt4 << 3) | (bt3 << 2) | (bt2 << 1) | (bt1));
 }

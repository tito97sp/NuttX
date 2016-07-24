/********************************************************************************************
 * include/nuttx/ioexpander/gpio.h
 *
 *   Copyright (C) 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ********************************************************************************************/

#ifndef __INCLUDE_NUTTX_IOEXPANDER_GPIO_H
#define __INCLUDE_NUTTX_IOEXPANDER_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/fs/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Command:     GPIOC_WRITE
 * Description: Set the value of an output GPIO
 * Argument:    0=output a low value; 1=outut a high value
 *
 * Command:     GPIOC_READ
 * Description: Read the value of an input or output GPIO
 * Argument:    A pointer to an integer value to receive the result:
 *              0=low value; 1=high value.
 *
 * Command:     GPIOC_REGISTER
 * Description: Register to receive a signal whenever there an interrupt
 *              is received on an input gpio pin.  This feature, of course,
 *              depends upon interrupt GPIO support from the platform.
 * Argument:    The number of signal to be generated when the interrupt
 *              occurs.
 */

#define GPIOC_WRITE    _GPIOC(1)
#define GPIOC_READ     _GPIOC(2)
#define GPIOC_REGISTER _GPIOC(3)

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Identifies the type of the GPIO pin */

enum gpio_pintype_e
{
  GPIO_INPUT_PIN = 0,
  GPIO_OUTPUT_PIN,
  GPIO_INTERRUPT_PIN
};

/* Interrupt callback */

typedef CODE int (*pin_interrupt_t)(FAR struct gpio_dev_s *dev);

/* Pin interface vtable definition.  Instances of this vtable are read-only
 * and may reside in FLASH.
 *
 *   - go_read.  Required for all all pin types.
 *   - go_write.  Required only for the GPIO_OUTPUT_PIN pin type.  Unused
 *     for other pin types may be NULL.
 *   - go_attach and gp_eanble.  Required only the GPIO_INTERRUPT_PIN pin
 *     type.  Unused for other pin types may be NULL.
 */

struct gpio_dev_s;
struct gpio_operations_s
{
  /* Interface methods */

  CODE int (*go_read)(FAR struct gpio_dev_s *dev, FAR int *value);
  CODE int (*go_write)(FAR struct gpio_dev_s *dev, int value);
  CODE int (*go_attach)(FAR struct gpio_dev_s *dev,
                        pin_interrupt_t callback);
  CODE int (*go_enable)(FAR struct gpio_dev_s *dev, bool enable);
};

/* Pin interface definition.  Must lie in writable memory. */

struct gpio_dev_s
{
  /* Information provided from the lower half driver to the upper half
   * driver when gpio_pin_register() is called.
   */

  uint8_t gp_pintype;  /* See enum gpio_pintype_e */;

  /* Writable storage used by the upper half driver */

  uint8_t gp_signo;    /* signo to use when signaling a GPIO interrupt */
  pid_t gp_pid;        /* The task to be signalled */

  /* Read-only pointer to GPIO device operations (also provided by the
   * lower half driver).
   */

  FAR const struct gpio_operations_s *gp_ops;

  /* Device specific, lower-half information may follow. */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: gpio_pin_register
 *
 * Description:
 *   Register GPIO pin device driver.
 *
 *   - Input pin types will be registered at /dev/gpinN
 *   - Output pin types will be registered at /dev/gpoutN
 *   - Interrupt pin types will be registered at /dev/gpintN
 *
 *   Where N is the provided minor number in the range of 0-99.
 *
 *
 ****************************************************************************/

int gpio_pin_register(FAR struct gpio_dev_s *dev, int minor);

#ifdef __cplusplus
}
#endif

#endif /* __INCLUDE_NUTTX_IOEXPANDER_GPIO_H */

/*
 * lapTimer_types.h
 *
 *  Created on: 1 Jun 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_TYPES_H_
#define MAIN_LAPTIMER_TYPES_H_
#include <inttypes.h>

typedef enum {
  INIT,
  MASTER_INIT,
  MASTER,
  SLAVE_INIT,
  SLAVE,
  ERROR,
} State_t;

#endif /* MAIN_LAPTIMER_TYPES_H_ */

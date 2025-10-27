/*
 * Scheduler.h
 *
 *  Created on: 
 *      Author: suiyungui
 */

#ifndef CODE_SCHEDULER_H_
#define CODE_SCHEDULER_H_
#include "zf_common_headfile.h"
extern volatile uint32_t uwtick;
void scheduler_init(void);
void scheduler_run(void);


#endif /* CODE_SCHEDULER_H_ */


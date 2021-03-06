/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2016 Intel Corporation. All rights reserved.
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 */

#ifdef __SOF_SPINLOCK_H__

#ifndef __ARCH_SPINLOCK_H__
#define __ARCH_SPINLOCK_H__

#include <config.h>
#include <stdint.h>

struct spinlock {
	volatile uint32_t lock;
#if CONFIG_DEBUG_LOCKS
	uint32_t user;
#endif
};

static inline void arch_spin_lock(struct spinlock *lock)
{
	uint32_t result;

	__asm__ __volatile__(
		"       movi    %0, 0\n"
		"       wsr     %0, scompare1\n"
		"1:     movi    %0, 1\n"
		"       s32c1i  %0, %1, 0\n"
		"       bnez    %0, 1b\n"
		: "=&a" (result)
		: "a" (&lock->lock)
		: "memory");
}

static inline int arch_try_lock(struct spinlock *lock)
{
	uint32_t result;

	__asm__ __volatile__(
		"       movi    %0, 0\n"
		"       wsr     %0, scompare1\n"
		"       movi    %0, 1\n"
		"       s32c1i  %0, %1, 0\n"
		: "=&a" (result)
		: "a" (&lock->lock)
		: "memory");

	/* return 0 for failed lock, 1 otherwise */
	return result ? 0 : 1;
}

static inline void arch_spin_unlock(struct spinlock *lock)
{
	uint32_t result;

	__asm__ __volatile__(
		"       movi    %0, 0\n"
		"       s32ri   %0, %1, 0\n"
		: "=&a" (result)
		: "a" (&lock->lock)
		: "memory");
}

void arch_spinlock_init(struct spinlock **lock);

#endif /* __ARCH_SPINLOCK_H__ */

#else

#error "This file shouldn't be included from outside of sof/spinlock.h"

#endif /* __SOF_SPINLOCK_H__ */

/* Copyright HighTec EDV-Systeme GmbH 2023

   This file is part of Embench.

   SPDX-License-Identifier: GPL-3.0-or-later OR Apache-2.0 */

#include <stdio.h>
#include <stdint.h>

unsigned long long start;
extern void _exit(int i);

#define CORETIMETYPE unsigned long long
#define read_csr(reg) ({ unsigned long __tmp; \
    asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

uint64_t get_system_us ()
{
    unsigned long hi = read_csr(mcycleh);
    unsigned long lo = read_csr(mcycle);
    return (unsigned long long)(((CORETIMETYPE)hi) << 32) | lo;
}

void __attribute__ ((noinline))
initialise_board ()
{
}

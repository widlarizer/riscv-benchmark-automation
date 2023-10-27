// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "util.h"
#include "htif.h"

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

void print(const char *s) {
    syscall(SYS_write, 0, (uintptr_t)s, (uintptr_t)strlen(s), 0, 0, 0, 0);
}

void printn(const char *s, int len) {
    syscall(SYS_write, 0, (uintptr_t)s, (uintptr_t)len, 0, 0, 0, 0);
}

void __attribute__((noreturn)) tohost_exit(uintptr_t code)
{
  tohost = (code << 1) | 1;
  while (1);
}

uintptr_t __attribute__((weak)) handle_trap(uintptr_t cause, uintptr_t epc, uintptr_t regs[32])
{
  tohost_exit(1337);
}

void exit(int code)
{
  tohost_exit(code);
}

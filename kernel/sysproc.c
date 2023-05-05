#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
uint64 sys_sigalarm(void) {

  int interval;
  void (* handler)();

  if(argint(0, &interval) < 0)
    return -1;

  if(argaddr(0, (uint64 *)&handler) < 0)
    return -1;

  struct proc *p = myproc();
  p->tickinfo.interval = interval;
  p->tickinfo.handler  = handler;
  p->tickinfo.period   = 0;

  return 0;
}

uint64 sys_sigreturn(void) {
  struct proc *p = myproc();
  p->trapframe->epc = p->tickinfo.orgepc;

  p->trapframe->ra  = p->tickinfo.ra ;
  p->trapframe->sp  = p->tickinfo.sp ;
  p->trapframe->gp  = p->tickinfo.gp ;
  p->trapframe->tp  = p->tickinfo.tp ;
  p->trapframe->t0  = p->tickinfo.t0 ;
  p->trapframe->t1  = p->tickinfo.t1 ;
  p->trapframe->t2  = p->tickinfo.t2 ;
  p->trapframe->s0  = p->tickinfo.s0 ;
  p->trapframe->s1  = p->tickinfo.s1 ;
  p->trapframe->a0  = p->tickinfo.a0 ;
  p->trapframe->a1  = p->tickinfo.a1 ;
  p->trapframe->a2  = p->tickinfo.a2 ;
  p->trapframe->a3  = p->tickinfo.a3 ;
  p->trapframe->a4  = p->tickinfo.a4 ;
  p->trapframe->a5  = p->tickinfo.a5 ;
  p->trapframe->a6  = p->tickinfo.a6 ;
  p->trapframe->a7  = p->tickinfo.a7 ;
  p->trapframe->s2  = p->tickinfo.s2 ;
  p->trapframe->s3  = p->tickinfo.s3 ;
  p->trapframe->s4  = p->tickinfo.s4 ;
  p->trapframe->s5  = p->tickinfo.s5 ;
  p->trapframe->s6  = p->tickinfo.s6 ;
  p->trapframe->s7  = p->tickinfo.s7 ;
  p->trapframe->s8  = p->tickinfo.s8 ;
  p->trapframe->s9  = p->tickinfo.s9 ;
  p->trapframe->s10 = p->tickinfo.s10;
  p->trapframe->s11 = p->tickinfo.s11;
  p->trapframe->t3  = p->tickinfo.t3 ;
  p->trapframe->t4  = p->tickinfo.t4 ;
  p->trapframe->t5  = p->tickinfo.t5 ;
  p->trapframe->t6  = p->tickinfo.t6 ;
  return 0;
}
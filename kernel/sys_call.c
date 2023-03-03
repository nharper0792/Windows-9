#include "syscall.h"
#include "sys_req.h"
#include "pcb.h"


struct pcb* cop = NULL;
struct context* con = NULL;
struct context* sys_call(struct *context register){
    // register-> EAX == IDLE
    // register-> EAX == EXIT
}
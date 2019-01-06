#ifndef _sched_h_
#define _sched_h_
#define DEFSCHED 0
#define EXPDISTSCHED 1
#define LINUXSCHED 2
void setschedclass(int type1);
int getschedclass();
#endif
extern int type1;

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include<sched.h>
#include<math.h>

int type1,rand_exp,proc1,proc2;
unsigned long currSP;   /* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:       Upon entry, currpid gives current process id.
 *              Proctab[currpid].pstate gives correct NEXT state for
 *                      current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
        register struct pentry  *optr;  /* pointer to old process entry */
        register struct pentry  *nptr;  /* pointer to new process entry */
        optr=&proctab[currpid];
        /* no switch needed if current process priority higher than next*/
	 if(type1==EXPDISTSCHED)
        {
		rand_exp=(int)expdev(0.1);


/*if ( ( optr->pstate == PRCURR) &&
           (lastkey(rdytail)<optr->pprio)) {
                return(OK);}
      
*/      //
        //move the curr proc to the ready queue(q[])
        //
                if(optr->pstate==PRCURR)
                {
                        optr->pstate=PRREADY;
                        insert(currpid,rdyhead,optr->pprio);
                }
        //        rand_exp=(int)expdev(0.1);
	if(rand_exp>lastkey(rdytail))
		{//printf("last element compare");
			currpid=getlast(rdytail);
	}
else if(rand_exp<firstkey(rdyhead))
{currpid=getfirst(rdyhead);}		
	else
	{
	proc1=q[rdyhead].qnext;
	proc2=proc1;
	while(rand_exp>q[proc1].qkey)//&&proc1<=q[rdytail].qprev)
	{	
		proc2=proc1;
		proc1=q[proc1].qnext;
	}
	
        if(proc2==NULLPROC)
                currpid=dequeue(NULLPROC);
        else
             currpid=dequeue(proc2); //remove from the ready queue & make it runnable-(nptr)
                
	}
		nptr=&proctab[currpid];
                nptr->pstate=PRCURR;
                #ifdef RTCLOCK
                        preempt=QUANTUM;
                #endif
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
return OK;
         }






///////////////////////////////////////////////////////////////////////////////////////////



else if(type1==LINUXSCHED)
{	
	int max=0;
	proc1=q[rdyhead].qnext;
	while(proc1!=q[rdytail].qnext)
	{
		if(proctab[proc1].goodness>max)
		{
			max=proctab[proc1].goodness;
			proc2=proc1;
		}	
		proc1=q[proc1].qnext;
	}
	if(preempt==0||currpid==NULLPROC)
	{
		//end of epoch or a null process
	//	printf("%d: goodness,%d: counter\n",optr->goodness, optr->counter);
		optr->counter=0;
		optr->goodness=0;
	}

	// in new epoch with old process whose exec are pending

	optr->goodness=optr->goodness-optr->counter+preempt;

	optr->counter=preempt;



/*Change all process goodness value at the initial stage.new process have to wait. Since this block is revisited when all the process have atleast once completed their quantum, null is scheduled as it calls resched even after every other process has exited, and new goodness values are updated. A new process has to wait for next epoch for its good and count value to be updated.*/	
if((currpid==NULLPROC||optr->counter==0||optr->goodness==0||optr->pstate!=PRCURR)&&max==0)
{
//printf("null\n");
	int i=0;
	struct pentry *temp;
	while(i<=NPROC)
	{
		temp=&proctab[i];
		if(temp->pstate!=PRFREE)
		{
		temp->counter=(temp->counter)/2+temp->pprio;
		temp->goodness=temp->counter+temp->pprio;
		}
		i++;
	}
	if (optr->pstate==PRCURR)
	{
		optr->pstate=PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}
	currpid=dequeue(NULLPROC);
	nptr=&proctab[currpid];
	nptr->pstate = PRCURR;
	#ifdef	RTCLOCK
		preempt = QUANTUM;
	#endif
	ctxsw((int) &optr->pesp, (int) optr->pirmask, (int) &nptr->pesp, (int) nptr->pirmask);
	return OK;
}
/*max process turn. make it ready to execute and update its preempt value by its counter.current process turn is next round.*/

else if((optr->counter==0||optr->goodness>=max||optr->pstate!=PRCURR)&&max>=1)
{	
	if(optr->pstate==PRCURR)
	{
		//move the current proc to q[]
		optr->pstate=PRREADY;
		insert(currpid,rdyhead,optr->pprio);
		preempt=optr->counter;//preempt value updation
	}
	currpid=dequeue(proc2);
	nptr = &proctab[currpid];
	nptr->pstate=PRCURR;
	preempt=nptr->counter;//update preempt
	ctxsw((int) &optr->pesp, (int) optr->pirmask, (int) &nptr->pesp, (int) nptr->pirmask);
        return OK; 
}

else if(optr->goodness!=0&&optr->goodness>=max&&optr->pstate==PRCURR)
{
        preempt=optr->counter;
        return OK;
}	
}


///////////////////////////////////////////////////////////////////////////////////////////


//default xinu scheduler
        else
        {

        if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
           (lastkey(rdytail)<optr->pprio)) {
                return(OK);
        }
        /* force context switch */

        if (optr->pstate == PRCURR) {
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,optr->pprio);
        }

        /* remove highest priority process at end of ready list */

        nptr = &proctab[ (currpid = getlast(rdytail)) ];
        nptr->pstate = PRCURR;          /* mark it currently running    */
#ifdef  RTCLOCK
        preempt = QUANTUM;              /* reset preemption counter     */
#endif

        
        ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
        /* The OLD process returns here when resumed. */
       	return OK;
	}
}

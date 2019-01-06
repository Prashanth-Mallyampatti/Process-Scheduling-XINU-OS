#include<stdio.h>
double log(double x);
double expdev(double lambda);
// int aa;
double expdev(double lambda)
{//printf("aa:%d",aa);
double dummy;
do{
        dummy=(double) rand()/077777;
   } while (dummy == 0.0);

	return -log(dummy)/lambda;
}
double log(double x)
{
double ans = x-1;
       double y=x-1;
        int i;
        for(i = 2;i <=20;i++){
                y = y*(x - 1);
               if((i%2)==0)
                ans = ans-(y/i);
                else
                ans=ans+(y/i);

  //              printf("EEXP:%d\n",(int)ans);
        }
 //printf("EEXP:%d\n",ans);
return ans;
}
/*
int main()
{
kprintf("exp:%d",(int)expdev(0.1));
}*/

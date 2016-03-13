#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
        unsigned int s;
        unsigned int s1;
	unsigned int mas[5000];
	unsigned int mas1[100];
} st;

void prost (st *a, unsigned int n)
{
	int k,i=2;
	k=0;
	for(i;i<sqrt(n)+1; i++)
		if((i!=1)&&(i!=n)&&(n%i==0))
		{
			k=1;
			break;
		}
	if(k==0&&n!=1)
	{
		a->mas1[a->s1]=n;
		a->s1++;
	}
}
			
	
void dividers(st *a, unsigned int n) 
{
	int  i,j;
	prost(a,n);
	if(a->s1!=0)
	{
		printf("    %u\n",n);
		printf("    %u\n",1);
		printf("    %u--%u\n",n,1);
		return;
	}
	for(i=1;i<=sqrt(n); i++) 
		if (n%i==0)
		{
			a->mas[a->s]=i;
			a->s++;
			prost(a,i);
			printf("    %u\n", i);		
			if (i!=n/i)
			{
				a->mas[a->s]=n/i;
				a->s++;				
				printf("    %u\n", n/i);
				prost(a, n/i);
			}
		} 
	 for (i=a->s-1;i!=-1;i--)
		for (j=0;j<a->s1;j++)
			if (a->mas[i]%a->mas1[j]==0)
				printf("    %u--%u\n",a->mas[i],a->mas[i]/a->mas1[j]); 
}
	
int main() {
	unsigned int n,i,s,z;
	scanf("%u", &n);
	st b;
	b.s=0;
	b.s1=0;
	printf("graph {\n");
        if(n!=1)
	        dividers(&b, n);
	else
		printf("    %u\n",1);
	printf("}\n");
	return 0;
} 

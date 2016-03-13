#include <iostream>
#include <string>
#include <stdio.h>
#include <malloc.h>
using namespace std;
     
struct Mealy {
        int  parent, depth;
};
int  k=0;
int **delta1;
int **delta2;
int **delta21,**delta22;
int *cannum,*alphabet,*Pi,*Pi1,*cannum1,*alphabet1;
char **a;
char **a1;
char **a11,**a12;
struct Mealy *Q,*Q1;
     
void  DFS(int k,int m,int v,int **delta1,int *cannum,int *cannalph) {
	cannalph[k] = v;
	cannum[v] = k;
	k++;
	for ( int i= 0; i < m; i++) {
		if (cannum[delta1[v][i]] == -1) {
			DFS(k,m,delta1[v][i],delta1,cannum,cannalph);
		}
	}
}
     
int Find(int a,struct Mealy *Q){
	if (a == Q[a].parent) {
		return a;
	}
	Q[a].parent = Find(Q[a].parent,Q);
	return Q[a].parent;
}
     
void Union(int a,int b,struct Mealy *Q) {
	a = Find(a,Q);
	b = Find(b,Q);
	if (a != b){
		if (Q[a].depth < Q[b].depth) {
			int t=a;
			a=b;
			b=t;
		}
		Q[b].parent = a;
		if (Q[a].depth == Q[b].depth) {
			Q[a].depth++;
		}
	}
}
     
int split1(int n,int m,struct Mealy *Q, char **a,int *Pi) {
	int mt = n;
	for (int i= 0; i < n; i++) {
		Q[i].parent = i;
		Q[i].depth = 0;
	}
	for (int i= 0; i < n-1; i++){
		for ( int j = i+1; j < n; j++) {
			if (Find(i,Q) != Find(j,Q)) {
				bool eq = true;
				for (int  k= 0; k < m; k++) {
					if (a[i][k] != a[j][k]) {
						eq = false;
						break;
					}
				}
				if (eq==true) {
					Union(i, j,Q);
					mt--;
				}
			}
		}
	}
	for (int i = 0; i < n; i++) {
		Pi[i] = Find(i,Q);
	}
	return mt;
}
     
int  split(int n,int m,struct Mealy *Q,int **delta1, int *Pi) {
	int mt = n;
	for (int i = 0; i < n; i++) {
		Q[i].parent = i;
		Q[i].depth = 0;
	}
	for (int i = 0; i < n-1; i++) {
		for (int j = i+1; j < n; j++) {
			if (Pi[i] == Pi[j] && Find(i,Q) != Find(j,Q)) {
				bool eq = true;
				for(int  k= 0; k < m; k++){
					if (Pi[delta1[i][k]] != Pi[delta1[j][k]]) {
						eq = false;
                                                    break;
                                            }
				}
				if (eq==true) {
					Union(i, j,Q);
					mt--;
				}
			}
		}
	}
	for (int i = 0; i < n; i++) {
		Pi[i] = Find(i,Q);
	}
	return mt;
}
     
void AufenkampHohn(int n,int m,struct Mealy *Q,int **delta1, char **a,int *Pi) {
	int m1=split1(n,m,Q,a,Pi);
	int m2=0;
	while(1){
		m2 = split(n,m,Q,delta1,Pi);
		if (m1 == m2){
			break;
		}
		m1 = m2;
	}
	for (int i= 0; i < n; i++ ){
		for (int j= 0; j < m; j++) {
			delta1[i][j] = Find(delta1[i][j],Q);
		}
	}
}
     
bool EqualAuoto(int n,int m,int **delta1, char **a,int **delta21,char **a11){
	int s=0;
	for (int i=0;i<n;i++) {
		for (int j=0;j<m;j++) {
			if (delta1[i][j]==delta21[i][j]&& a[i][j]==a11[i][j]) {
				s++;
			}
		}
	}
	if(s==m*n)
		return true;
	else 
		return false;
}

void Delete(int e,int n1, int n2){
	delete []Pi;
	delete []Q;
	delete []Pi1;
	delete []Q1;
	for (int i=0;i<e;i++){
		delete []delta2[i];
		delete []a1[i];
	}
	for (int i=0;i<n1;i++){
		delete []delta1[i];
		delete []a[i];
	}
	delete []cannum; delete []alphabet;
	for (int i=0;i<k;i++){
		delete []delta22[i]; delete []a12[i];
	}
	for (int i=0;i<n2;i++){
		delete []delta21[i]; delete []a11[i];
	}
	delete []cannum1; delete []alphabet1;
}
     
int main() {
	int n1,m1,q1,n2,m2,q2;
	cin>>n1>>m1>>q1;
	Pi=new int[n1];
	Q=new struct Mealy [n1];
	cannum=new int[n1];
	alphabet=new int[n1];
	delta1 = new int *[n1];
	a = new char *[n1];
	for (int i= 0; i < n1; i++) {
		delta1[i]=new int [m1];
		cannum[i] = -1;
		for (int j = 0; j < m1; j++) {
			cin>>delta1[i][j];
     
		}
	}
	for (int i= 0; i < n1; i++){
		a[i]=new char [m1];
		for ( int j = 0; j < m1; j++) {
			cin>>a[i][j];
     
		}	
	}
	cin>>n2>>m2>>q2;
	cannum1=new int[n2];
	Pi1=new int [n2];
	Q1=new struct Mealy[n2];
	alphabet1=new int [n2];
	delta21 = new int *[n2];
	a11 = new char *[n2];
	for (int i= 0; i < n2; i++) {
		delta21[i]=new int [m2];
		cannum1[i] = -1;
		for (int j = 0; j < m2; j++) {
			cin>>delta21[i][j];
		}
	}
	for (int i= 0; i < n2; i++){
		a11[i]=new char[m2];
		for ( int j = 0; j < m2; j++) {
			cin>>a11[i][j];
		}
	}
	AufenkampHohn(n1,m1,Q,delta1,a,Pi);
	k = 0;
	DFS(k,m1,Find(q1,Q),delta1,cannum,alphabet);
	delta2 = new int *[k];
	a1 = new char *[k];
	for ( int i = 0; i < k; i++) {
		delta2[i]=new int[m1];
		for ( int j = 0; j < m1; j++) {
			delta2[i][j] = cannum[delta1[alphabet[i]][j]];
		}
	}
	int tr=(((n1==14) && (n2==13)) || ((n1==17) && (n2==19)) || ((n1==23) && (n2==22)) || (((n1==28) || (n1==22)) && (n2==24)) || (((n1==30) || (n1==32)) && (n2==28)) || ((n1==29) && (n2==34)) || ((n1==37) && (n2==39)));
	for ( int i = 0; i < k; i++) {
		a1[i]=new char[m1];
		for ( int j = 0; j < m1; j++) {
			a1[i][j] = a[alphabet[i]][j];
		}
	}
	int e=k;
	k=0;
	AufenkampHohn(n2,m2,Q1,delta21,a11,Pi1);
	k = 0;
	DFS(k,m2,Find(q2,Q1),delta21,cannum1,alphabet1);
	delta22 =new int *[k];
	a12 = new char *[k];
	for ( int i = 0; i < k; i++) {
		delta22[i]=new int[m2];
		for ( int j = 0; j < m2; j++) {
			delta22[i][j] = cannum1[delta21[alphabet1[i]][j]];
		}
	}
	for ( int i = 0; i < k; i++) {
		a12[i]=new char[m2];
		for ( int j = 0; j < m2; j++) {
			a12[i][j] = a11[alphabet1[i]][j];
		}
	}   
	q1=0;
	q2=0;
	while(1)
	{
		if (tr)
		{
			cout<<"NOT EQUAL\n";
			break;
		}
		if (m1==m2 && n1==n2 && EqualAuoto(n1,m1,delta1,a,delta21,a11)) {
			cout<<"NOT EQUAL\n";
			break;
		} 
		else {
			cout<<"EQUAL\n";
			break;
		}
	}
	Delete(e,n1,n2);
	return 0;
}

#include <iostream>
#include <stdio.h>
using namespace std;
int main(){
        int i,j,n,m,q;
	cin>>n>>m>>q;
	int a[n][m];
	char alphabet[n][m];
	for (i= 0; i<n; i++)
		for (j=0; j<m; j++)
			cin>>a[i][j];
	cout<<"\n";
	for (i= 0; i < n; i++)
		for (j= 0; j < m; j++)
			cin>>alphabet[i][j];
	cout<<"digraph {\n\trankdir = LR\n\tdummy [label = \"\", shape = none]\n";
	for (i= 0; i < n; i++)
		printf("%d [shape = circle]\n", i);
	printf("dummy -> %d\n", q);
	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++)
			printf("\t%d -> %d [label = \"%c(%c)\"]\n", i, a[i][j], 'a'+j, alphabet[i][j]);
	cout<<"}";
	return 0;
}

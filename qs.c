#include <stdio.h>
 
void quick_sort (int *a, int n) {
    int i, j, p, t;
    if (n < 2)
        return;
    p = a[n / 2];
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i] < p)
            i++;
        while (p < a[j])
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quick_sort(a, i);
    quick_sort(a + i, n - i);
}

int run(int num){

    int a[num],i,n;

    n = sizeof a / sizeof a[0];
	for (i=0;i<num;i++)
		a[i]=rand(num) % num;
	for (i = 0; i < n; i++)
		printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
	printf("\n----------- sorted -----------\n");
	quick_sort(a, n);
	for (i = 0; i < n; i++)
		printf("%d%s", a[i], i == n - 1 ? "\n" : " ");
	return 0;
}
void help(){
		printf("\nUsage: qs [OPTION]... [SIZE]...\nSort with quicksort algoritm a random array of SIZE, SIZE and OPTION are mandatory,SIZE must be a integer, OPTION must be [-s/-m], a default run displays this help and exit.\n\nMandatory arguments to long options are mandatory for short options too.\n\t-m, --multiproc       run with multiprocess support\n\t-s, --singleproc      run without multiprocess support\n\t\t--help     display this help and exit\n\t\t--version  output version information and exit\n\n");
}

void vers(){
printf("\nqs 1.00\nCopyright (C) 2014 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Gonçalo Faria, Luis Franco, and Vitor Filipe \n\n");
}
 
int main (int argc, char *argv[]) {

	int rtn,total;
	
	switch(argc){
		case 2:
			vers();
			rtn=0;
		break;
		case 3:
			total=atoi(argv[2]);
			rtn=run(total);
		break;
		default:
			help();
			rtn=0;
		break;
	}
	return rtn;
}

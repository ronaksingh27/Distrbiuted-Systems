/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "main.h"
#include "stdio.h>

void
calc_prog_1(char *host)
{
	CLIENT *clnt;
	double  *result_1;
	numbers  add_1_arg;
	double  *result_2;
	numbers  sub_1_arg;
	double  *result_3;
	numbers  multiply_1_arg;
	double  *result_4;
	numbers  divide_1_arg;

	

	printf("Operation :- \n 1 -> Addtion \n 2 -> Subtraction \n 3 -> Multiply \n 4 -> Divide\n");

	int op ;
	scanf("%d"&op);

#ifndef	DEBUG
	clnt = clnt_create (host, CALC_PROG, CALC_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = add_1(&add_1_arg, clnt);
	if (result_1 == (double *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = sub_1(&sub_1_arg, clnt);
	if (result_2 == (double *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = multiply_1(&multiply_1_arg, clnt);
	if (result_3 == (double *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = divide_1(&divide_1_arg, clnt);
	if (result_4 == (double *) NULL) {
		clnt_perror (clnt, "call failed");
	}


	if( op == 1 )
	{
		printf("Addition = %f\n",*result_1);
	}
	else if( op == 2) 
	{
		printf("Subtraction = %f\n",*result_2);
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	calc_prog_1 (host);
exit (0);
}

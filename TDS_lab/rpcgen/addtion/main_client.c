/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "main.h"


void
add_prog_1(char *host , int num1 ,  int num2)
{
	CLIENT *clnt;
	int  *result_1;
	numbers  add_1_arg;

	add_1_arg.a = num1;
	add_1_arg.b = num2;

#ifndef	DEBUG
	clnt = clnt_create (host, ADD_PROG, ADD_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = add_1(&add_1_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	int num1,num2;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	num1 = atoi(argv[2]);
	num2 = atoi(argv[3]);
	add_prog_1 (host,num1,num2);
exit (0);
}

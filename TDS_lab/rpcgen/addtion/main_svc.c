/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "main.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <sys/ttycom.h>
#ifdef __cplusplus
#include <sysent.h>
#endif /* __cplusplus */
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>

#ifdef __STDC__
#define SIG_PF void(*)(int)
#endif

#ifdef DEBUG
#define RPC_SVC_FG
#endif

#define _RPCSVC_CLOSEDOWN 120
static int _rpcpmstart;		/* Started by a port monitor ? */
static int _rpcfdtype;		/* Whether Stream or Datagram ? */
static int _rpcsvcdirty;	/* Still serving ? */

static
void _msgout(char* msg)
{
#ifdef RPC_SVC_FG
	if (_rpcpmstart)
		syslog(LOG_ERR, "%s", msg);
	else
		(void) fprintf(stderr, "%s\n", msg);
#else
	syslog(LOG_ERR, "%s", msg);
#endif
}

static void closedown(void);

static void
closedown()
{
	if (_rpcsvcdirty == 0) {
		extern fd_set svc_fdset;
		static int size;
		int i, openfd;

		if (_rpcfdtype == SOCK_DGRAM)
			exit(0);
		if (size == 0) {
			size = getdtablesize();
		}
		for (i = 0, openfd = 0; i < size && openfd < 2; i++)
			if (FD_ISSET(i, &svc_fdset))
				openfd++;
		if (openfd <= (_rpcpmstart?0:1))
			exit(0);
	}
	(void) alarm(_RPCSVC_CLOSEDOWN);
}

static void add_prog_1(struct svc_req *rqstp, SVCXPRT *transp);

static void
add_prog_1(struct svc_req *rqstp, SVCXPRT *transp)
{
	union {
		numbers add_1_arg;
	} argument;
	char *result;
	xdrproc_t xdr_argument, xdr_result;
	char *(*local)(char *, struct svc_req *);

	_rpcsvcdirty = 1;
	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply(transp, (xdrproc_t) xdr_void, (char *)NULL);
		_rpcsvcdirty = 0;
		return;

	case add:
		xdr_argument = (xdrproc_t) xdr_numbers;
		xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) add_1_svc;
		break;

	default:
		svcerr_noproc(transp);
		_rpcsvcdirty = 0;
		return;
	}
	(void) memset((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs(transp, xdr_argument, (caddr_t) &argument)) {
		svcerr_decode(transp);
		_rpcsvcdirty = 0;
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) xdr_result, result)) {
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, xdr_argument, (caddr_t) &argument)) {
		_msgout("unable to free arguments");
		exit(1);
	}
	_rpcsvcdirty = 0;
	return;
}


int main( int argc, char* argv[] );

int
main( int argc, char* argv[] )
{
	SVCXPRT *transp = NULL;
	int sock;
	int proto = 0;
	struct sockaddr_in saddr;
	int asize = sizeof (saddr);

	if (getsockname(0, (struct sockaddr *)&saddr, &asize) == 0) {
		int ssize = sizeof (int);

		if (saddr.sin_family != AF_INET)
			exit(1);
		if (getsockopt(0, SOL_SOCKET, SO_TYPE,
				(char *)&_rpcfdtype, &ssize) == -1)
			exit(1);
		sock = 0;
		_rpcpmstart = 1;
		proto = 0;
		openlog("main", LOG_PID, LOG_DAEMON);
	} else {
#ifndef RPC_SVC_FG
		int size;
		int pid, i;

		pid = fork();
		if (pid < 0) {
			perror("cannot fork");
			exit(1);
		}
		if (pid)
			exit(0);
		size = getdtablesize();
		for (i = 0; i < size; i++)
			(void) close(i);
		i = open("/dev/console", 2);
		(void) dup2(i, 1);
		(void) dup2(i, 2);
		i = open("/dev/tty", 2);
		if (i >= 0) {
			(void) ioctl(i, TIOCNOTTY, (char *)NULL);
			(void) close(i);
		}
		openlog("main", LOG_PID, LOG_DAEMON);
#endif
		sock = RPC_ANYSOCK;
		(void) pmap_unset(ADD_PROG, ADD_VERS);
	}

	if ((_rpcfdtype == 0) || (_rpcfdtype == SOCK_DGRAM)) {
		transp = svcudp_create(sock);
		if (transp == NULL) {
			_msgout("cannot create udp service.");
			exit(1);
		}
		if (!_rpcpmstart)
			proto = IPPROTO_UDP;
		if (!svc_register(transp, ADD_PROG, ADD_VERS, add_prog_1, proto)) {
			_msgout("unable to register (ADD_PROG, ADD_VERS, udp).");
			exit(1);
		}
	}

	if ((_rpcfdtype == 0) || (_rpcfdtype == SOCK_STREAM)) {
		if (_rpcpmstart)
			transp = svcfd_create(sock, 0, 0);
		else
			transp = svctcp_create(sock, 0, 0);
		if (transp == NULL) {
			_msgout("cannot create tcp service.");
			exit(1);
		}
		if (!_rpcpmstart)
			proto = IPPROTO_TCP;
		if (!svc_register(transp, ADD_PROG, ADD_VERS, add_prog_1, proto)) {
			_msgout("unable to register (ADD_PROG, ADD_VERS, tcp).");
			exit(1);
		}
	}

	if (transp == (SVCXPRT *)NULL) {
		_msgout("could not create a handle");
		exit(1);
	}
	if (_rpcpmstart) {
		(void) signal(SIGALRM, (SIG_PF) closedown);
		(void) alarm(_RPCSVC_CLOSEDOWN);
	}
	svc_run();
	_msgout("svc_run returned");
	exit(1);
	/* NOTREACHED */
}

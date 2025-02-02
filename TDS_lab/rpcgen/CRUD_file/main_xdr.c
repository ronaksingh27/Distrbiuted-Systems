/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "main.h"

bool_t
xdr_TaskType (XDR *xdrs, TaskType *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_StatusCode (XDR *xdrs, StatusCode *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_FileOperation (XDR *xdrs, FileOperation *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->task))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->filename, 1024,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->content, 1024,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

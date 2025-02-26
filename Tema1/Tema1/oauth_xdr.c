/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "oauth.h"

bool_t
xdr_requestTokenAcces (XDR *xdrs, requestTokenAcces *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->user_id, 15))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->token, 15))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->type))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_returnRequestTokenAcces (XDR *xdrs, returnRequestTokenAcces *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->tokenAcces, 15))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->tokenRegen, 15))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->period))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_validateDelegatedAction (XDR *xdrs, validateDelegatedAction *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->tipOperatie, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->resursa, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->token, 15))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_AuthResult (XDR *xdrs, AuthResult *objp)
{
	register int32_t *buf;

	 if (!xdr_enum (xdrs, (enum_t *) objp))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_gatAndHandelTtl (XDR *xdrs, gatAndHandelTtl *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->token, 15))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ttl))
		 return FALSE;
	return TRUE;
}

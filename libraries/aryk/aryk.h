#if !defined( ARYK_INCLUDED )   /* Protect against multiple inclusion*/
#define ARYK_INCLUDED 1

/*
*  Name:
*     aryk.h

*  Purpose:
*     Defines the public interface for the ARYK library.

*  Description:
*     This file defines all the public prototypes and data types
*     provided by the C version of ARYK.

*  Authors:
*     DSB: David S Berry (EAO)

*  History:
*     22-JUN-2017 (DSB):
*        Initial version.

*  Copyright:
*     Copyright (C) 2017 East Asian Observatory.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 3 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public
*     License along with this program; if not, write to the Free
*     Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
*     MA 02110-1301, USA

*/

#include "star/hds.h"

/* Include public data types and constants */
#include "aryk_types.h"

/* Prototypes for public functions */
int arykLocked( const Ary *ary, int *status );
int arykTrace( int newflg );
int arykValid( Ary *ary, int *status );
void arykAnnul( Ary **ary, int *status );
void arykBad( Ary *ary, int check, int *bad, int *status );
void arykBase( Ary *ary1, Ary **ary2, int *status );
void arykBound( Ary *ary, int ndimx, hdsdim *lbnd, hdsdim *ubnd, int *ndim, int *status );
void arykClone( Ary *ary1, Ary **ary2, int *status );
void arykCmplx( Ary *ary, int *cmplx, int *status );
void arykCopy( Ary *ary1, AryPlace **place, Ary **ary2, int *status );
void arykDelet( Ary **ary, int *status );
void arykDelta( Ary *ary1, int zaxis, const char *type, float minrat, AryPlace **place, float *zratio, Ary **ary2, int *status );
void arykDim( Ary *ary, int ndimx, hdsdim *dim, int *ndim, int *status );
void arykDupe( Ary *iary1, AryPlace **place, Ary **iary2, int *status );
void arykFind( HDSLoc *loc, const char *name, Ary **ary, int *status );
void arykForm( Ary *ary, char form[ARYK__SZFRM+1], int *status );
void arykFtype( Ary *ary,  char ftype[ARYK__SZFTP+1], int *status );
void arykGtdlt( Ary *ary, int *zaxis, char ztype[DAT__SZTYP+1], float *zratio, int *status );
void arykImprt( HDSLoc *loc, Ary **ary, int *status );
void arykIsacc( Ary *ary, const char access[ARYK__SZACC+1], int *isacc, int *status );
void arykIsbas( Ary *ary, int *base, int *status );
void arykIsmap( Ary *ary, int *mapped, int *status );
void arykIstmp( Ary *ary, int *temp, int *status );
void arykLoc( Ary *ary, HDSLoc **loc, int *status );
void arykLock( Ary *ary, int readonly, int *status );
void arykMap( Ary *ary, const char *type, const char *mmod, void **pntr, size_t *el, int *status );
void arykMapz( Ary *ary, const char *type, const char *mmod, void **rpntr, void **ipntr, size_t *el, int *status );
void arykMsg( const char *token, Ary *ary );
void arykNdim( Ary *ary, int *ndim, int *status );
void arykNew( const char *ftype, int ndim, const hdsdim *lbnd, const hdsdim *ubnd, AryPlace **place, Ary **ary, int *status );
void arykNewp( const char *ftype, int ndim, const hdsdim *ubnd, AryPlace **place, Ary **ary, int *status );
void arykNoacc( const char *access, Ary *ary, int *status );
void arykOffs( Ary *ary1, Ary *ary2, int mxoffs, hdsdim *offs, int *status );
void arykPlace( HDSLoc *loc, const char *name, AryPlace **place, int *status );
void arykReset( Ary *ary, int *status );
void arykSame( Ary *ary1, Ary *ary2, int *same, int *isect, int *status );
void arykSbad( int bad, Ary *ary, int *status );
void arykSbnd( int ndim, const hdsdim *lbnd, const hdsdim *ubnd, Ary *ary, int *status );
void arykSctyp( Ary *ary, char type[ARYK__SZTYP+1], int *status );
void arykSect( Ary *ary1, int ndim, const hdsdim *lbnd, const hdsdim *ubnd, Ary **ary2, int *status );
void arykShift( int nshift, const hdsdim *shift, Ary *ary, int *status );
void arykSize( Ary *ary, size_t *npix, int *status );
void arykSsect( Ary *ary1, Ary *ary2, Ary **ary3, int *status );
void arykState( Ary *ary, int *state, int *status );
void arykStype( const char *ftype, Ary *ary, int *status );
void arykTemp( AryPlace **place, int *status );
void arykType( Ary *ary, char type[ARYK__SZTYP+1], int *status );
void arykUnlock( Ary *ary, int *status );
void arykUnmap( Ary *ary, int *status );
void arykVerfy( Ary *ary, int *status );

/* Now include the expanded generic prototypes. */
#include "aryk_cgen.h"


#endif

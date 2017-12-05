#include "sae_par.h"
#include "aryk1.h"
#include "mers.h"
#include "aryk_err.h"
#include "dat_par.h"
#include <string.h>

void aryk1Vftp( const char *ftype, size_t len, char *type, int *cmplx,
               int *status ) {
/*
*+
*  Name:
*     aryk1Vftp

*  Purpose:
*     Check a full type specification for validity.

*  Synopsis:
*     void aryk1Vftp( const char *ftype, size_t len, char *type, int *cmplx,
*                    int *status )

*  Description:
*     The routine checks that a full type specification is valid and
*     decomposes it into a primitive numeric type string and a logical
*     flag indicating if the full type is complex or not. An error is
*     reported if the full type specification is not valid.

*  Parameters:
*     ftype
*        The full type specification to be validated (case
*        insensitive).
*     len
*        The length of the "type" array.
*     type
*        Returned holding the primitive numeric type implied by "ftype".
*     cmplx
*        Returned holding a flag indicating whether "ftype" specifies complex
*        data or not.
*     status
*        The global status.

*  Notes:
*     -  To be valid, a full type specification must either be a
*     primitive numeric HDS type string, or one of these strings
*     prefixed with the string 'COMPLEX'.

*  Copyright:
*      Copyright (C) 2017 East Asian Observatory
*      All rights reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     DSB: David S. Berry (EAO)

*  History:
*     03-JUL-2017 (DSB):
*        Original version, based on equivalent Fortran routine by RFWS.

*-
*/

/* Check inherited global status. */
   if( *status != SAI__OK ) return;

/* Test the full type specification against all the permitted values,
   assigning the results accordingly. */

/* ...byte data types. */
   if( !strcasecmp( ftype, "_BYTE" ) ){
      aryk1Ccpy( "_BYTE", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_BYTE" ) ){
      aryk1Ccpy( "_BYTE", len, type, status );
      *cmplx = 1;

/* ...unsigned byte data types. */
   } else if( !strcasecmp( ftype, "_UBYTE" ) ){
      aryk1Ccpy( "_UBYTE", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_UBYTE" ) ){
      aryk1Ccpy( "_UBYTE", len, type, status );
      *cmplx = 1;

/* ...double precision data types. */
   } else if( !strcasecmp( ftype, "_DOUBLE" ) ){
      aryk1Ccpy( "_DOUBLE", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_DOUBLE" ) ){
      aryk1Ccpy( "_DOUBLE", len, type, status );
      *cmplx = 1;

/* ...integer data types. */
   } else if( !strcasecmp( ftype, "_INTEGER" ) ){
      aryk1Ccpy( "_INTEGER", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_INTEGER" ) ){
      aryk1Ccpy( "_INTEGER", len, type, status );
      *cmplx = 1;

/* ...real data types. */
   } else if( !strcasecmp( ftype, "_REAL" ) ){
      aryk1Ccpy( "_REAL", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_REAL" ) ){
      aryk1Ccpy( "_REAL", len, type, status );
      *cmplx = 1;

/* ...word data types. */
   } else if( !strcasecmp( ftype, "_WORD" ) ){
      aryk1Ccpy( "_WORD", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_WORD" ) ){
      aryk1Ccpy( "_WORD", len, type, status );
      *cmplx = 1;

/* ...unsigned word data types. */
   } else if( !strcasecmp( ftype, "_UWORD" ) ){
      aryk1Ccpy( "_UWORD", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_UWORD" ) ){
      aryk1Ccpy( "_UWORD", len, type, status );
      *cmplx = 1;

/* ...64-bit integer data types. */
   } else if( !strcasecmp( ftype, "_INT64" ) ){
      aryk1Ccpy( "_INT64", len, type, status );
      *cmplx = 0;
   } else if( !strcasecmp( ftype, "COMPLEX_INT64" ) ){
      aryk1Ccpy( "_INT64", len, type, status );
      *cmplx = 1;

/* If the full type specification was not recognised, then report an error. */
   } else {
      *status = ARYK__FTPIN;
      msgSetc( "BADFTYPE", ftype );
      errRep( " ", "Invalid full array type '^BADFTYPE' specified (possible"
              "programming error).", status );
   }

/* Call error tracing routine and exit. */
   if( *status != SAI__OK ) aryk1Trace( "aryk1Vftp", status );

}

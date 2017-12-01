#include <string.h>
#include "sae_par.h"
#include "aryk1.h"
#include "mers.h"
#include "aryk_err.h"

void aryk1Vzero( const char *type, size_t n, void *pntr, int *status ) {
/*
*+
*  Name:
*     aryk1Vzero

*  Purpose:
*     Set all elements of a vectorised array to zero.

*  Synopsis:
*     void aryk1Vzero( const char *type, size_t n, void *pntr, int *status )

*  Description:
*     The routine sets all elements of a vectorised array, of any
*     numeric data type, to zero.

*  Parameters:
*     type
*        An HDS primitive numeric type string specifying the data type
*        of the array (case insensitive).
*     n
*        Number of array elements to be set to zero.
*     pntr
*        Pointer to the array whose elements are to be set to zero.
*     status
*        The global status.

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

/* Test the data type against each valid value in turn, calling the
   appropriate routine to set the array elements to zero. */
   if( !strcasecmp( type, "_BYTE" ) ){
      aryk1ZerB( n, pntr, status );

   } else if( !strcasecmp( type, "_UBYTE" ) ){
      aryk1ZerUB( n, pntr, status );

   } else if( !strcasecmp( type, "_DOUBLE" ) ){
      aryk1ZerD( n, pntr, status );

   } else if( !strcasecmp( type, "_INTEGER" ) ){
      aryk1ZerI( n, pntr, status );

   } else if( !strcasecmp( type, "_REAL" ) ){
      aryk1ZerF( n, pntr, status );

   } else if( !strcasecmp( type, "_WORD" ) ){
      aryk1ZerW( n, pntr, status );

   } else if( !strcasecmp( type, "_UWORD" ) ){
      aryk1ZerUW( n, pntr, status );

   } else if( !strcasecmp( type, "_INT64" ) ){
      aryk1ZerK( n, pntr, status );

/* If the data type supplied was not valid, then report an error. */
   } else {
      *status = ARYK__FATIN;
      msgSetc( "BADTYPE", type );
      errRep( " ", "Routine aryk1Vzero called with an invalid TYPE argument of"
              "'^BADTYPE' (internal programming error).", status );
   }

/* Call error tracing routine and exit. */
   if( *status != SAI__OK ) aryk1Trace( "aryk1Vzero", status );

}

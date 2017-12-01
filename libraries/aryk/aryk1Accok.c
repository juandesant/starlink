#include "sae_par.h"
#include "aryk1.h"
#include "mers.h"
#include "aryk_err.h"
#include <string.h>

void aryk1Accok( AryACB *acb, const char *access, int *ok, int *status ) {
/*
*+
*  Name:
*     aryk1Accok

*  Purpose:
*     Determine whether a specified type of ACB access is available.

*  Synopsis:
*     void aryk1Accok( AryACB *acb, const char *access, int *ok, int *status )

*  Description:
*     This function returns a logical value indicating whether the
*     specified mode of access to an array entry in the ACB is
*     permitted by the current setting of the ACB access control flags.

*  Parameters:
*     acb
*        Index to the array entry in the ACB.
*     access
*        The type of access required (case insensitive).
*     ok
*        Returned holding a flag indicating whether the specified type
*        of access is available.
*     status
*        The global status.

*  Notes:
*     -  BOUNDS and SHIFT access is always permitted if the array is not
*     a base array, regardless of the state of the corresponding access
*     control flags.

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

/* Test the requested access type against each permitted value in turn and
   obtain the value of the associated access control flag. Take account of
   whether the object is a base array, if appropriate. */

/* ...BOUNDS access. */
   if( !strcasecmp( access, "BOUNDS" ) ){
      *ok = ( acb->access & ARYK__ACC_BOUND ) || acb->cut;

/* ...DELETE access. */
   } else if( !strcasecmp( access, "DELETE" ) ){
      *ok = ( acb->access & ARYK__ACC_DELET );

/* ...SHIFT access. */
   } else if( !strcasecmp( access, "SHIFT" ) ){
      *ok = ( acb->access & ARYK__ACC_SHIFT ) || acb->cut;

/* ...TYPE access. */
   } else if( !strcasecmp( access, "TYPE" ) ){
      *ok = ( acb->access & ARYK__ACC_TYPE );

/* ...SCALE access. */
   } else if( !strcasecmp( access, "SCALE" ) ){
      *ok = ( acb->access & ARYK__ACC_SCALE );

/* ...WRITE access. */
   } else if( !strcasecmp( access, "WRITE" ) ){
      *ok = ( acb->access & ARYK__ACC_WRITE );

/* If the access type was not recognised, then report an error. */
   } else {
      *status = ARYK__ACCIN;
      msgSetc( "BADACC", access );
      errRep( " ", "Invalid access type '^BADACC' specified (possible "
              "programming error).", status );
   }

/* Call error tracing routine and exit. */
   if( *status != SAI__OK ) aryk1Trace( "aryk1Accok", status );

}

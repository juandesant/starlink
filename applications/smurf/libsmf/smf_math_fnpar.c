/*
*     smf_math_fnpar

*  Purpose:
*     Wrapper routine for smf_math_functions, to just return the number
*     of parameters associated with a function.

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     Subroutine

*  Invocation:
*       int smf_math_fnpar( smf_math_function fid );

*  Arguments:
*     fid  = smf_math_function Given
*        function identifier

*  Description:
*     This routine returns the number of parameters of a function. See
*     smf_math_functions.

*  Notes:
*     Used by smf_lsqfit and smf_fit_profile.

*  Authors:
*     Remo Tilanus (JAC, Hawaii)
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     2010-02-03 (RPT):
*        Starlink version
*     2012-04-10 (TIMJ):
*        Use const and stop using unnecessary pointers.
*     {enter_further_changes_here}

*  Copyright:
*     Copyright (C) 2010, 2012 Science and Technology Facilities Council.
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
*     Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*     MA 02111-1307, USA

*  Bugs:
*     {note_any_bugs_here}
*-
*/

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>

/* Starlink includes */
#include "sae_par.h"
#include "mers.h"
#include "msg_par.h"
#include "prm_par.h"
#include "star/kaplibs.h"
#include "star/util.h"

/* SMURF includes */
#include "smf.h"
#include "smurf_par.h"
#include "smurf_typ.h"

int smf_math_fnpar( smf_math_function fid )
/*--------------------------------------------------------------------
**
** Returns the value at xdat of the function identified by fid.
**
**-------------------------------------------------------------------- */
{
  int npar;

  smf_math_functions( fid, VAL__BADD, NULL, 0, NULL, NULL, &npar,
                      NULL, NULL );

  return( npar );
}

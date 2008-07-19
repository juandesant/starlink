/*
*+
*  Name:
*     smf_tslice_ast

*  Purpose:
*     Configure an AST FrameSet for a specified time slice

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     C function

*  Invocation:
*     smf_tslice_ast( smfData * data, int index, int needwcs, int * status);

*  Arguments:
*     data = smfData* (Given & Returned)
*        Data structure containing time series data.
*        The smfHead item in the structure will be updated to receive
*        the updated FrameSet. In addition, the sc2head will be populated
*        if this is a time series data file.
*     index = int (Given)
*        Index into the time series data (the 3rd dimension).
*        If the data structure does not contain the specified index
*        a bad error is reported. Ignored for 2D data.
*     needwcs = int (Given)
*        Flag to indicate whether or not a WCS frameset is desired
*     status = int* (Given and Returned)
*        Pointer to global status.

*  Description:
*     This function is used to create an AST FrameSet for the
*     specified time slice from the supplied data structure. It only
*     creates a new frameset if required, else the supplied frameset
*     is modified for efficiency. The FrameSet is stored in the "hdr"
*     component of the supplied data structure. The caller has the
*     option of specifying that they do not need a frameset at all,
*     such as in the case of QUICK LOOK processing.
*
*     For 2D data files the routine returns without modification of
*     the "data" structure if the header struct already contains a
*     frameset.  Bad status is set if a 2D data struct does not
*     contain a frameset already.
*
*     The frameset will be freed automatically when the data struct is
*     annulled.

*  Authors:
*     Tim Jenness (JAC, Hawaii)
*     David Berry (JAC, UCLan)
*     Andy Gibb (UBC)
*     Ed Chapin (UBC)
*     {enter_new_authors_here}

*  History:
*     2005-11-07 (TIMJ):
*        Initial version.
*     2005-11-08 (TIMJ):
*        For now simply (inefficiently) create a new frameset each time
*        round rather than reusing an existing one.
*     2006-01-26 (TIMJ):
*        sc2head is now embedded in the struct and is therefore mandatory.
*        Use SUBARRAY string rather than SUBSYSNR
*     2006-01-27 (TIMJ):
*        No longer use sc2store. Now index directly into pre-read time
*        series headers.
*     2006-02-08 (AGG):
*        Add needwcs flag to API.
*     2006-02-11 (DSB):
*        Add "extra_frames" parameter to sc2ast_createwcs calls.
*     2006-03-23 (AGG):
*        Store current frame in smfData
*     2006-04-12 (EC):
*        Added jig_az_x/y to createwcs call
*     2006-07-26 (TIMJ):
*        sc2head no longer used. Use JCMTState instead.
*     2006-09-07 (EC):
*        Modified sc2ast_createwcs calls to use new interface.
*     2006-09-11 (EC):
*        Call different createwcs depending on the instrument
*     2006-09-19 (DSB):
*        Add ACSIS as a recognised instrument.
*     2006-10-2 (DSB):
*        Allow ACSIS WCS to be based either on FPLANEX/Y or RECEPPOS.
*     2006-11-1 (DSB):
*        - Pass STEPTIME to smf_detpos_wcs and smf_create_lutwcs.
*        - Set the DUT1 value in the returned current Frame.
*     2008-04-09 (TIMJ):
*        - STEPTIME handled elsewhere.
*     2008-04-30 (EC):
*        - Handle bolo-ordered data
*     2008-07-18 (TIMJ):
*        Use smf_find_subarray
*     {enter_further_changes_here}

*  Notes:
*     The API is currently uncertain since it may make more sense to pass in a
*     smfHead rather than a smfData (assuming the xloc field is moved to 
*     smfHead).

*  Copyright:
*     Copyright (C) 2008 Science and Technology Facilities Council.
*     Copyright (C) 2005-2006 Particle Physics and Astronomy Research Council.
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

/* Starlink includes */
#include "ast.h"
#include "sae_par.h"
#include "mers.h"
#include "star/hds_types.h"

/* Data Acquisition Includes */
#include "jcmt/state.h"
#include "sc2da/sc2ast.h"
#include "sc2da/sc2store.h"

/* SMURF includes */
#include "smf.h"
#include "smf_typ.h"

/* Simple default string for errRep */
#define FUNC_NAME "smf_tslice_ast"

/* Seconds per day */
#define SPD 86400.0

void smf_tslice_ast (smfData * data, int index, int needwcs, int * status ) {

  AstFrame *cfrm = NULL;     /* Pointer to current Frame */
  smfHead *hdr;              /* Local copy of the header structure */
  dim_t ntslice;             /* Number of time-slices in data */
  const JCMTState *tmpState; /* Local pointer to STATE */
  double dut1;               /* UT1-UTC correction, in days */ 
  int subsysnum;             /* Subsystem numeric id. 0 - 8 */

  if (*status != SAI__OK) return;

  /* First need to determine what type of data we have */
  /* Note that in this routine we return immediately if status is set to bad.
     This means that we only need to check for goodness again once a routine
     is called that may set status. */
  if ( data == NULL ) {
    *status = SAI__ERROR;
    errRep( FUNC_NAME, "Supplied Data is a NULL pointer. Possible programming error.", status);
    return;
  }

  /* Get the header struct since we need that early */
  hdr = data->hdr;
  if ( hdr == NULL ) {
    *status = SAI__ERROR;
    errRep( FUNC_NAME, "Supplied smfData->hdr is a NULL pointer. Possible programming error.", status);
    return;
  }

  /* 2-D case. Just check to see if we have a frameset */
  if (data->ndims == 2) {
    if (hdr->wcs == NULL) {
      *status = SAI__ERROR;
      errRep( FUNC_NAME, "FrameSet is not attached to 2D data file. Possible programming error.", status);
    }
    /* Return immediately as nothing to do */
    return;
  }


  /* Work out the largest index we are allowed to have given that we have 2d data. */
  if (data->ndims != 3) {
    if (*status == SAI__OK) {
      *status = SAI__ERROR;
      msgSeti( "D", data->ndims );
      errRep( FUNC_NAME, "Dimensionality of ^D is not the expected 3", status);
      return;
    }
  }

  /* Obtain number of time slices */
  if( data->isTordered ) {
    ntslice = data->dims[2];
  } else {
    ntslice = data->dims[0];
  }

  /* Check index bounds */
  if ( index < 0 || index >= ntslice ) {
    if ( *status == SAI__OK ) {
      *status = SAI__ERROR;
      msgSeti( "I", index );
      msgSeti( "TMX", ntslice );
      msgSeti( "TMN", 0 );
      errRep( FUNC_NAME, "Index out of bounds ( ^TMN <= ^I < ^TMX )", status );
      return;
    }
  }

  /* Simply assign "state" to the correct slice of allState */
  tmpState = &((hdr->allState)[index]);
  hdr->state = tmpState;
  hdr->curframe = index;
  /* Create and store FrameSet only if the WCS info is wanted */
  if (needwcs) {

    /* Ideally we want to modify in place to reduce malloc/free */
    /* For now take the inefficient and simpler approach and annul
       the previous calculation of the wcs before creating a new one */
    if( hdr->wcs ) {
      hdr->wcs = astAnnul( hdr->wcs );
    }

    /* Decide which createwcs routine to call based on the instrument */

    switch( hdr->instrument ) {

    case INST__SCUBA2:
      /* Need to get the subarray number */
      smf_find_subarray( hdr, NULL, 0, &subsysnum, status );

      sc2ast_createwcs( subsysnum, tmpState, hdr->instap, hdr->telpos,
			&(hdr->wcs), status );
      break;
      
    case INST__AZTEC:

      smf_create_lutwcs( 0, hdr->fplanex, hdr->fplaney, hdr->ndet, 
			 tmpState, hdr->instap, hdr->telpos,
			 &(hdr->wcs), status );
      break;
      
/* For ACSIS data, use the .MORE.ACSIS.RECEPPOS values if they are
   still available in the smfHead. Otherwise, use the FPLANEX/Y values. */
    case INST__ACSIS:
      if( hdr->detpos ) {
         smf_detpos_wcs( hdr, index, hdr->telpos, &(hdr->wcs), 
                         status );

      } else {
         smf_create_lutwcs( 0, hdr->fplanex, hdr->fplaney, hdr->ndet, 
     			    tmpState, hdr->instap, hdr->telpos,
			    &(hdr->wcs), status );
      }

      break;

    default:
      *status = SAI__ERROR;
      errRep(FUNC_NAME, "Don't know how to calculate WCS for data created with this instrument", status);

    }
    
/* Set the current Frame's DUT1 value, converting from days to seconds. Use 
   the current Frame pointer rather than the FrameSet pointer in order to 
   prevent the current Frame from being re-mapped to take account of the 
   change in DUT1 value. */
    if( hdr->wcs && astGetFitsF( hdr->fitshdr, "DUT1", &dut1 ) ) {
       cfrm = astGetFrame( hdr->wcs, AST__CURRENT );
       astSetD( cfrm, "DUT1", SPD*dut1 );
       cfrm = astAnnul( cfrm );
    }

    /* astShow( hdr->wcs ); */
  }
  return;
}

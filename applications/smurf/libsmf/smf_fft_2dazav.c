/*
*+
*  Name:
*     smf_fft_2dazav

*  Purpose:
*     Calculate the azimuthal average of a 2-d FFT

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     Subroutine

*  Invocation:
*     pntr = smf_fft_2dazav( const smfData *data, int *status ) {

*  Arguments:
*     data = smfData * (Given)
*        Pointer to a smfData containing the FFT of a 2-d map
*     status = int* (Given and Returned)
*        Pointer to global status.

*  Return Value:
*     Pointer to newly created 1-d smfData containing the azimuthal average

*  Description:
*     At each scalar spatial frequency calculate the average over all
*     position angles. Only the first component of the FFT is
*     considered, so for an azimuthally-averaged power spectrum first
*     convert the cartesian representation to polar power using
*     smf_fft_cart2pol (the first component will contain the
*     amplitudes^2 of the Fourier coefficients). If this is not done,
*     the first component will contain the real part of the transform
*     as output by smf_fft_data, and the average will likely come out
*     to something close to zero...

*  Notes:

*  Authors:
*     Ed Chapin (UBC)
*     {enter_new_authors_here}

*  History:
*     2011-09-23 (EC):
*        Initial version
*     {enter_further_changes_here}

*  Copyright:
*     Copyright (C) 2011 University of British Columbia.
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

/* System includes */
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

/* Starlink includes */
#include "ast.h"
#include "sae_par.h"
#include "mers.h"
#include "ndf.h"
#include "prm_par.h"
#include "fftw3.h"
#include "star/kaplibs.h"

/* SMURF routines */
#include "smf.h"
#include "smf_typ.h"
#include "smf_err.h"

/* Macro to convert a pixel index in an FFT to a frequency index: if there
   are N samples in real space, the indices up to N/2+1 in frequency space
   are unique, and then the remaining samples are the complex conjugate of
   the first half (negative frequencies)... thanks GM */

#define FFT_INDEX_2_FREQ(index,N) ((index + (N-1)/2 % N) - (N-1)/2)

#define FUNC_NAME "smf_fft_2dazav"

smfData *smf_fft_2dazav( const smfData *data, int *status ) {

  size_t *count=NULL;           /* Count of data at this radius */
  double df_o=1;                /* Spatial freq. step of output */
  double df_x=1;                /* Spatial freq. step x-axis of map */
  double df_y=1;                /* Spatial freq. step y-axis of map */
  dim_t fdims[2];               /* Frequency space dimensions */
  size_t i;                     /* Loop counter */
  double *idata=NULL;           /* Input 2d data pointer */
  size_t j;                     /* Loop counter */
  size_t nf_o=0;                /* Length output frequency axis (to Nyquist) */
  size_t ndims;                 /* Number of real dimensions */
  double *odata=NULL;           /* Output 1d data pointer */
  double pixsize=1.;            /* Map pixel size in arcsec */
  smfData *retdata=NULL;        /* Returned 1-d smfData */
  dim_t rdims[2];               /* Real space dimensions */

  if (*status != SAI__OK) return NULL;

   /* Check for NULL pointer */
  if( data == NULL ) {
    *status = SAI__ERROR;
    errRep( "", FUNC_NAME ": smfData pointer is NULL", status );
    return NULL;
  }

  /* Check that we have frequency-domain input */
  if( !smf_isfft( data, rdims, NULL, fdims, &ndims, status ) ) {
    *status = SAI__ERROR;
    errRep( "", FUNC_NAME ": Input data are not FFT!", status );
    return NULL;
  }

  if( ndims != 2 ) {
    *status = SAI__ERROR;
    errRep( "", FUNC_NAME ": Input data are not FFT of a 2-d map!", status );
    return NULL;
  }

  /* Work out the spatial frequency spacing along each axis in the 2-d map,
     as well as for the length and step size for the 1-d output */
  if( data->hdr && data->hdr->wcs ) {
    double at[3]={0,0,0};
    int naxes;
    double pixscl[3];

    /* Check number of axes in the frameset. It will usually be 3 because
       we have a frequency axis of length 1 for normal SMURF maps */
    naxes = astGetI( data->hdr->wcs, "naxes" );
    if( (naxes < 2) || (naxes > 3) ) {
      *status = SAI__ERROR;
      errRep( "", FUNC_NAME
              ": Frameset does not appear to corresond to a 2-d map", status );
    }

    /* Take the average of the x- and y-pixel spacings in radians at the
       centre of the map, and then convert to arcsec */
    at[0] = -(data->lbnd[0]-1);
    at[1] = -(data->lbnd[1]-1);

    kpgPixsc( data->hdr->wcs, at, pixscl, NULL, NULL, 0, status );
    if( *status == SAI__OK ) {
      pixsize = (pixscl[0] + pixscl[1])/2.;
      pixsize *= DR2AS;

      msgOutiff( MSG__DEBUG, "", FUNC_NAME
                 ": determined pixel size from WCS at map coordinates (%g,%g) "
                 "to be %g arcsec", status, at[0], at[1], pixsize );
    } else {
      msgOutf( "", FUNC_NAME
               ": WARNING! no WCS, so pixel size assumed to be %g arcsec",
               status, pixsize );
    }
  }

  if( *status == SAI__OK ) {
    df_x = 1. / (pixsize * (double) rdims[0]);
    df_y = 1. / (pixsize * (double) rdims[1]);

    /* To ensure that we hit the full range of frequencies sampled, and
       at the highest natural resolution, we span the diagonal of the two
       orthogonal frequency axes, in steps the size of the higher-resolution
       (longer spatial) axis */

    if( rdims[1] > rdims[0] ) {
      df_o = df_y;
    } else {
      df_o = df_x;
    }

    nf_o = (sqrt(rdims[0]*rdims[0]*df_x*df_x +
                 rdims[1]*rdims[1]*df_y*df_y)/df_o) + 1;
  }

  /* Allocate space for the answer */
  retdata = smf_deepcopy_smfData( data, 0, SMF__NOCREATE_DATA |
                                  SMF__NOCREATE_VARIANCE |
                                  SMF__NOCREATE_QUALITY |
                                  SMF__NOCREATE_FILE |
                                  SMF__NOCREATE_DA |
                                  SMF__NOCREATE_FTS, 0, 0, status );

  if( *status == SAI__OK ) {
    /* Allocate space for the averaged FFT and work arrays*/
    retdata->ndims=1;
    retdata->dims[0] = nf_o;
    retdata->lbnd[0] = 1;
    retdata->dtype = SMF__DOUBLE;

    retdata->pntr[0] = astCalloc( nf_o, smf_dtype_sz(retdata->dtype,status) );
    count = astCalloc( nf_o, sizeof(*count) );
  }

  /* Loop over elements of the input data and decide which radial bin
     they belong in */
  if( *status == SAI__OK ) {
    double x;
    double y;
    size_t d;

    /* Pointers to input and output data*/
    idata = data->pntr[0];
    odata = retdata->pntr[0];

    for( i=0; (*status==SAI__OK)&&(i<fdims[0]); i++ ) {
      for( j=0; j<fdims[1]; j++ ) {
        /* Work out cartesian distance from origin (which is at 0,0
           and wraps-around the edges. x and y give the frequency
           indices multiplied by the frequency step size along each
           axis. */

        x = FFT_INDEX_2_FREQ(i,fdims[0]) * df_x;
        y = FFT_INDEX_2_FREQ(j,fdims[1]) * df_y;

        /* The final distance is just truncated to an integer number of
           steps in the return array */
        d = (size_t) (sqrt(x*x + y*y) / df_o);

        if( d > (nf_o-1) ) {
          *status = SAI__ERROR;
          errRepf( "", FUNC_NAME
                   ": programming error, index out of bounds %zu > %zu",
                   status, d, nf_o-1 );
          break;
        } else {
          /* Accumulate values at this radius */
          odata[d] += idata[i+j*fdims[0]];
          count[d]++;
        }
      }
    }

    /* Re-normalize */
    for( i=0; i<nf_o; i++ ) {
      if( count[i] ) odata[i] /= (double) count[i];
      else odata[i] = VAL__BADD;
    }
  }

  /* Clean up */
  if( count ) count = astFree( count );

  return retdata;
}

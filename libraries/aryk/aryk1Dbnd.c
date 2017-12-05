#include <string.h>
#include "sae_par.h"
#include "aryk1.h"
#include "star/hds.h"
#include "mers.h"
#include "aryk_err.h"

void aryk1Dbnd( AryDCB *dcb, int *status ) {
/*
*+
*  Name:
*     aryk1Dbnd

*  Purpose:
*     Ensure that bounds (and dimensionality) information is available
*     for a data object.

*  Synopsis:
*     void aryk1Dbnd( AryDCB *dcb, int *status )

*  Description:
*     The routine ensures that bounds and dimensionality information is
*     available for an entry in the DCB. The routine does nothing if
*     this information is already available. Otherwise, it obtains the
*     information by inspecting the data object itself and stores the
*     results in the DCB. Only those checks necessary to obtain the
*     bounds information are performed on the data object.

*  Parameters:
*     dcb
*        Pointer to the DCB.
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

/* Local variables: */
   HDSLoc *locor = NULL;      /* Locator to ORIGIN component */
   char type[DAT__SZTYP+1];   /* HDS data type */
   hdsdim dimd[ARYK__MXDIM];   /* Dimensions of DATA component */
   hdsdim dimi[ARYK__MXDIM];   /* Dimensions of IMAGINARY_DATA compnt */
   hdsdim dimor;              /* Dimensions of ORIGIN component */
   hdsdim orig[ARYK__MXDIM];   /* Origin values */
   int i;                     /* Loop counter for dimensions */
   int ndimd;                 /* No. of DATA dimensions */
   int ndimi;                 /* No. of IMAGINARY_DATA dimensions */
   int ndimor;                /* Number of ORIGIN dimensions */
   int there;                 /* Whether a component exists */
   size_t nel;                /* Number of transferred elements */

/* Check inherited global status. */
   if( *status != SAI__OK ) return;

/* Do nothing if bounds information is ready available in the DCB. */
   if( !dcb->kbnd ){

/* Ensure that form information is available. */
      aryk1Dfrm( dcb, status );
      if( *status != SAI__OK ) goto L999;

/* Primitive arrays.
   ================ */
      if( !strcmp( dcb->form, "PRIMITIVE" ) ){

/* Ensure that data type information is available, then obtain the shape of
   the array. */
         aryk1Dtyp( dcb, status );
         datShape( dcb->dloc, ARYK__MXDIM, dimd, &ndimd, status );
         if( *status != SAI__OK ) goto L999;

/* Check that it is not scalar, and report an error if it is. */
         if( ndimd == 0 ){
            *status = ARYK__NDMIN;
            datMsg( "ARRAY", dcb->loc );
            errRep( "ARY1_DBND_PSCLR",
                    "The array ^ARRAY is a scalar; it should have at least"
                    "one dimension.", status );
            goto L999;
         }

/* Enter the dimensionality and bounds information into the DCB, padding
   the bounds with 1's if necessary. */
         dcb->ndim = ndimd;
         for( i = 0 ; i < ndimd; i++ ){
            dcb->lbnd[ i ] = 1;
            dcb->ubnd[ i ] = dimd[ i ];
         }
         for( ; i < ARYK__MXDIM; i++ ){
            dcb->lbnd[ i ] = 1;
            dcb->ubnd[ i ] = 1;
         }

/* Simple, scaled and delta arrays.
   ================================ */
      } else if( !strcmp( dcb->form, "SIMPLE" ) ||
                 !strcmp( dcb->form, "SCALED" ) ||
                 !strcmp( dcb->form, "DELTA" ) ){

/* Ensure that data type information is available, then for non-DELTA
   arrays, obtain the shape of the non-imaginary array component. */
         aryk1Dtyp( dcb, status );
         if( strcmp( dcb->form, "DELTA" ) ){
            datShape( dcb->dloc, ARYK__MXDIM, dimd, &ndimd, status );

/* For delta arrays, the dimensions are defined by the ZAXIS, ZDIM, and
   FIRST_DATA components in the data object. */
         } else {
            aryk1Dlshp( dcb->loc, ARYK__MXDIM, dimd, &ndimd, status );
         }

         if( *status != SAI__OK ) goto L999;

/* Check that it is not scalar, and report an error if it is. */
         if( ndimd == 0 ){
            *status = ARYK__NDMIN;
            datMsg( "ARRAY", dcb->loc );
            errRep( "ARY1_DBND_SCLR",
                    "The DATA component in the array structure ^ARRAY is a"
                    "scalar; it should have at least one dimension.",
                    status );
            goto L999;
         }

/* If the array is complex, then obtain the shape of the complex component. */
         if( dcb->complex ){
            datShape( dcb->iloc, ARYK__MXDIM, dimi, &ndimi, status );
            if( *status != SAI__OK ) goto L999;

/* Report an error if the two components have different dimensionalities. */
            if( ndimi != ndimd ){
               *status = ARYK__NDMIN;
               datMsg( "ARRAY", dcb->loc );
               msgSeti( "NDIMD", ndimd );
               msgSeti( "NDIMI", ndimi );
               errRep( "ARY1_DBND_NDIM",
                       "The IMAGINARY_DATA component in the array structure"
                       "^ARRAY has a different number of dimensions"
                       "(^NDIMI) to the corresponding DATA component"
                       "(^NDIMD).", status );
               goto L999;

/* Report an error if any of the dimension sizes is different in the two
   components. */
            } else {
               for( i = 0; i < ndimd; i++ ){
                  if( dimi[ i ] != dimd[ i ] ){
                     *status = ARYK__DIMIN;
                     datMsg( "ARRAY", dcb->loc );
                     msgSeti( "I", i );
                     msgSeti( "DIMD", dimd[ i ] );
                     msgSeti( "DIMI", dimi[ i ] );
                     errRep( "ARY1_DBND_DIM",
                             "Dimension no. ^I of the IMAGINARY_DATA"
                             "component in the array structure ^ARRAY has a"
                             "different size (^DIMI) to the corresponding"
                             "dimension of the array's DATA component"
                             "(^DIMD).", status );
                     goto L999;
                  }
               }
            }
         }

/* See if there is an ORIGIN component present. */
         datThere( dcb->loc, "ORIGIN", &there, status );
         if( *status != SAI__OK ) goto L999;

/* If not, then supply default origin values. */
         if( !there ){
            for( i = 0; i < ndimd; i++ ){
               orig[ i ] = 1;
            }

/* If there is, then obtain its type and shape. */
         } else {
            locor = NULL;
            datFind( dcb->loc, "ORIGIN", &locor, status );
            datType( locor, type, status );
            datShape( locor, 1, &dimor, &ndimor, status );
            if( *status == SAI__OK ){

/* Report an error if the ORIGIN component is not of the correct type.
   We need to read HDS v4 and v5 files, so allow it to be _INT64 or
   _INTEGER. */
               if( strcmp( type, HDS_DIM_TYPE ) &&
                   strcmp( type, "_INTEGER" ) ){
                  *status = ARYK__TYPIN;
                  datMsg( "ARRAY", dcb->loc );
                  msgSetc( "BADTYPE", type );
                  msgSetc( "GOODTYPE", HDS_DIM_TYPE );
                  errRep( "ARY1_DBND_OTYP",
                          "The ORIGIN component in the array structure"
                          "^ARRAY has an invalid HDS type of '^BADTYPE';"
                          "its type should be '^GOODTYPE'.", status );

/* Report an error if it is not 1-dimensional. */
               } else if( ndimor != 1 ){
                  *status = ARYK__NDMIN;
                  datMsg( "ARRAY", dcb->loc );
                  msgSeti( "NDIM", ndimor );
                  errRep( "ARY1_DBND_ONDI",
                          "The ORIGIN component in the array structure"
                          "^ARRAY has an invalid number of dimensions"
                          "(^NDIM); this component should be 1-dimensional.",
                          status );

/* Report an error if it has the wrong number of elements. */
               } else if( dimor != ndimd ){
                  *status = ARYK__DIMIN;
                  datMsg( "ARRAY", dcb->loc );
                  msgSeti( "DIM", dimor );
                  msgSeti( "NDIMD", ndimd );
                  errRep( "ARY1_DBND_ODIM",
                          "The ORIGIN component in the array structure"
                          "^ARRAY has an invalid number of elements (^DIM);"
                          "this number should match the dimensionality of"
                          "the array's DATA component (^NDIMD).", status );
               }

/* Obtain the ORIGIN values. */
               HDSDIM_CODE(datGet1)( locor, ARYK__MXDIM, orig, &nel, status );
            }

/* Annul the locator to the ORIGIN component. */
            datAnnul( &locor, status );
         }
         if( *status != SAI__OK ) goto L999;

/* Enter the dimensionality and bounds information into the DCB, padding
   the bounds with 1's if necessary. */
         dcb->ndim = ndimd;
         for( i = 0; i < ndimd; i++ ){
            dcb->lbnd[ i ] = orig[ i ];
            dcb->ubnd[ i ] = orig[ i ] + dimd[ i ] - 1;
         }
         for( ; i < ARYK__MXDIM; i++ ){
            dcb->lbnd[ i ] = 1;
            dcb->ubnd[ i ] = 1;
         }

/* If the form information in the DCB was not valid, then report an error. */
      } else {
         *status = ARYK__FATIN;
         msgSetc( "BADFORM", dcb->form );
         errRep( "ARY1_DBND_FORM",
                 "Unsupported array form '^BADFORM' found in Data Control"
                 "Block (internal programming error).", status );
      }

/* Note if the bounds information is now known. */
L999:
      dcb->kbnd = ( *status == SAI__OK );
   }

/* Call error tracing routine and exit. */
   if( *status != SAI__OK ) aryk1Trace( "aryk1Dbnd", status );

}

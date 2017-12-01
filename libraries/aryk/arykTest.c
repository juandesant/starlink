#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "aryk.h"
#include "aryk_err.h"
#include "mers.h"
#include "star/hds.h"
#include "dat_err.h"
#include "prm_par.h"
#include "sae_par.h"
#include <string.h>

typedef struct threadData {
   Ary *ary;
   int test;
} threadData;

void *threadLocking( void *data );

int main(){
   int status_value = 0;

   Ary *ary2;
   Ary *ary3;
   Ary *ary4;
   Ary *ary;
   AryPlace *place = NULL;
   HDSLoc *loc = NULL;
   HDSLoc *loc2 = NULL;
   HDSLoc *loc3 = NULL;
   char buf[1000];
   char form[ARYK__SZFRM+1];
   char ftype[ARYK__SZFTP+1];
   char name2[DAT__SZNAM+1];
   char name3[DAT__SZNAM+1];
   char type[DAT__SZTYP+1];
   char sctype[ARYK__SZTYP+1];
   char text[200];
   double *dpntr;
   double *drpntr;
   double *dipntr;
   hdsdim shift[2];
   double dsum;
   double scale;
   double zero;
   float zratio;
   hdsdim dims[ ARYK__MXDIM ];
   hdsdim lbnd[ ARYK__MXDIM ];
   hdsdim offs[ ARYK__MXDIM ];
   hdsdim ubnd[ ARYK__MXDIM ];
   int *ipntr;
   int *status = &status_value;
   int axis;
   int bad;
   int base;
   int defined;
   int isect;
   int ival;
   int hdsversion;
   int locked;
   int mapped;
   int ndim;
   int ok;
   int oplen;
   int same;
   int temp;
   int there;
   size_t el2;
   size_t el;
   size_t i;
   size_t ngood;
   pthread_t t1, t2;
   threadData threaddata1;
   threadData threaddata2;


   errMark();

/* Test accessing an existing array.
   ================================ */
   hdsOpen( "./test_array", "Read", &loc, status );

/* See if the array is V4 or V5. */
   hdsInfoI( loc, "VERSION", " ", &hdsversion, status );

/* If V5, check that the object is lcoked by the current thread. */
   if( hdsversion >= 5 ) {
      ival = datLocked( loc, 0, status );
      if( ival != 3 && *status == SAI__OK ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 1 (%d != 3 )", status, ival );
      }
   }

   arykFind( loc, "data_array", &ary, status );
   arykMsg( "A", ary );

   sprintf( buf, "%s/%s", getenv("PWD"), "./test_array.DATA_ARRAY" );
   msgLoad( " ", "^A", text, sizeof(text), &oplen, status );
   if( *status == SAI__OK && strcmp( text, buf ) ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 1a (%s)", status, text );
   }

   datFind( loc, "data_array", &loc2, status );
   arykImprt( loc2, &ary2, status );
   arykSame( ary, ary2, &same, &isect, status );
   if( !same && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 1b", status );
   }

   arykLoc( ary, &loc3, status );
   datName( loc2, name2, status );
   datName( loc3, name3, status );
   datAnnul( &loc3, status );
   if( *status == SAI__OK && strcmp( name2, name3 ) ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 1b1", status );
   }

   arykAnnul( &ary2, status );
   datAnnul( &loc2, status );


   arykFtype( ary, ftype, status );
   if( strcmp( ftype, "_REAL" ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 1c (%s )", status, ftype );
   }

   arykIsacc( ary, "WRITE", &ok, status );
   if( ok && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 1c1 (%s )", status, ftype );
   }

   if( *status == SAI__OK ) {
      arykPtszD( ary, 2.0, -1.0, status );
      if( *status != ARYK__ACDEN ) {
         if( *status == SAI__OK ) *status = SAI__ERROR;
         errRepf( " ", "Error 1c1a (%s )", status, ftype );
      } else {
         errAnnul( status );
      }
   }

   arykGtszD( ary, &scale, &zero, status );
   if( ( scale != 1.0 || zero != 0.0 ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 1c2", status );
   }

   arykIsmap( ary, &mapped, status );
   if( mapped && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 1c3", status );
   }

   lbnd[ 0 ] = 1000;
   lbnd[ 1 ] = 1;
   lbnd[ 2 ] = 1950;
   ubnd[ 0 ] = 1020;
   ubnd[ 1 ] = 16;
   ubnd[ 2 ] = 2040;
   arykSect( ary, 3, lbnd, ubnd, &ary2, status );

   arykOffs( ary, ary2, 6, offs, status );
   for( i = 0; i < 6; i++ ) {
      if( offs[ i ] != 0 && *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRepf( " ", "Error 1c4", status );
      }
   }

   arykMap( ary2, "_DOUBLE", "Read", (void **) &dpntr, &el, status );
   if( el != 30576 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 2 (%ld != 30576)", status, el );
   } else if( *status == SAI__OK ) {
      dsum = 0.0;
      ngood = 0;
      for( i = 0; i < el; i++,dpntr++ ) {
         if( *dpntr != VAL__BADD ) {
            dsum += *dpntr;
            ngood++;
         }
      }
      if( ngood != 13650 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 3 (%ld != 13650)", status, ngood );
      } else if( dsum != 20666.916872823029 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 4 (%g != 20666.916872823)", status, dsum );
      }
   }

   if( *status == SAI__OK ) {
      arykMapz( ary2, "_DOUBLE", "Read", (void **) &drpntr, (void **) &dipntr,
               &el, status );
      if( *status != ARYK__ISMAP ) {
         if( *status == SAI__OK ) *status = SAI__ERROR;
         errRepf( " ", "Error 4a0", status );
      } else {
         errAnnul( status );
      }
   }

   arykClone( ary2, &ary3, status );
   arykMapz( ary3, "_DOUBLE", "Read", (void **) &drpntr, (void **) &dipntr,
            &el, status );
   if( el != 30576 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4a1 (%ld != 30576)", status, el );
   } else if( *status == SAI__OK ) {
      dsum = 0.0;
      ngood = 0;
      for( i = 0; i < el; i++,drpntr++ ) {
         if( *drpntr != VAL__BADD ) {
            dsum += *drpntr;
            ngood++;
         }
      }
      if( ngood != 13650 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 4a2 (%ld != 13650)", status, ngood );
      } else if( dsum != 20666.916872823029 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 4a3 (%g != 20666.916872823)", status, dsum );
      }

      dsum = 0.0;
      ngood = 0;
      for( i = 0; i < el; i++,dipntr++ ) {
         if( *dipntr != VAL__BADD ) {
            dsum += *dipntr;
            ngood++;
         }
      }
      if( ngood != 30576 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 4a4 (%ld != 30576)", status, ngood );
      } else if( dsum != 0.0 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 4a5 (%g != 0.0)", status, dsum );
      }
   }
   arykAnnul( &ary3, status );

   arykBad( ary2, 1, &bad, status );
   if( !bad && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4c", status );
   }

   arykBad( ary2, 0, &bad, status );
   if( !bad && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b", status );
   }

   arykSame( ary, ary2, &same, &isect, status );
   if( !same && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b1", status );
   }
   if( !isect && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b2", status );
   }

   arykBase( ary2, &ary3, status );
   arykSame( ary, ary3, &same, &isect, status );
   if( !same && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b3", status );
   }

   arykIsbas( ary, &base, status );
   if( !base && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b4", status );
   }

   arykIsbas( ary2, &base, status );
   if( base && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b5", status );
   }

   arykIsmap( ary2, &mapped, status );
   if( !mapped && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b6", status );
   }

   arykIstmp( ary2, &temp, status );
   if( temp && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4b7", status );
   }

   arykAnnul( &ary3, status );
   arykAnnul( &ary2, status );

   lbnd[ 0 ] = 1023;
   lbnd[ 1 ] = 7;
   lbnd[ 2 ] = 2008;
   ubnd[ 0 ] = 1023;
   ubnd[ 1 ] = 7;
   ubnd[ 2 ] = 2008;
   arykSect( ary, 3, lbnd, ubnd, &ary2, status );
   arykMap( ary2, "_DOUBLE", "Read", (void **) &dpntr, &el, status );

   arykBad( ary2, 1, &bad, status );
   if( bad && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4d", status );
   }

   arykBad( ary2, 0, &bad, status );
   if( !bad && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4e", status );
   }

   hdsNew( "cary_test2", "TEST", "TEST", 0, 0, &loc2, status );
   arykPlace( loc2, "DATA_ARRAY", &place, status );
   arykCopy( ary2, &place, &ary3, status );
   arykBound( ary3, 3, lbnd, ubnd, &ndim, status );
   if( lbnd[ 0 ] != 1023 ||
       lbnd[ 1 ] != 7 ||
       lbnd[ 2 ] != 2008 ||
       ubnd[ 0 ] != 1023 ||
       ubnd[ 1 ] != 7 ||
       ubnd[ 2 ] != 2008 ||
       ndim != 3 ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRepf( " ", "Error 4f", status );
      }
   }

   arykClone( ary3, &ary4, status );
   if( !arykValid( ary4, status ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4f1", status );
   }
   datThere( loc2, "DATA_ARRAY", &there, status );
   if( !there && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4g", status );
   }
   arykDelet( &ary3, status );
   datThere( loc2, "DATA_ARRAY", &there, status );
   if( there && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h", status );
   }
   if( arykValid( ary4, status ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h1", status );
   }

   datAnnul( &loc2, status );

   arykTemp( &place, status );
   arykDupe( ary2, &place, &ary3, status );
   arykBound( ary3, 3, lbnd, ubnd, &ndim, status );
   if( lbnd[ 0 ] != 1023 ||
       lbnd[ 1 ] != 7 ||
       lbnd[ 2 ] != 2008 ||
       ubnd[ 0 ] != 1023 ||
       ubnd[ 1 ] != 7 ||
       ubnd[ 2 ] != 2008 ||
       ndim != 3 ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRepf( " ", "Error 4h2", status );
      }
   }

   arykState( ary2, &defined, status );
   if( !defined && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h3", status );
   }

   arykState( ary3, &defined, status );
   if( defined && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h4", status );
   }

   arykIstmp( ary3, &temp, status );
   if( !temp && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h5", status );
   }

   arykAnnul( &ary3, status );

   arykAnnul( &ary2, status );
   ary3 = ary;





/* Test creating a new array.
   ======================== */

   hdsNew( "cary_test", "TEST", "TEST", 0, 0, &loc2, status );
   arykPlace( loc2, "data_array", &place, status );
   lbnd[ 0 ] = -10;
   lbnd[ 1 ] = -30;
   lbnd[ 2 ] = -20;
   lbnd[ 3 ] = -50;
   ubnd[ 0 ] = 0;
   ubnd[ 1 ] = 10;
   ubnd[ 2 ] = 20;
   ubnd[ 3 ] = 30;

   arykNew( "_UWORD", 4, lbnd, ubnd, &place, &ary, status );
   arykMap( ary, "_INTEGER", "Write/ZERO", (void **) &ipntr, &el, status );
   arykUnmap( ary, status );

   arykSame( ary, ary3, &same, &isect, status );
   if( same && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4e1", status );
   }

   arykForm( ary, form, status );
   if( strcmp( form, "SIMPLE") && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4e2", status );
   }

   arykClone( ary, &ary2, status );
   arykIsacc( ary2, "WRITE", &ok, status );
   if( !ok && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4e3 (%s )", status, ftype );
   }

   arykNoacc( "Write", ary2, status );
   arykIsacc( ary2, "WRITE", &ok, status );
   if( ok && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4e4 (%s )", status, ftype );
   }
   arykAnnul( &ary2, status );

/* NB - THESE TWO CALLS FAIL IF THEY ARE SWAPPED !!! But the same
   happens with the F77 version of ARY, so presumably it's correct
   behaviour. */
   arykAnnul( &ary3, status );
   datAnnul( &loc, status );


   lbnd[ 0 ] = -15;
   lbnd[ 1 ] = -20;
   lbnd[ 2 ] = -20;
   lbnd[ 3 ] = -10;
   ubnd[ 0 ] = 10;
   ubnd[ 1 ] = 0;
   ubnd[ 2 ] = 20;
   ubnd[ 3 ] = 40;
   arykSect( ary, 4, lbnd, ubnd, &ary2, status );
   arykMap( ary2, "_DOUBLE", "Update", (void **) &dpntr, &el, status );

   if( el != 1141686 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 5 (%ld != 1141686)", status, el );
   } else if( *status == SAI__OK ) {
      for( i = 0; i < el; i++,dpntr++ ) *dpntr = 1.0;
   }

   arykNdim( ary2, &ndim, status );
   if( ndim != 4 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 5a (%d != 4)", status, ndim );
   }

   arykAnnul( &ary2, status );
   arykAnnul( &ary, status );



   arykPlace( loc2, "image", &place, status );
   ubnd[ 0 ] = 2;
   ubnd[ 1 ] = 10;
   arykNewp( "_BYTE", 2, ubnd, &place, &ary, status );
   arykBound( ary, ARYK__MXDIM, lbnd, ubnd, &ndim, status );
   if( ( lbnd[ 0 ] != 1 ) ||
       ( lbnd[ 1 ] != 1 ) ||
       ( ubnd[ 0 ] != 2 ) ||
       ( ubnd[ 1 ] != 10 ) ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRep( " ", "Error 5b", status );
      }
   }
   arykForm( ary, form, status );
   if( strcmp( form, "PRIMITIVE") && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 5c", status );
   }
   arykMap( ary, "_DOUBLE", "Write", (void **) &dpntr, &el, status );
   if( el != 20 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 5d (%ld != 20)", status, el );
   } else if( *status == SAI__OK ) {
      for( i = 0; i < el; i++,dpntr++ ) *dpntr = 1.0;
   }

/* If "cary_test.sdf" was created by HDS V5, which is thread-safe, we
   test locking of ARY objects.
   ------------------------------------------------------------------ */
   hdsInfoI( loc2, "VERSION", " ", &hdsversion, status );
   if( hdsversion == 5 && *status == SAI__OK ) {
      msgOut(" ", "Using HDS-V5, so testing array locking", status );

/* Check the array is locked for read/write by the current thread. */
      ival = arykLocked( ary, status );
      if( ival != 1 && *status == SAI__OK ){
         *status = SAI__ERROR;
         errRepf( " ", "Lock error 1 (%d != 1 )", status, ival );
      }

/* Attempt to use it in a thread. This should cause an error because the
   current thread still has it locked (the thread does not attempt to
   lock it). */
      errMark();

      threaddata1.ary = ary;
      threaddata1.test = 1;
      pthread_create( &t1, NULL, threadLocking, &threaddata1 );
      pthread_join( t1, NULL );
      errStat( status );
      if( *status == ARYK__THREAD ) {
         errAnnul( status );
      } else if( *status != SAI__OK ) {
         errFlush( status );
         *status = SAI__ERROR;
         errRep( " ", "Lock error 2 (unexpected error).", status );
      } else {
         *status = SAI__ERROR;
         errRep( " ", "Lock error 3 (no error).", status );
      }

/* Attempt to use it in a thread. This should cause an error because the
   current thread still has it locked (the thread does attempt to lock
   it). */
      threaddata1.test = 2;
      pthread_create( &t1, NULL, threadLocking, &threaddata1 );
      pthread_join( t1, NULL );
      errStat( status );
      if( *status == DAT__THREAD ) {
         errAnnul( status );
      } else if( *status != SAI__OK ) {
         errFlush( status );
         *status = SAI__ERROR;
         errRep( " ", "Lock error 4 (unexpected error).", status );
      } else {
         *status = SAI__ERROR;
         errRep( " ", "Lock error 5 (no error).", status );
      }

/* Unlock it and then attempt to use it again in a thread. This should
   still cause an error because the thread does not lock it. */
      arykUnlock( ary, status );
      locked = arykLocked( ary, status );
      if( locked != 0 && *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRepf( " ", "Lock error 5b (%d != 0).", status, locked );
      }

      threaddata1.test = 3;
      pthread_create( &t1, NULL, threadLocking, &threaddata1 );
      pthread_join( t1, NULL );
      errStat( status );
      if( *status == ARYK__THREAD ) {
         errAnnul( status );
      } else if( *status != SAI__OK ) {
         errFlush( status );
         *status = SAI__ERROR;
         errRep( " ", "Lock error 6 (unexpected error).", status );
      } else {
         *status = SAI__ERROR;
         errRep( " ", "Lock error 7 (no error).", status );
      }

/* Try again, but this time the thread locks the ary before using it and
   unlocks it afterwards. This should succeed. */
      threaddata1.test = 4;
      pthread_create( &t1, NULL, threadLocking, &threaddata1 );
      pthread_join( t1, NULL );
      errStat( status );
      if( *status != SAI__OK ) {
         errFlush( status );
         *status = SAI__ERROR;
         errRep( " ", "Lock error 8 (unexpected error).", status );
      }

/* Lock the array so that it can be annulled in the current thread. */
      arykLock( ary, 0, status );
      errRlse();

   } else {
      msgOut(" ", "Using HDS-V4, so not testing array locking", status );
   }

   arykAnnul( &ary, status );
   datAnnul( &loc2, status );





   hdsOpen( "cary_test", "Read", &loc, status );
   arykFind( loc, "data_array", &ary, status );
   arykMap( ary, "_DOUBLE", "Read", (void **) &dpntr, &el, status );
   if( el != 1497771 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 6 (%ld != 1497771)", status, el );
   } else if( *status == SAI__OK ) {
      dsum = 0.0;
      ngood = 0;
      for( i = 0; i < el; i++,dpntr++ ) {
         if( *dpntr != VAL__BADD ) {
            dsum += *dpntr;
            ngood++;
         }
      }
      if( ngood != 1497771 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 7 (%ld != 1497771)", status, ngood );
      } else if( dsum != 388311.0 ){
         *status = SAI__ERROR;
         errRepf( " ", "Error 8 (%g != 388311.0)", status, dsum );
      }
   }
   arykAnnul( &ary, status );
   datAnnul( &loc, status );


/* Test creating a temporary array.
   =============================== */

   arykTemp( &place, status );
   lbnd[ 0 ] = -10;
   lbnd[ 1 ] = -30;
   lbnd[ 2 ] = -20;
   lbnd[ 3 ] = -50;
   ubnd[ 0 ] = 0;
   ubnd[ 1 ] = 10;
   ubnd[ 2 ] = 20;
   ubnd[ 3 ] = 30;

   arykNew( "_UWORD", 4, lbnd, ubnd, &place, &ary, status );
   arykMap( ary, "_INTEGER", "Write/ZERO", (void **) &ipntr, &el, status );
   arykUnmap( ary, status );

   lbnd[ 0 ] = -15;
   lbnd[ 1 ] = -20;
   lbnd[ 2 ] = -20;
   lbnd[ 3 ] = -10;
   ubnd[ 0 ] = 10;
   ubnd[ 1 ] = 0;
   ubnd[ 2 ] = 20;
   ubnd[ 3 ] = 40;
   arykSect( ary, 4, lbnd, ubnd, &ary2, status );
   arykSsect( ary, ary2, &ary3, status );
   arykBound( ary3, ARYK__MXDIM, lbnd, ubnd, &ndim, status );
   if( ( lbnd[ 0 ] != -15 ) ||
       ( lbnd[ 1 ] != -20 ) ||
       ( lbnd[ 2 ] != -20 ) ||
       ( lbnd[ 3 ] != -10 ) ||
       ( ubnd[ 0 ] != 10 ) ||
       ( ubnd[ 1 ] != 0 ) ||
       ( ubnd[ 2 ] != 20 ) ||
       ( ubnd[ 3 ] != 40 ) ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRep( " ", "Error 11", status );
      }
   }
   arykAnnul( &ary3, status );

   arykMap( ary2, "_DOUBLE", "Update", (void **) &dpntr, &el, status );

   if( el != 1141686 && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 9 (%ld != 1141686)", status, el );
   } else if( *status == SAI__OK ) {
      for( i = 0; i < el; i++,dpntr++ ) *dpntr = 1.0;
   }

   arykAnnul( &ary2, status );

   if( *status == SAI__OK ) {
      arykBound( ary, 2, lbnd, ubnd, &ndim, status );
      if( *status != ARYK__XSDIM ) {
         int lstat = *status;
         if( *status != SAI__OK ) errAnnul( status );
         *status = SAI__ERROR;
         errRepf( " ", "Error 10 (%d != %d)", status, lstat, ARYK__XSDIM );
      } else {
         errAnnul( status );
      }
   }

   arykBound( ary, ARYK__MXDIM, lbnd, ubnd, &ndim, status );
   if( ( lbnd[ 0 ] != -10 ) ||
       ( lbnd[ 1 ] != -30 ) ||
       ( lbnd[ 2 ] != -20 ) ||
       ( lbnd[ 3 ] != -50 ) ||
       ( ubnd[ 0 ] != 0 ) ||
       ( ubnd[ 1 ] != 10 ) ||
       ( ubnd[ 2 ] != 20 ) ||
       ( ubnd[ 3 ] != 30 ) ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRep( " ", "Error 11", status );
      }
   } else if( *status == SAI__OK ){
      for( i = 4; i < ARYK__MXDIM; i++ ) {
         if( lbnd[i] != 1 || ubnd[i] != 1 ) {
            *status = SAI__ERROR;
            errRep( " ", "Error 12", status );
         }
      }
   }

   arykClone( ary, &ary2, status );
   arykSame( ary, ary2, &same, &isect, status );
   if( !same && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 13", status );
   }

   arykCmplx( ary2, &ival, status );
   if( ival && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 14", status );
   }

   arykAnnul( &ary2, status );

   arykPtszD( ary, 2.0, -1.0, status );
   arykGtszD( ary, &scale, &zero, status );
   if( ( scale != 2.0 || zero != -1.0 ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h6", status );
   }
   arykForm( ary, form, status );
   if( strcmp( form, "SCALED") && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h7", status );
   }
   arykSctyp( ary, sctype, status );
   if( strcmp( sctype, "_UWORD") && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 4h8", status );
   }
   if( *status == SAI__OK ) {
      arykVerfy( ary, status );
      if( *status != SAI__OK ) {
         errRepf( " ", "Error 4h9", status );
      }
   }
   arykAnnul( &ary, status );




/* Test delta compression
   ====================== */

   arykTemp( &place, status );
   lbnd[ 0 ] = -10;
   lbnd[ 1 ] = -20;
   lbnd[ 2 ] = 0;
   ubnd[ 0 ] = 0;
   ubnd[ 1 ] = 10;
   ubnd[ 2 ] = 20;

   arykNew( "_INTEGER", 3, lbnd, ubnd, &place, &ary, status );
   arykMap( ary, "_INTEGER", "Write", (void **) &ipntr, &el, status );
   if( *status == SAI__OK ) {
      for( i = 0; i < el; i++ ) ipntr[i] = i;
   }
   arykUnmap( ary, status );

   arykDim( ary, ARYK__MXDIM, dims, &ndim, status );
   if( ndim != 3 || dims[0] != 11 || dims[1] != 31 || dims[2] != 21 ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRepf( " ", "Error 14b", status );
      }
   }

   arykTemp( &place, status );
   arykDelta( ary, 0, " ", 0.8, &place, &zratio, &ary2, status );

   arykForm( ary2, form, status );
   if( strcmp( form, "DELTA") && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 14c", status );
   }

   if( ( zratio < 3.09731 || zratio > 3.09733 ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 15", status );
   }

   arykGtdlt( ary2, &axis, type, &zratio, status );
   if( ( zratio < 3.09731 || zratio > 3.09733 ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 16", status );
   }
   if( strcmp( type, "_BYTE" ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 17 (%s)", status, type );
   }
   if( axis != 2 && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 18", status );
   }

   if( *status == SAI__OK ) {
      arykVerfy( ary2, status );
      if( *status != SAI__OK ) {
         errRepf( " ", "Error 18b", status );
      }
   }

   arykMap( ary2, "_INTEGER", "Read", (void **) &ipntr, &el2, status );

   if( *status == SAI__OK ) {
      if( el != el2 ) {
         *status = SAI__ERROR;
         errRepf( " ", "Error 19", status );
      }

      for( i = 0; i < el; i++ ) {
         if( ipntr[i] != i ) {
            *status = SAI__ERROR;
            errRepf( " ", "Error 20 (%d != %zu)", status, ipntr[i], i );
         }
      }
   }
   arykUnmap( ary2, status );

   if( *status == SAI__OK ) {
      arykReset( ary2, status );
      if( *status != ARYK__CMPAC ){
         if( *status == SAI__OK ) *status = SAI__ERROR;
         errRepf( " ", "Error 21", status );
      } else{
         errAnnul( status );
      }
   }

   arykBad( ary, 1, &bad, status );
   if( bad && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 22", status );
   }

   arykSbad( 1, ary, status );
   arykBad( ary, 0, &bad, status );
   if( !bad && *status == SAI__OK ){
      *status = SAI__ERROR;
      errRepf( " ", "Error 23", status );
   }

   lbnd[ 0 ] = -10;
   lbnd[ 1 ] = -10;
   lbnd[ 2 ] = 0;
   ubnd[ 0 ] = 10;
   ubnd[ 1 ] = 10;
   ubnd[ 2 ] = 20;
   arykSbnd( 3, lbnd, ubnd, ary, status );
   arykBound( ary, 3, lbnd, ubnd, &ndim, status );
   if( ( lbnd[ 0 ] != -10 ) ||
       ( lbnd[ 1 ] != -10 ) ||
       ( lbnd[ 2 ] != 0 ) ||
       ( ubnd[ 0 ] != 10 ) ||
       ( ubnd[ 1 ] != 10 ) ||
       ( ubnd[ 2 ] != 20 ) ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRep( " ", "Error 23a", status );
      }
   }

   arykState( ary, &defined, status );
   if( !defined && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 24", status );
   }
   arykReset( ary, status );
   arykState( ary, &defined, status );
   if( defined && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 25", status );
   }

   shift[ 0 ] = 1;
   shift[ 1 ] = -2;
   arykShift( 2, shift, ary, status );
   arykBound( ary, 3, lbnd, ubnd, &ndim, status );
   if( ( lbnd[ 0 ] != -9 ) ||
       ( lbnd[ 1 ] != -12 ) ||
       ( lbnd[ 2 ] != 0 ) ||
       ( ubnd[ 0 ] != 11 ) ||
       ( ubnd[ 1 ] != 8 ) ||
       ( ubnd[ 2 ] != 20 ) ) {
      if( *status == SAI__OK ) {
         *status = SAI__ERROR;
         errRep( " ", "Error 26", status );
      }
   }

   arykSize( ary, &el, status );
   if( el !=
       ( ubnd[ 0 ] - lbnd[ 0 ] + 1 )*
       ( ubnd[ 1 ] - lbnd[ 1 ] + 1 )*
       ( ubnd[ 2 ] - lbnd[ 2 ] + 1 ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 27", status );
   }

   arykStype( "COMPLEX_REAL", ary, status );
   arykFtype( ary, ftype, status );
   if( strcmp( ftype, "COMPLEX_REAL" ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 28", status );
   }

   arykType( ary, type, status );
   if( strcmp( type, "_REAL" ) && *status == SAI__OK ) {
      *status = SAI__ERROR;
      errRepf( " ", "Error 29", status );
   }

   arykAnnul( &ary2, status );
   arykAnnul( &ary, status );

   errRlse();

   return *status;
}





void *threadLocking( void *data ) {
   threadData *tdata = (threadData *) data;
   Ary *ary = tdata->ary;
   hdsdim dim[7];
   int ndim;
   int status = SAI__OK;

   if( tdata->test == 1 ) {
      arykDim( ary, 7, dim, &ndim, &status );

   } else if( tdata->test == 2 ) {
      arykLock( ary, 1, &status );
      arykDim( ary, 7, dim, &ndim, &status );

   } else if( tdata->test == 3 ) {
      arykDim( ary, 7, dim, &ndim, &status );

   } else if( tdata->test == 4 ) {
      arykLock( ary, 1, &status );
      arykDim( ary, 7, dim, &ndim, &status );
      arykUnlock( ary, &status );


   }
   return NULL;
}



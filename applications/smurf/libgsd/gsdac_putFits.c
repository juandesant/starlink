/*
*+
*  Name:
*     gsdac_putFits.c

*  Purpose:
*     Retrieve values from the GSD headers to fill the FITS headers
*     in the ACSIS file.

*  Language:
*     Starlink ANSI C

*  Type of Module:
*     Subroutine

*  Invocation:
*     gsdac_putFits ( const gsdVars *gsdVars, const int subBandNum,
*                     const int nSubsys, const int obsNum, 
*                     const int utDate, const int nSteps, 
*                     const char *backend, const int recepsUsed,
*                     char *recepNames[], const char *samMode, 
*                     const char *obsType, const dateVars *dateVars,
*                     const mapVars *mapVars, const double *lineFreqs,
*                     const double *IFFreqs,
*                     AstFitsChan *fitschan, int *status )

*  Arguments:
*     gsdVars = const gsdVars* (Given)
*        GSD file access parameters.
*     subBandNum = const int (Given)
*        Subband number.
*     nSubsys = const int (Given)
*        Number of subsystems.
*     obsNum = const int (Given)
*        Observation number.
*     utDate = const int (Given)
*        UT observation date.
*     nSteps = const int (Given)
*        Number of time steps in observation.
*     backend = const char* (Given)
*        Name of the backend.
*     recepsUsed = const int (Given)
*        Number of receptors actually used.
*     recepnames = char*[] (Given)
*        Names of receptors.
*     samMode = const char* (Given)
*        Sample mode.
*     obsType = const char* (Given)
*        Observation type.
*     dateVars = const dateVars* (Given)
*        Date and time variables.
*     mapVars = const mapVars* (Given)
*        Map/Chop/Scan variables.
*     lineFreqs = const double* (Given)
*        Frequencies for line transitions for each subband.
*     IFFreqs = const double* (Given)
*        IFs for each subband.
*     fitschan = AstFitsChan* (Given and Returned)
*        FITS headers.
*     status = int* (Given and Returned)
*        Pointer to global status.  

*  Description:
*     This routine fills the AstFitsChan with the values retrieved
*     from the GSD headers.  

*  Authors:
*     J.Balfour (UBC)
*     Tim Jenness (JAC)
*     {enter_new_authors_here}

*  History :
*     2008-02-04 (JB):
*        Original.
*     2008-02-14 (JB):
*        Use gsdVars struct to store headers/arrays.
*     2008-02-19 (JB):
*        Pass in gsdWCS struct instead of JCMTState.
*     2008-02-21 (JB):
*        Fix integration time calculation.
*     2008-02-28 (JB):
*        Replace subsysNum with subBandNum.
*     2008-02-28 (JB):
*        Move getDateVars and getMapVars to wrtData.
*     2008-03-04 (JB):
*        Use number of scans actually completed.
*     2008-03-06 (JB):
*        Save calculation of am, az, el start/end till after
*        all spectra completed.
*     2008-03-07 (JB):
*        Use IFFreq of 4.0, and set refChan as offset channel.
*     2008-03-19 (JB):
*        Removed unused variables.
*     2008-03-24 (JB):
*        Calculate intTime and nMix.
*     2008-03-28 (JB):
*        Find molecule and transition using LUT.
*     2008-03-31 (JB):
*        Call smf_get_moltrans to get transition info.
*     2008-04-02 (JB):
*        Fixed typo in OBSIDSS.
*     2008-04-03 (JB):
*        Fix pressure and steptime.
*     2008-04-11 (JB):
*        Remove wcs argument (not needed).
*     2008-04-11 (JB):
*        Add NCHNSUBS FITS header.
*     2008-04-14 (JB):
*        Add a few missing FITS headers.
*     2008-04-16 (JB):
*        Calculate OBSGEO values from -OBS values.
*     2008-04-18 (JB):
*        For special configs use centreFreq for moltrans.
*     2008-04-22 (JB):
*        Add BUNIT header for compatability with specwrite.
*     2008-04-23 (JB):
*        Use frequencies from matchFreqs for refchan/IF.
*     2008-04-25 (JB):
*        INSTAP_X = INSTAP_Y = 0.0.
*     2008-04-30 (JB):
*        Correct FFT_WIN and BEDEFAC.
*     2008-06-24 (TIMJ):
*        Fix compiler warnings.
*     2008-12-02 (DSB):
*        Use astSetFitsU to set undefined values.
*     2009-07-08 (TIMJ):
*        Use astSetFitsCM instead of astSetFitsCN

*  Copyright:
*     Copyright (C) 2008-2009 Science and Technology Facilities Council.
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
*-
*/

/* Standard includes */
#include <string.h>
#include <ctype.h>

/* Starlink includes */
#include "sae_par.h"
#include "mers.h"
#include "cnf.h"

/* SMURF includes */
#include "smurf_par.h"
#include "libsmf/smf.h"
#include "gsdac.h"
#include "gsdac_standard_sources.h"

#define FUNC_NAME "gsdac_putFits"

void gsdac_putFits ( const gsdVars *gsdVars, const int subBandNum,
                     const int nSubsys, const int obsNum, 
                     const int utDate, const int nSteps, 
                     const char *backend, const int recepsUsed, 
                     char *recepNames[], const char *samMode,
                     const char *obsType, const dateVars *dateVars,
                     const mapVars *mapVars, const double *lineFreqs,
                     const double *IFFreqs,
                     AstFitsChan *fitschan, int *status )

{

  /* Local variables */

  char bwMode[SZFITSCARD];    /* ACSIS total bandwidth setup */
  char curChar;               /* character pointer */
  int day;                    /* days for time conversion. */
  char doppler[SZFITSCARD];   /* doppler velocity definition */
  double etal;                /* telescope efficiency */
  int hour;                   /* hours for time conversion. */
  int i;                      /* loop counter */
  float IFchanSp;             /* TOPO IF channel spacing (Hz) */
  char instrume[SZFITSCARD];  /* front-end receiver */
  double intTime;             /* total time spent integrating (s) */
  int josMin;                 /* ?? */
  int min;                    /* minutes for time conversion. */
  const char *molecule;       /* target molecular species */
  int month;                  /* months for time conversion. */
  int nMix;                   /* number of mixers */
  double nRefStep;            /* number of nod sets repeated */
  char object[SZFITSCARD];    /* object of interest */
  char object2[SZFITSCARD];   /* object of interest (second half of name) */
  double obsgeo[3];           /* cartesian coordinates of telescope */
  char obsIDSS[SZFITSCARD];   /* unique observation number + subsystem
                                 number in format
                                 INSTR_NNNNN_YYYYMMDDTHHMMSS_N */
  char obsSB[SZFITSCARD];     /* observed sideband */
  int parse;                  /* flag for incorrect date string. */
  char recptors[SZFITSCARD];  /* active FE receptor IDs for this obs */
  double refChan;             /* reference IF channel no. */
  char seeDatSt[SZFITSCARD];  /* time of seeingSt in format
                                 YYYY-MM-DDTHH:MM:SS */
  char sSysObs[SZFITSCARD];   /* spectral ref. frame during observation */
  int standard;               /* true for spectral line standards */
  int startIdx;               /* index in pattern at start of observation */
  int stBetRef;               /* max number of steps between refs */
  int stBetRef_defined;       /* Use the stBetRef value? */
  double stepTime;            /* RTS step time */
  char subBands[SZFITSCARD];  /* ACSIS sub-band set-up */
  char tauDatSt[SZFITSCARD];  /* time of tau225St observation in 
                                 format YYYY-MM-DDTHH:MM:SS */
  const char *transiti;       /* target transition for molecule */
  int year;                   /* year for time conversion. */  

  /* Check inherited status */
  if ( *status != SAI__OK ) return;

  /* Get the cartesian coordinates of the telescope's location. */
  smf_terr( gsdVars->telLatitude * DD2R, gsdVars->telHeight * 1000.0, 
            -gsdVars->telLongitude * DD2R, obsgeo );  

  /* Get the telescope efficiency and convert from percentage to decimal */
  etal = gsdVars->etal / 100.0; 


  /* Obs Id, Date, Pointing Info */
 
  /* Truncate the object names and concatenate. */
  cnfImprt ( gsdVars->object1, 16, object );

  if ( strncmp ( gsdVars->object2, " ", 1 ) != 0 ) {

    cnfImprt ( gsdVars->object2, 16, object2 );
    strcat ( object, ", " ); 
    strcat ( object, object2 );

  }

  /* Determine if this is a spectral line standard. */
  /* Loop through the array of standards and see if the object matches
     one of them. */
  standard = 0;
  i = 0;
  while ( strcmp ( standards[i], "" ) != 0 ) {
    if ( strcmp ( object, standards[i] ) == 0 ) {
      standard = 1;
    }
    i++;
  }

  /* Copy the obsID into the obsIDSS and add the subsystem number. */
  sprintf ( obsIDSS, "%s_%i", dateVars->obsID, 
            subBandNum % nSubsys + 1 );


  /* Integration time related. */

  /* Get the sum of the integration times.  This is either the
     sum of the elements of the intTimes table for grids, or 
     the scan time times the number of scans for rasters. */
  if ( gsdVars->obsContinuous ) {  

    intTime = gsdVars->scanTime * gsdVars->nScan;

  } else {

    i = 0;
    intTime = 0.0;
    for ( i = 0; i < gsdVars->nScan; i++ ) {
      intTime += gsdVars->intTimes[i];
    }

  }


  /* ACSIS Specific. */

  /* Get the molecule and transition. */
  smf_get_moltrans ( lineFreqs[subBandNum] * 1000.0, &molecule, &transiti,
                     status );

  /* Get the bandwidth setup. */
/***** NOTE: may be different for rxb widebands *****/  
  sprintf ( bwMode, "%iMHzx%i", (int)(gsdVars->bandwidths[subBandNum]), 
            gsdVars->BEChans[subBandNum] );

/***** NOTE: Possibly undef? *****/
  strcpy ( subBands, bwMode );

  /* Get the reference channel (offset from channel which contains
     IFFreq). */
  refChan = ( (double)( gsdVars->BEChans[subBandNum] ) / 2.0 ) +
            ( ( IFFreqs[subBandNum] - gsdVars->totIFs[subBandNum] ) /
              ( fabs(gsdVars->freqRes[subBandNum]) / 1000.0 ) );

  IFchanSp = gsdVars->freqRes[subBandNum] * 1000000.0;


  /* FE Specific. */

  /* Truncate the name of the frontend. */
  cnfImprt ( gsdVars->frontend, 16, instrume );

  /* Get the number of mixers, 2 for RXB & RXW, 1 for others. */
  if ( strncmp ( "RXB", gsdVars->frontend, 3 ) == 0 ||
       strncmp ( "RXW", gsdVars->frontend, 3 ) == 0 ) {
    nMix = 2;
  } else {
    nMix = 1;
  }

  /* Get the observed sideband (-ve value = LSB, +ve value = USB ). */
  if ( gsdVars->sbSigns[subBandNum] > 0 ) strcpy ( obsSB, "USB" );
  else strcpy ( obsSB, "LSB" );

  /* Get the names of the receptors. */
  strcpy ( recptors, recepNames[0] );
  for ( i = 1; i < recepsUsed; i++ ) {
    strcat ( recptors, " " );
    strcat ( recptors, recepNames[i] );   
  }

  /* Truncate the doppler velocity definition and set
     to lowercase. */
  cnfImprt ( gsdVars->velDefn, 16, doppler );
  i = 0;
  curChar = doppler[i];
  while ( curChar != '\0' ) {
    doppler[i] = tolower(curChar);
    i++;
    curChar = doppler[i];
  }  

/***** NOTE : Possibly comes from VEL_REF (c12vref). */
  strcpy ( sSysObs, "TOPOCENT" );


  /* Environmental data. */

  /* Convert dates from YYMMDDHHMMSS to 
     YYYY-MM-DDTHH:MM:SS. */
  parse = sscanf ( gsdVars->tauTime, "%02d%02d%02d%02d%02d", &year, 
                   &month, &day, &hour, &min );

  if ( parse == 0 || parse == EOF ) {

    msgOut ( FUNC_NAME, "Couldn't convert CSO tau time, continuing anyway.", status );
    strcpy ( tauDatSt, "" );

  } else {
    
    /* Inelegant method to get YYYY from YY. */
    if ( year > 70 ) year = year + 1900;
    else year = year + 2000;
 
    sprintf ( tauDatSt, "%04d-%02d-%02dT%02d:%02d:00", 
              year, month, day, hour, min );

  }


  /* Convert dates from YYMMDDHHMMSS to 
     YYYY-MM-DDTHH:MM:SS. */
  parse = sscanf ( gsdVars->seeTime, "%02d%02d%02d%02d%02d", &year, 
                   &month, &day, &hour, &min );

  if ( parse == 0 || parse == EOF ) {
    msgOutif(MSG__VERB," ",
	     "Couldn't convert seeing time, continuing anyway.", status);
    strcpy ( seeDatSt, "" );
  } else {
    
    /* Kludge to get YYYY from YY. */
    if ( year > 70 ) year = year + 1900;
    else year = year + 2000;

    sprintf ( seeDatSt, "%04d-%02d-%02dT%02d:%02d:00", 
           year, month, day, hour, min );

  }

  /* JOS parameters */

  /* STEPTIME is SCAN_TIME for grids, and SCAN_TIME divided by the
     number of points in a scan for rasters. */
  if ( strncmp ( samMode, "raster", 6 ) == 0 ) {
  
    if ( strncmp ( gsdVars->obsDirection, "HORIZONTAL", 10 ) == 0 )
      stepTime = gsdVars->scanTime / gsdVars->nMapPtsX;
    else
      stepTime = gsdVars->scanTime / gsdVars->nMapPtsY;

  } else {

    stepTime = gsdVars->scanTime;

  }


  /* Get the JOS_MIN (1 for raster, for sample this is the number
     of STEPTIME integrations coadded into a single spectrum. */
  if ( strcmp ( samMode, "sample" ) == 0 )
    josMin = gsdVars->nScan;
  else
    josMin = 1;

  /* Get the length of time in the reference, and the number
     of steps between references. set up defaults first to avoid compiler
     warnings. */
  nRefStep = (double)(gsdVars->scanTime);
  stBetRef = 1;

  if ( gsdVars->obsContinuous ) {

    /* For rasters, determine the number of time scanning
       each point in one row from the total time for
       the row / number of points in the row.  The length
       of time in the reference is then sqrt (number of 
       points in the row) * (time per point). */
    nRefStep = sqrt ( (double)(gsdVars->nScanPts) ) * 
           ( (double)(gsdVars->scanTime) / (double)(gsdVars->nScanPts) );

    stBetRef = gsdVars->nScanPts;

  }  

  /* Set a flag indicating if the stBetRef value is defined. */
  stBetRef_defined =   ( strcmp ( mapVars->swMode, "chop" ) != 0 );

  /* Get the starting index into the pattern. */
  gsdac_getStartIdx ( gsdVars, samMode, &startIdx, status );

 

  /************************************/
  /*      WRITE OUT FITS HEADERS      */
  /************************************/

  astSetFitsS ( fitschan, "TELESCOP", gsdVars->telName, 
	        "Name of Telescope", 0 );

  astSetFitsS ( fitschan, "ORIGIN", "Joint Astronomy Centre, Hilo", 
                "Origin of file", 0 );

  astSetFitsF ( fitschan, "ALT-OBS", gsdVars->telHeight * 1000.0, 
                "[m] Height of observation above sea level", 0 );

  astSetFitsF ( fitschan, "LAT-OBS", gsdVars->telLatitude,
                "[deg] Latitude of Observatory", 0 );

  astSetFitsF ( fitschan, "LONG-OBS", -(gsdVars->telLongitude),
                "[deg] East longitude of Observatory", 0 );

  astSetFitsF ( fitschan, "OBSGEO-X", obsgeo[0],
                "[m]", 0 );  

  astSetFitsF ( fitschan, "OBSGEO-Y", obsgeo[1],
                "[m]", 0 );  

  astSetFitsF ( fitschan, "OBSGEO-Z", obsgeo[2],
                "[m]", 0 ); 

  astSetFitsF ( fitschan, "ETAL", etal,
                "Telescope efficiency", 0 ); 


  /* OMP and ORAC-DR Specific */
  astSetFitsCM ( fitschan,
                 "---- OMP and ORAC-DR Specific ----", 0 );

  astSetFitsS ( fitschan, "PROJECT", gsdVars->project, 
	        "PATT number", 0 );

/***** NOTE: possiby REDUCE_POINTING for spectral 5 points *****/
  astSetFitsS ( fitschan, "RECIPE", "REDUCE_SCIENCE", 
	        "ORAC-DR recipe", 0 );

  astSetFitsU ( fitschan, "DRGROUP", "Data Reduction group ID", 0 );

  astSetFitsU ( fitschan, "MSBID", "ID of minimum schedulable block", 0 );

  astSetFitsU ( fitschan, "MSBTID", "Transaction ID of MSB", 0 );

  astSetFitsU ( fitschan, "SURVEY", "Survey Name", 0 );

  astSetFitsU ( fitschan, "RMTAGENT", "name of Remote Agent", 0 );

  astSetFitsU ( fitschan, "AGENTID", "Unique identifier for remote agent", 0 );


  /* Obs Id, Date, Pointing Info */
  astSetFitsCM ( fitschan,
                 "---- Obs Id, Date, pointing Info ----", 0 );

  astSetFitsS ( fitschan, "OBJECT", object, 
	        "Object of interest", 0 );

  astSetFitsL ( fitschan, "STANDARD", standard, 
                "True if the spectral line is a standard", 0 );

  astSetFitsI ( fitschan, "OBSNUM", obsNum, 
                "Observation number", 0 );

  astSetFitsI ( fitschan, "NSUBSCAN", 1,
		"Sub-scan number", 0 );

  astSetFitsL ( fitschan, "OBSEND", 1, 
                "True if the file is the last in current observation", 0 );
  
  astSetFitsI ( fitschan, "UTDATE", utDate, 
                "UT Date as integer in yyyymmdd format", 0 );

  astSetFitsS ( fitschan, "DATE-OBS", dateVars->dateObs, 
                "UTC Datetime of start of observation", 0 );

  astSetFitsS ( fitschan, "DATE-END", dateVars->dateEnd, 
                "UTC Datetime of end of observation", 0 );

  astSetFitsF ( fitschan, "DUT1", gsdVars->obsUT1C, 
                "[d] UT1-UTC correction", 0 );

  astSetFitsS ( fitschan, "OBSID", dateVars->obsID, 
                "Unique observation identifier", 0 );

  astSetFitsS ( fitschan, "OBSIDSS", obsIDSS, 
                "Unique observation + subsystem ID", 0 );

/***** NOTE: possibly same as REFRECEP *****/
  astSetFitsU ( fitschan, "INSTAP", "Receptor at tracking centre (if any)", 0 );

  astSetFitsF ( fitschan, "INSTAP_X", 0.0,
                "[arcsec] Aperture X off. rel. to instr centre", 0 );

  astSetFitsF ( fitschan, "INSTAP_Y", 0.0,
                "[arcsec] Aperture Y off. rel. to instr centre", 0 );

  /* The following 6 cards are just placeholders and will be
     updated later. */
  astSetFitsU ( fitschan, "AMSTART", "Airmass at start of observation", 0 );

  astSetFitsU ( fitschan, "AMEND", "Airmass at end of observation", 0 );

  astSetFitsU ( fitschan, "AZSTART", "[deg] Azimuth at start of observation", 0 );

  astSetFitsU ( fitschan, "AZEND", "[deg] Azimuth at end of observation", 0 );

  astSetFitsU ( fitschan, "ELSTART", "[deg] Elevation at start of observation", 0 );

  astSetFitsU ( fitschan, "ELEND", "[deg] Elevation at end of observation", 0 );

  astSetFitsS ( fitschan, "HSTSTART", dateVars->HSTstart,
                "HST at start of observation", 0 );

  astSetFitsS ( fitschan, "HSTEND", dateVars->HSTend,
                "HST at end of observation", 0 );

  astSetFitsS ( fitschan, "LSTSTART", dateVars->LSTstart,
                "LST at start of observation", 0 );

  astSetFitsS ( fitschan, "LSTEND", dateVars->LSTend,
                "LST at end of observation", 0 );

  /* Integration time related. */
  astSetFitsCM ( fitschan,
                 "---- Integration time related ----", 0 );

  astSetFitsF ( fitschan, "INT_TIME", intTime,
                "Time spent integrating, entire", 0 );


  /* ACSIS Specific. */
  astSetFitsCM ( fitschan,
                 "---- ACSIS Specific ----", 0 );

  astSetFitsS ( fitschan, "BACKEND", backend,
                "Name of the backend", 0 );

  astSetFitsS ( fitschan, "MOLECULE", molecule,
                "Target molecular species", 0 );

  astSetFitsS ( fitschan, "TRANSITI", transiti,
                "Target transition for MOLECULE", 0 );

  astSetFitsU ( fitschan, "DRRECIPE", "ACSIS-DR recipe name", 0 );

  astSetFitsS ( fitschan, "BWMODE", bwMode,
                "Bandwidth setup", 0 );

  astSetFitsI ( fitschan, "SUBSYSNR", ( subBandNum % nSubsys ) + 1,
                "Sub-system number", 0 );

  astSetFitsS ( fitschan, "SUBBANDS", bwMode,
                "Sub-band setup", 0 );

  astSetFitsI ( fitschan, "NSUBBAND", 1, 
                "Number of subbands", 0 );

  astSetFitsF ( fitschan, "SUBREFP1", refChan, 
                "Reference channel for subband1", 0 );

  astSetFitsU ( fitschan, "SUBREFP2", "Reference channel for subband2", 0 );

  astSetFitsI ( fitschan, "NCHNSUBS", gsdVars->BEChans[subBandNum], 
                "Number of subbands", 0 );

  astSetFitsI ( fitschan, "REFCHAN", refChan, 
                "Reference IF channel No.", 0 );

  astSetFitsF ( fitschan, "IFCHANSP", IFchanSp,
                "[Hz] TOPO IF channel spacing (signed)", 0 ); 

  astSetFitsS ( fitschan, "FFT_WIN", "truncate", 
	        "Type of window used for FFT", 0 );

  if ( strncmp ( gsdVars->backend, "DAS", 3 ) == 0 ) {

    astSetFitsF ( fitschan, "BEDEGFAC", 1.15, 
	          "Backend degradation factor", 0 );

  } else if ( strncmp ( gsdVars->backend, "AOSC", 4 ) == 0 ) {

    astSetFitsF ( fitschan, "BEDEGFAC", 1.0, 
	          "Backend degradation factor", 0 );

  } else {

    astSetFitsU ( fitschan, "BEDEGFAC", "Backend degradation factor", 0 ); 

  }

  astSetFitsU ( fitschan, "MSROOT", "Root name of raw measurement sets", 0 ); 


  /* FE Specific. */
  astSetFitsCM ( fitschan,
                 "---- FE Specific ----", 0 );

  astSetFitsS ( fitschan, "INSTRUME", instrume, 
	        "Front-end receiver", 0 ); 

  astSetFitsS ( fitschan, "SB_MODE", gsdVars->sbMode, 
	        "Sideband mode", 0 ); 

  astSetFitsF ( fitschan, "IFFREQ", IFFreqs[subBandNum],
                "[GHz] IF Frequency", 0 );

  astSetFitsI ( fitschan, "N_MIX", nMix, 
                "No. of mixers", 0 );

  astSetFitsS ( fitschan, "OBS_SB", obsSB, 
		"The observed sideband", 0 );

  astSetFitsF ( fitschan, "LOFREQS", gsdVars->LOFreqs[subBandNum],
		"[GHz] LO Frequency at start of obs", 0 );

  astSetFitsF ( fitschan, "LOFREQE", gsdVars->LOFreqs[subBandNum],
                "[GHz] LO Frequency at end of obs", 0 );

  astSetFitsS ( fitschan, "RECPTORS", recptors,
                "Active FE receptor IDs for this obs", 0 );

  astSetFitsS ( fitschan, "REFRECEP", recepNames[0], 
                "Receptor with unit sensitivity", 0 );

  if ( strcmp ( samMode, "sample" ) == 0 ) {
    astSetFitsU ( fitschan, "MEDTSYS", "[K] Median of the T-sys across all receptors", 0 );
  } else {
    astSetFitsF ( fitschan, "MEDTSYS", 
                  gsdVars->sourceSysTemps[subBandNum],
		  "[K] Median of the T-sys across all receptors", 0 );
  }

  astSetFitsS ( fitschan, "TEMPSCAL", "TA*", 
                "Temperature scale in use", 0 );

  astSetFitsS ( fitschan, "DOPPLER", doppler,
                "Doppler velocity definition", 0 );

  astSetFitsS ( fitschan, "SSYSOBS", sSysObs,
                "Spectral ref. frame during observation", 0 );


  /* Environmental data. */
  astSetFitsCM ( fitschan,
                 "---- Environmental Data ----", 0 );

  astSetFitsF ( fitschan, "ATSTART", gsdVars->tamb, 
                "[degC] Air temp at start of observation", 0 );

  astSetFitsF ( fitschan, "ATEND", gsdVars->tamb, 
                "[degC] Air temp at end of observation", 0 );

  astSetFitsF ( fitschan, "HUMSTART",gsdVars->hamb , 
                "Rel Humidity at observation start", 0 );

  astSetFitsF ( fitschan, "HUMEND", gsdVars->hamb, 
                "Rel Humidity observation end", 0 );

  astSetFitsF ( fitschan, "BPSTART", gsdVars->pamb, 
                "[mbar] Pressure at observation start", 0 );

  astSetFitsF ( fitschan, "BPEND", gsdVars->pamb, 
                "[mbar] Pressure at observation end", 0 );

  astSetFitsU ( fitschan, "WNDSPDST", "[km/h] Wind Speed at obs start", 0 );
  
  astSetFitsU ( fitschan, "WNDSPDEN", "[km/h] Wind Speed at obs end", 0 );

  astSetFitsU ( fitschan, "WNDDIRST", "[deg] Wind direction, azimuth at obs start", 0 );

  astSetFitsU ( fitschan, "WNDDIREN", "[deg] Wind direction, azimuth at obs end", 0 );

  astSetFitsF ( fitschan, "TAU225ST", gsdVars->tau225,
		"Tau at 225 GHz from CSO at start", 0 );

  astSetFitsF ( fitschan, "TAU225EN", gsdVars->tau225,
		"Tau at 225 GHz from CSO at end", 0 );

  astSetFitsS ( fitschan, "TAUDATST", tauDatSt,
		"Time of TAU225ST observation", 0 );

  astSetFitsS ( fitschan, "TAUDATEN", tauDatSt,
		"Time of TAU225EN observation", 0 );

  astSetFitsS ( fitschan, "TAUSRC", "CSO225GHZ",
		"Source of the TAU225 value", 0 );

  astSetFitsU ( fitschan, "WVMTAUST", "186GHz Tau from JCMT WVM at start", 0 );

  astSetFitsU ( fitschan, "WVMTAUEN", "185GHz Tau from JCMT WVM at end", 0 );

  astSetFitsU ( fitschan, "WVMDATST", "Time of WVMTAUST", 0 );

  astSetFitsU ( fitschan, "WVMDATEN", "Time of WVMTAUEN", 0 );

  astSetFitsF ( fitschan, "SEEINGST", gsdVars->seeing,
		"[arcsec] SAO atmospheric seeing (start)", 0 );

  astSetFitsF ( fitschan, "SEEINGSEN", gsdVars->seeing,
		"[arcsec] SAO atmospheric seeing (end)", 0 );

  astSetFitsS ( fitschan, "SEEDATST", seeDatSt,
		"Date/Time of SEEINGST", 0 );

  astSetFitsS ( fitschan, "SEEDATEN", seeDatSt,
		"Date/Time of SEEINGEN", 0 );

  astSetFitsU ( fitschan, "FRLEGTST", "[degC] Mean Front leg temperature - Start", 0 );

  astSetFitsU ( fitschan, "FRLEGTEN", "[degC] Mean Front leg temperature - End", 0 );

  astSetFitsU ( fitschan, "BKLEGTST", "[degC] Mean Back leg temperature - Start", 0 );

  astSetFitsU ( fitschan, "BKLEGTEN", "[degC] Mean Back leg temperature - End", 0 );


  /* Switching and Map setup for the observation. */
  astSetFitsCM ( fitschan,
                 "---- Switching and Map setup for the observationi ----", 0 );

  astSetFitsS ( fitschan, "SAM_MODE", samMode, 
                "Sampling Mode", 0 );  

  astSetFitsS ( fitschan, "SW_MODE", mapVars->swMode,
                "Switch Mode", 0 );

  astSetFitsS ( fitschan, "SKYREFX", mapVars->skyRefX,
                "X co-ord of Reference Position", 0 );

  astSetFitsS ( fitschan, "SKYREFY", mapVars->skyRefY,
                "Y co-ord of Reference Position", 0 );   

  astSetFitsS ( fitschan, "OBS_TYPE", obsType,
		"Type of observation", 0 );

  if ( ( strcmp ( samMode, "grid" ) == 0
       && strcmp ( mapVars->swMode, "chop" ) == 0 ) ||
       strcmp ( samMode, "sample" ) == 0 ) {

    astSetFitsS ( fitschan, "CHOP_CRD", mapVars->chopCrd,
                  "Chopping co-ordinate system", 0 );

    astSetFitsF ( fitschan, "CHOP_FRQ", gsdVars->chopFrequency,
		  "[Hz] Chop frequency", 0 );

    astSetFitsF ( fitschan, "CHOP_PA", gsdVars->chopPA,
		  "[deg] Chop PA; 0=in lat, 90=in long", 0 );

    astSetFitsF ( fitschan, "CHOP_THR", gsdVars->chopThrow,
		  "[arcsec] Chop throw", 0 );

  } else {

    astSetFitsU ( fitschan, "CHOP_CRD", "Chopping co-ordinate system", 0 );

    astSetFitsU ( fitschan, "CHOP_FRQ", "[Hz] Chop frequency", 0 );

    astSetFitsU ( fitschan, "CHOP_PA", "[deg] Chop PA; 0=in lat, 90=in long", 0 );

    astSetFitsU ( fitschan, "CHOP_THR", "[arcsec] Chop throw", 0 );

  }

  astSetFitsU ( fitschan, "ROT_CRD", "Coordinate frame of image rotator", 0 );

  astSetFitsU ( fitschan, "ROT_PA", "[[deg] Angle of image rotator", 0 );

  astSetFitsU ( fitschan, "JIGL_CNT", "Number of offsets in jiggle pattern", 0 );

  astSetFitsU ( fitschan, "JIGL_NAM", "File containing the jiggle offsets", 0 );

  astSetFitsU ( fitschan, "JIG_PA", "[deg] Jiggle PA; 0=in lat, 90=in long", 0 );

  astSetFitsU ( fitschan, "JIG_CRD", "Jiggling co-ordinate system", 0 );

  astSetFitsU ( fitschan, "JIG_SCAL", "Scale size of jiggle pattern", 0 );

  if ( strcmp ( samMode, "raster" ) == 0
       && strcmp ( mapVars->swMode, "pssw" ) == 0 ) {

    astSetFitsF ( fitschan, "MAP_HGHT", mapVars->mapHght,
		  "[arcsec] Requested height of map", 0 );

    astSetFitsF ( fitschan, "MAP_PA", mapVars->mapPA,
		  "[deg] Requested PA of map", 0 );

    astSetFitsF ( fitschan, "MAP_WDTH", mapVars->mapWdth,
		  "[arcsec] Requested width of map", 0 );

    astSetFitsS ( fitschan, "LOCL_CRD", mapVars->loclCrd,
		  "Local offset/map PA co-ordinate system", 0 );

    astSetFitsF ( fitschan, "MAP_X", gsdVars->centreOffsetX,
		  "[arcsec] Requested map offset from telescope centre", 0 );

    astSetFitsF ( fitschan, "MAP_Y", gsdVars->centreOffsetY,
		  "[arcsec] Requested map offset from telescope centre", 0 );  

    astSetFitsS ( fitschan, "SCAN_CRD", mapVars->scanCrd,
		  "Co-ordinate system for scan", 0 );

    astSetFitsF ( fitschan, "SCAN_VEL", mapVars->scanVel,
		  "[arcsec/sec] Scan velocity along scan direction", 0 );

    astSetFitsF ( fitschan, "SCAN_DY", mapVars->scanDy,
		  "[arcsec] Scan spacing perp. to scan", 0 );

    astSetFitsS ( fitschan, "SCAN_PAT", mapVars->scanPat,
		  "Scan pattern name", 0 );

  } else {

    astSetFitsU ( fitschan, "MAP_HGHT", "[arcsec] Requested height of map", 0 );

    astSetFitsU ( fitschan, "MAP_PA", "[deg] Requested PA of map", 0 );

    astSetFitsU ( fitschan, "MAP_WDTH",  "[arcsec] Requested width of map", 0 );

    astSetFitsU ( fitschan, "LOCL_CRD", "Local offset/map PA co-ordinate system", 0 );

    astSetFitsU ( fitschan, "MAP_X", "[arcsec] Requested map offset from telescope centre", 0 );

    astSetFitsU ( fitschan, "MAP_Y", "[arcsec] Requested map offset from telescope centre", 0 );  

    astSetFitsU ( fitschan, "SCAN_CRD", "Co-ordinate system for scan", 0 );

    astSetFitsU ( fitschan, "SCAN_VEL", "[arcsec/sec] Scan velocity along scan direction", 0 );

    astSetFitsU ( fitschan, "SCAN_DY", "[arcsec] Scan spacing perp. to scan", 0 );

    astSetFitsU ( fitschan, "SCAN_PAT", "Scan pattern name", 0 );

  }


  /* SMU */
  astSetFitsCM ( fitschan,
                 "---- SMU ----", 0 );

  astSetFitsF ( fitschan, "ALIGN_DX", gsdVars->smuDX, 
		"SMU tables X axis focus offset", 0 );

  astSetFitsF ( fitschan, "ALIGN_DY", gsdVars->smuDY, 
		"SMU tables Y axis focus offset", 0 );

  astSetFitsF ( fitschan, "FOCUS_DZ", gsdVars->smuDZ, 
		"SMU tables Z axis focus offset", 0 );

  astSetFitsF ( fitschan, "DAZ", gsdVars->smuOffsEW, 
		"SMU azimuth pointing offset", 0 );

  astSetFitsF ( fitschan, "DEL", gsdVars->smuOffsNS, 
		"SMU elevation pointing offset", 0 );

  astSetFitsF ( fitschan, "UAZ", gsdVars->errAz, 
		"User azimuth pointing offset", 0 );

  astSetFitsF ( fitschan, "UEL", gsdVars->errEl, 
		"User elevation pointing offset", 0 );


  /* JOS parameters */
  astSetFitsCM ( fitschan,
                 "---- JOS parameters ----", 0 );

  astSetFitsF ( fitschan, "STEPTIME", stepTime, 
                "RTS step time during an RTS sequence", 0 ); 

  astSetFitsI ( fitschan, "NUM_CYC", gsdVars->nCycle, 
                "Number of times to repeat entire recipe", 0 );  

  astSetFitsI ( fitschan, "NUM_NODS", 1, 
                "Number of times to repeat nod set", 0 );

  astSetFitsU ( fitschan, "JOS_MULT", "", 0 );//k description

  astSetFitsI ( fitschan, "JOS_MIN", josMin, "", 0 );//k description

  astSetFitsU ( fitschan, "NCALSTEP", "Number of RTS steps for each CAL", 0 );

  astSetFitsF ( fitschan, "NREFSTEP", nRefStep,
		"Mean no. of RTS steps for each REF", 0 );

  if( stBetRef_defined ) {
     astSetFitsI ( fitschan, "STBETREF", stBetRef, 
   		   "Target number of RTS steps between REFs", 0 );
  } else {
     astSetFitsU ( fitschan, "STBETREF", "Target number of RTS steps between REFs", 0 );
  }

  astSetFitsU ( fitschan, "STBETCAL", "Target number of RTS steps between CALs",0 );

  astSetFitsI ( fitschan, "STARTIDX", startIdx,
		"Index into pattern at start of obs", 0 );

  astSetFitsU ( fitschan, "FOCAXIS", "Focus Axis to move (X, Y, Z)", 0 );

  astSetFitsU ( fitschan, "NFOCSTEP", "Number of focal position steps", 0 );

  astSetFitsU ( fitschan, "FOCSTEP", "Distance between focal steps", 0 );


  /* Miscellaneous parameters */
  astSetFitsCM ( fitschan,
                 "---- Miscellaneous ----", 0 );

  astSetFitsU ( fitschan, "OCSCFG", "OCS config filename", 0 );

  astSetFitsL ( fitschan, "SIMULATE", 0,
		"True if any data are simulated", 0 );

  astSetFitsL ( fitschan, "SIM_CORR", 0,
		"True if any CORRTASK is simualted", 0 );

  astSetFitsL ( fitschan, "SIM_SMU", 0,
		"True if SMU data is simulated", 0 );

  astSetFitsL ( fitschan, "SIM_TCS", 0,
		"True if TCS data is simulated", 0 );

  astSetFitsL ( fitschan, "SIM_RTS", 0,
		"True if RTS data is simulated", 0 );

  astSetFitsL ( fitschan, "SIM_IF", 0,
		"True if IF data is simulated", 0 );

  astSetFitsS ( fitschan, "STATUS", "NORMAL",
		"Status at end of observation", 0 );


  /* Rover=specific parameters */
  astSetFitsCM ( fitschan,
                 "---- ROVER Specific ----", 0 );

  astSetFitsL ( fitschan, "POL_CONN", 0,
		"True if ROVER is connected", 0 );

  astSetFitsU ( fitschan, "POL_MODE", "Step-and-integrate (STEPINT) or spinning (SPIN)", 0 );

  astSetFitsU ( fitschan, "ROTAFREQ", "[Hz] Spin frequency (if spinning)", 0 );

  astSetFitsU ( fitschan, "POL_CRD", "Coordinate frame of polarimeter angles", 0 );

  astSetFitsU ( fitschan, "POL_PA", "[[deg] Angle of pol fast axis", 0 );

  astSetFitsS ( fitschan, "BUNIT", "K", "", 0 );


}

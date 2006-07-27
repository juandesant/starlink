
#ifndef INCLUDED_JCMT_STATE_H
#define INCLUDED_JCMT_STATE_H

/*
*+
*  Name:
*     jcmt/state.h

*  Purpose:
*     Define the public C data structures representing JCMT STATE information

*  Invocation:
*     #include "jcmt/state.h"

*  Language:
*     C Include file

*  Description:
*     This include file defines the basic data structure used to pass STATE
*     information between functions. Additionally, it defines the sizes of string
*     members and the data types and names used by HDS (with ordering information).

*  Authors:
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     BDK: Dennis Kelly (UKATC, Edinburgh)

*  History:
*     17-AUG-2004 (BDK):
*        Original version for SCUBA-2 DA
*     27-FEB-2006 (TIMJ):
*        Original version for ACSIS specwriter
*     25-JUL-2006 (TIMJ):
*        Merge ACSIS and SCUBA-2 into a single include file.

*  Copyright:
*     Copyright (C) 2004, 2006 Particle Physics and Astronomy Research Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
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

*-
*/

/* Name of standard NDF extension for state information */

#define JCMT__EXTNAME  "JCMTSTATE"
#define JCMT__EXTTYPE  "RTS_ARR"

/* Some sizes used for dimensioning the string arrays */
#define JCMT__SZRTS_TASKS      80
#define JCMT__SZTCS_SOURCE     32
#define JCMT__SZTCS_TR_SYS     16
#define JCMT__SZACS_RECEPTOR    5
#define JCMT__SZACS_SOURCE_RO  16
#define JCMT__SZSMU_CHOP_PHASE  1
#define JCMT__SZTCS_BEAM        1

/* Flags indicating whether a component should be used in ACSIS or SCUBA-2. Instrument
   usability can be checked by AND'ing the value in hdsRecord array with one of these.
 */

#define INST__ACSIS   1
#define INST__SCUBA2  2

/* This struct represents all the information available from a JCMT SEQUENCE as published
   in STATE structures by each task involved. Not all will be defined. The struct ordering
   can be changed at any time and should never be relied upon.
*/
typedef struct JCMTState {
  unsigned int rts_num;
  double rts_end;    /* MJD TAI of end of sequence step */
  char   rts_tasks[JCMT__SZRTS_TASKS + 1];
  double smu_x;
  double smu_y;
  double smu_z;
  char   smu_chop_phase[JCMT__SZSMU_CHOP_PHASE+1];
  int    smu_jig_index;
  double smu_az_jig_x;
  double smu_az_jig_y;
  double smu_az_chop_x;
  double smu_az_chop_y;
  double smu_tr_jig_x;
  double smu_tr_jig_y;
  double smu_tr_chop_x;
  double smu_tr_chop_y;
  double tcs_airmass;
  double tcs_az_ang;
  double tcs_az_ac1;
  double tcs_az_ac2;
  double tcs_az_dc1;
  double tcs_az_dc2;
  double tcs_az_bc1;
  double tcs_az_bc2;
  char   tcs_beam[JCMT__SZTCS_BEAM+1];
  int    tcs_index;
  char   tcs_source[JCMT__SZTCS_SOURCE+1];
  char   tcs_tr_sys[JCMT__SZTCS_TR_SYS+1];
  double tcs_tr_ang;
  double tcs_tr_ac1;
  double tcs_tr_ac2;
  double tcs_tr_dc1;
  double tcs_tr_dc2;
  double tcs_tr_bc1;
  double tcs_tr_bc2;
  int    jos_drcontrol; /* JOS DR control tag */
  float  enviro_rel_hum;
  float  enviro_pressure;
  float  enviro_air_temp;
  float  wvm_th;
  float  wvm_t12;
  float  wvm_t42;
  float  wvm_t78;
  float  wvm_tw;
  int    wvm_qual;
  float  wvm_time;
  double sc2_heat;
  float  acs_exposure;
  int    acs_no_prev_ref;
  int    acs_no_next_ref;
  int    acs_no_ons;
  char   acs_source_ro[JCMT__SZACS_SOURCE_RO+1];
  double pol_ang;
  float  fts_pos;



  /* These are not related to the sequence as such but define the particular RECEPTOR
     that is being associated with the sequence. The ACSIS specwriter writes this information
     to a separate structure in the file and so could usefully be defined as a completely
     separate struct. */
  double acs_feedx;          /* X coordinate of feed "acs_feed" */
  double acs_feedy;          /* Y coordinate of feed "acs_feed" */
  int    acs_spec_window_id; /* SPW id used to assign subsystem */
  unsigned int acs_feed;     /* Feed number */
  float  acs_tsys;
  float  acs_trx;

  /* this should be passed into the acsSpecWriteTS explicitly */
  unsigned int rts_endnum;  /* Highest number expected in this sequence */

} JCMTState;

/* Generate indices to allow the programmer to access individual elements
   of the component definitions, guaranteeing uniqueness.
   Not certain that these need to be public since the hdsRecords array is designed to
   include the number.
*/

typedef enum
{
   RTS_NUM,
   RTS_END,
   RTS_TASKS,
   SMU_X,
   SMU_Y,
   SMU_Z,
   SMU_CHOP_PHASE,
   SMU_JIG_INDEX,
   SMU_AZ_JIG_X,
   SMU_AZ_JIG_Y,
   SMU_AZ_CHOP_X,
   SMU_AZ_CHOP_Y,
   SMU_TR_JIG_X,
   SMU_TR_JIG_Y,
   SMU_TR_CHOP_X,
   SMU_TR_CHOP_Y,
   TCS_AIRMASS,
   TCS_AZ_ANG,
   TCS_AZ_AC1,
   TCS_AZ_AC2,
   TCS_AZ_DC1,
   TCS_AZ_DC2,
   TCS_AZ_BC1,
   TCS_AZ_BC2,
   TCS_BEAM,
   TCS_INDEX,
   TCS_SOURCE,
   TCS_TR_SYS,
   TCS_TR_ANG,
   TCS_TR_AC1,
   TCS_TR_AC2,
   TCS_TR_DC1,
   TCS_TR_DC2,
   TCS_TR_BC1,
   TCS_TR_BC2,
   JOS_DRCONTROL,
   ENVIRO_REL_HUM,
   ENVIRO_PRESSURE,
   ENVIRO_AIR_TEMP,
   WVM_TH,
   WVM_T12,
   WVM_T42,
   WVM_T78,
   WVM_TW,
   WVM_QUAL,
   WVM_TIME,
   SC2_HEAT,
   ACS_EXPOSURE,
   ACS_NO_PREV_REF,
   ACS_NO_NEXT_REF,
   ACS_NO_ONS,
   ACS_SOURCE_RO,
   POL_ANG,
   FTS_POS,
   JCMT_COMP_NUM     /* Tells us the total number of available components */
} JCMT_HEADLIST;

/* Private Function to put quotes around a symbol so that we can do
   CPP string concatenation */
#define myxstr(s) mystr(s)
#define mystr(s) #s
#define CHARTYP(s) "_CHAR*" myxstr(s)

/* Define the names and types to be used in the HDS component and whether
   they should be used for ACSIS, SCUBA-2 or are common to all instruments.. */

/* To allow easy grouping, we define a struct */
typedef struct HDSdataRecord {
  int  position;    /* actual position in list. Used so that JCMT_HEADLIST is not forced to be in same order. */
  char * type;
  char * name;
  int  instrument;  /* bit mask indicating whether a particular instrument uses this field */
} HDSdataRecord;

static const HDSdataRecord hdsRecords[JCMT_COMP_NUM] =
  {
    { RTS_NUM, "_INTEGER", "RTS_NUM", INST__ACSIS | INST__SCUBA2 },
    { RTS_END, "_DOUBLE", "RTS_END", INST__ACSIS | INST__SCUBA2 },
    { RTS_TASKS, CHARTYP(JCMT__SZRTS_TASKS), "RTS_TASKS", INST__ACSIS | INST__SCUBA2 },
    { SMU_X, "_DOUBLE", "SMU_X", INST__ACSIS | INST__SCUBA2 },
    { SMU_Y, "_DOUBLE", "SMU_Y", INST__ACSIS | INST__SCUBA2 },
    { SMU_Z, "_DOUBLE", "SMU_Z", INST__ACSIS | INST__SCUBA2 },
    { SMU_CHOP_PHASE, CHARTYP(JCMT__SZSMU_CHOP_PHASE), "SMU_CHOP_PHASE", INST__ACSIS | INST__SCUBA2 },
    { SMU_JIG_INDEX, "_INTEGER", "SMU_JIG_INDEX", INST__ACSIS | INST__SCUBA2 },
    { SMU_AZ_JIG_X, "_DOUBLE", "SMU_AZ_JIG_X", INST__ACSIS | INST__SCUBA2 },
    { SMU_AZ_JIG_Y, "_DOUBLE", "SMU_AZ_JIG_Y", INST__ACSIS | INST__SCUBA2 },
    { SMU_AZ_CHOP_X, "_DOUBLE", "SMU_AZ_CHOP_X", INST__ACSIS | INST__SCUBA2 },
    { SMU_AZ_CHOP_Y, "_DOUBLE", "SMU_AZ_CHOP_Y", INST__ACSIS | INST__SCUBA2 },
    { SMU_TR_JIG_X, "_DOUBLE", "SMU_TR_JIG_X", INST__ACSIS | INST__SCUBA2 },
    { SMU_TR_JIG_Y, "_DOUBLE", "SMU_TR_JIG_Y", INST__ACSIS | INST__SCUBA2 },
    { SMU_TR_CHOP_X, "_DOUBLE", "SMU_TR_CHOP_X", INST__ACSIS | INST__SCUBA2 },
    { SMU_TR_CHOP_Y, "_DOUBLE", "SMU_TR_CHOP_Y", INST__ACSIS | INST__SCUBA2 },
    { TCS_AIRMASS, "_DOUBLE", "TCS_AIRMASS", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_ANG, "_DOUBLE", "TCS_AZ_ANG", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_AC1, "_DOUBLE", "TCS_AZ_AC1", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_AC2, "_DOUBLE", "TCS_AZ_AC2", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_DC1, "_DOUBLE", "TCS_AZ_DC1", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_DC2, "_DOUBLE", "TCS_AZ_DC2", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_BC1, "_DOUBLE", "TCS_AZ_BC1", INST__ACSIS | INST__SCUBA2 },
    { TCS_AZ_BC2, "_DOUBLE", "TCS_AZ_BC2", INST__ACSIS | INST__SCUBA2 },
    { TCS_BEAM, CHARTYP(JCMT__SZTCS_BEAM), "TCS_BEAM", INST__ACSIS | INST__SCUBA2 },
    { TCS_INDEX, "_INTEGER", "TCS_INDEX", INST__ACSIS | INST__SCUBA2 },
    { TCS_SOURCE, CHARTYP(JCMT__SZTCS_SOURCE), "TCS_SOURCE", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_SYS, CHARTYP(JCMT__SZTCS_TR_SYS), "TCS_TR_SYS", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_ANG, "_DOUBLE", "TCS_TR_ANG", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_AC1, "_DOUBLE", "TCS_TR_AC1", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_AC2, "_DOUBLE", "TCS_TR_AC2", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_DC1, "_DOUBLE", "TCS_TR_DC1", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_DC2, "_DOUBLE", "TCS_TR_DC2", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_BC1, "_DOUBLE", "TCS_TR_BC1", INST__ACSIS | INST__SCUBA2 },
    { TCS_TR_BC2, "_DOUBLE", "TCS_TR_BC2", INST__ACSIS | INST__SCUBA2 },
    { JOS_DRCONTROL, "_INTEGER", "JOS_DRCONTROL", INST__ACSIS | INST__SCUBA2 },
    { ENVIRO_REL_HUM, "_REAL", "ENVIRO_REL_HUM", INST__ACSIS },
    { ENVIRO_PRESSURE, "_REAL", "ENVIRO_PRESSURE", INST__ACSIS },
    { ENVIRO_AIR_TEMP, "_REAL", "ENVIRO_AIR_TEMP", INST__ACSIS },
    { WVM_TH, "_REAL", "WVM_TH", INST__SCUBA2 },
    { WVM_T12, "_REAL", "WVM_T12", INST__SCUBA2 },
    { WVM_T42, "_REAL", "WVM_T42", INST__SCUBA2 },
    { WVM_T78, "_REAL", "WVM_T78", INST__SCUBA2 },
    { WVM_TW, "_REAL", "WVM_TW", INST__SCUBA2 },
    { WVM_QUAL, "_INTEGER", "WVM_QUAL", INST__SCUBA2 },
    { WVM_TIME, "_REAL", "WVM_TIME", INST__SCUBA2 },
    { SC2_HEAT, "_DOUBLE", "SC2_HEAT", INST__SCUBA2 },
    { ACS_SOURCE_RO, CHARTYP(JCMT__SZACS_SOURCE_RO), "ACS_SOURCE_RO", INST__ACSIS },
    { ACS_NO_PREV_REF, "_INTEGER", "ACS_NO_PREV_REF", INST__ACSIS },
    { ACS_NO_NEXT_REF, "_INTEGER", "ACS_NO_NEXT_REF", INST__ACSIS },
    { ACS_NO_ONS, "_INTEGER", "ACS_NO_ONS", INST__ACSIS },
    { ACS_EXPOSURE, "_REAL", "ACS_EXPOSURE", INST__ACSIS },
    { POL_ANG, "_DOUBLE", "POL_ANG", INST__ACSIS | INST__SCUBA2 },
    { FTS_POS, "_REAL", "FTS_POS", INST__SCUBA2 }
  };


/* tidy up namespace */
#undef CHARTYP
#undef mystr
#undef myxstr

/* INCLUDE_JCMT_STATE_H */
#endif

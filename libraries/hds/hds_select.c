#include <stdlib.h>
#include <stdio.h>
#include "sae_par.h"
#include "dat_par.h"
#include "dat1.h"
#include "hds_types.h"
#include "ems.h"
#include "hds.h"
#include "dat_err.h"
#include "star/hds_v4.h"
#include "star/hds_v5.h"
#define ISHDSv5(loc) ((loc) && (loc)->hds_version >= 5)
#if DEBUG_HDS
#define HDS_CHECK_STATUS(func,txt) if (*status != instat && *status != SAI__OK) { emsRepf("wrap_" func, func ": Error in call to HDS %s", status, txt); printf("Bad status from %s %s: %d\n", func, txt, *status);}
static void EnterCheck( const char * func, int status ) { printf("Enter HDS routine: %s [%d]\n", func,status); }
#else
#  define HDS_CHECK_STATUS(func,txt) if (*status != instat && *status != SAI__OK) { emsRepf("wrap_" func, func ": Error in call to HDS %s", status, txt);}
#  define EnterCheck(A,B) ;
#endif

/*=================================*/
/* datAlter - Alter size of object */
/*=================================*/

int
datAlter(HDSLoc *locator, int ndim, const hdsdim dims[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datAlter",*status);
  if (isv5) {
    retval = datAlter_v5(locator, ndim, dims, status);
  } else {
    retval = datAlter_v4(locator, ndim, dims, status);
  }
  HDS_CHECK_STATUS("datAlter",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==========================*/
/* datAnnul - Annul locator */
/*==========================*/

int
datAnnul(HDSLoc **locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(*locator);
  EnterCheck("datAnnul",*status);
  if (isv5) {
    retval = datAnnul_v5(locator, status);
  } else {
    retval = datAnnul_v4(locator, status);
  }
  HDS_CHECK_STATUS("datAnnul",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==============================================*/
/* datBasic - Map data (in basic machine units) */
/*==============================================*/

int
datBasic(const HDSLoc *locator, const char *mode_c, unsigned char **pntr, size_t *len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datBasic",*status);
  if (isv5) {
    retval = datBasic_v5(locator, mode_c, pntr, len, status);
  } else {
    retval = datBasic_v4(locator, mode_c, pntr, len, status);
  }
  HDS_CHECK_STATUS("datBasic",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datCcopy - copy one structure level */
/*=====================================*/

int
datCcopy(const HDSLoc *locator1, const HDSLoc *locator2, const char *name, HDSLoc **locator3, int *status) {
  /* Requires special code */
  int instat = *status;
  int isv5 = 0;
  int loc1isv5 = 0;
  int loc2isv5 = 0;
  EnterCheck("datCcopy",*status);
  if (*status != SAI__OK) return *status;
  loc1isv5 = ISHDSv5(locator1);
  loc2isv5 = ISHDSv5(locator2);
  if (loc1isv5 && loc2isv5) {
    /* Just call the v5 code */
    isv5 = 1;
    datCcopy_v5(locator1, locator2, name, locator3, status);
  } else if ( !loc1isv5 && !loc2isv5 ) {
    isv5 = 0;
    datCcopy_v4(locator1, locator2, name, locator3, status);
  } else {
    if (loc1isv5) {
      datCcopy5to4(locator1, locator2, name, locator3, status);
    } else {
      datCcopy4to5(locator1, locator2, name, locator3, status);
    }
  }
  HDS_CHECK_STATUS("datCcopy",(isv5 ? "(v5)" : "(v4)"));
  return *status;
}

/*=======================================*/
/* datCctyp - construct _CHAR*nnn string */
/*=======================================*/

void
datCctyp(size_t size, char type[DAT__SZTYP+1]) {
  EnterCheck("datCctyp",-1);
  datCctyp_v5(size, type);
  return;
}


/*===========================================*/
/* datCell - Locate a "cell" (array element) */
/*===========================================*/

int
datCell(const HDSLoc *locator1, int ndim, const hdsdim subs[], HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datCell",*status);
  if (isv5) {
    retval = datCell_v5(locator1, ndim, subs, locator2, status);
  } else {
    retval = datCell_v4(locator1, ndim, subs, locator2, status);
  }
  HDS_CHECK_STATUS("datCell",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=================================================*/
/* datChscn - validate the supplied component name */
/*=================================================*/

int
datChscn(const char * name, int *status) {
  int retval = 0;
  int instat = *status;
  EnterCheck("datChscn",*status);
  if (*status != SAI__OK) return *status;
  retval = datChscn_v5(name, status);
  HDS_CHECK_STATUS("datChscn","(v5)");
  return retval;
}

/*==========================================*/
/* datClen - Obtain character string length */
/*==========================================*/

int
datClen(const HDSLoc *locator, size_t *clen, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datClen",*status);
  if (isv5) {
    retval = datClen_v5(locator, clen, status);
  } else {
    retval = datClen_v4(locator, clen, status);
  }
  HDS_CHECK_STATUS("datClen",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===========================*/
/* datClone - clone locator */
/*===========================*/

int
datClone(const HDSLoc *locator1, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datClone",*status);
  if (isv5) {
    retval = datClone_v5(locator1, locator2, status);
  } else {
    retval = datClone_v4(locator1, locator2, status);
  }
  HDS_CHECK_STATUS("datClone",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================*/
/* datCoerc - coerce object shape */
/*================================*/

int
datCoerc(const HDSLoc *locator1, int ndim, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datCoerc",*status);
  if (isv5) {
    retval = datCoerc_v5(locator1, ndim, locator2, status);
  } else {
    retval = datCoerc_v4(locator1, ndim, locator2, status);
  }
  HDS_CHECK_STATUS("datCoerc",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================*/
/* datCopy - copy object */
/*=======================*/

int
datCopy(const HDSLoc *locator1, const HDSLoc *locator2, const char *name_c, int *status) {
  /* Requires special code */
  int instat = *status;
  int isv5 = 0;
  int loc1isv5 = 0;
  int loc2isv5 = 0;
  EnterCheck("datCopy",*status);
  if (*status != SAI__OK) return *status;
  loc1isv5 = ISHDSv5(locator1);
  loc2isv5 = ISHDSv5(locator2);
  if (loc1isv5 && loc2isv5) {
    /* Just call the v5 code */
    isv5 = 1;
    datCopy_v5(locator1, locator2, name_c, status);
  } else if ( !loc1isv5 && !loc2isv5 ) {
    isv5 = 0;
    datCopy_v4(locator1, locator2, name_c, status);
  } else {
    if (loc1isv5) {
      datCopy5to4(locator1, locator2, name_c, status);
    } else {
      datCopy4to5(locator1, locator2, name_c, status);
    }
  }
  HDS_CHECK_STATUS("datCopy",(isv5 ? "(v5)" : "(v4)"));
  return *status;
}

/*============================================================*/
/* datDrep - Obtain primitive data representation information */
/*============================================================*/

int
datDrep(const HDSLoc *locator, char **format_str, char **order_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datDrep",*status);
  if (isv5) {
    retval = datDrep_v5(locator, format_str, order_str, status);
  } else {
    retval = datDrep_v4(locator, format_str, order_str, status);
  }
  HDS_CHECK_STATUS("datDrep",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datErase - Erase object                */
/*========================================*/

int
datErase(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datErase",*status);
  if (isv5) {
    retval = datErase_v5(locator, name_str, status);
  } else {
    retval = datErase_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datErase",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===========================================================*/
/* datErmsg - Translate a status value into an error message */
/*===========================================================*/

int
datErmsg(int status, size_t *len, char *msg_str) {
  EnterCheck("datErmsg",-1);
  return datErmsg_v5(status, len, msg_str);
}

/*================================*/
/* datFind - Find named component */
/*================================*/

int
datFind(const HDSLoc *locator1, const char *name_str, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datFind",*status);
  if (isv5) {
    retval = datFind_v5(locator1, name_str, locator2, status);
  } else {
    retval = datFind_v4(locator1, name_str, locator2, status);
  }
  HDS_CHECK_STATUS("datFind",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*============================*/
/* datGet - Read primitive(s) */
/*============================*/

int
datGet(const HDSLoc *locator, const char *type_str, int ndim, const hdsdim dims[], void *values, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet",*status);
  if (isv5) {
    retval = datGet_v5(locator, type_str, ndim, dims, values, status);
  } else {
    retval = datGet_v4(locator, type_str, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGet",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================*/
/* datGetC - Read _CHAR primitive(s) */
/*===================================*/

int
datGetC(const HDSLoc *locator, const int ndim, const hdsdim dims[], char values[], size_t char_len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetC",*status);
  if (isv5) {
    retval = datGetC_v5(locator, ndim, dims, values, char_len, status);
  } else {
    retval = datGetC_v4(locator, ndim, dims, values, char_len, status);
  }
  HDS_CHECK_STATUS("datGetC",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datGetD - Read _DOUBLE primitive(s) */
/*=====================================*/

int
datGetD(const HDSLoc *locator, int ndim, const hdsdim dims[], double values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetD",*status);
  if (isv5) {
    retval = datGetD_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetD_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetD",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================*/
/* datGetI - Read _INTEGER primitive(s) */
/*======================================*/

int
datGetI(const HDSLoc *locator, int ndim, const hdsdim dims[], int values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetI",*status);
  if (isv5) {
    retval = datGetI_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetI_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetI",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================*/
/* datGetK - Read _INT64 primitive(s) */
/*======================================*/

int
datGetK(const HDSLoc *locator, int ndim, const hdsdim dims[], int64_t values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetK",*status);
  if (isv5) {
    retval = datGetK_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetK_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetK",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================*/
/* datGetW - Read _WORD primitive(s) */
/*===================================*/

int
datGetW(const HDSLoc *locator, int ndim, const hdsdim dims[], short values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetW",*status);
  if (isv5) {
    retval = datGetW_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetW_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================*/
/* datGetUW - Read _UWORD primitive(s) */
/*===================================*/

int
datGetUW(const HDSLoc *locator, int ndim, const hdsdim dims[], unsigned short values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetUW",*status);
  if (isv5) {
    retval = datGetUW_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetUW_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetUW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================*/
/* datGetL - Read _LOGICAL primitive(s) */
/*======================================*/

int
datGetL(const HDSLoc *locator, int ndim, const hdsdim dims[], hdsbool_t values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetL",*status);
  if (isv5) {
    retval = datGetL_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetL_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetL",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================*/
/* datGetR - Read _REAL primitive(s) */
/*===================================*/

int
datGetR(const HDSLoc *locator, int ndim, const hdsdim dims[], float values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetR",*status);
  if (isv5) {
    retval = datGetR_v5(locator, ndim, dims, values, status);
  } else {
    retval = datGetR_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datGetR",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================*/
/* datGet0C - Read scalar string value  */
/*======================================*/

int
datGet0C(const HDSLoc * locator, char * value, size_t len, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0C",*status);
  if (isv5) {
    retval = datGet0C_v5(locator, value, len, status);
  } else {
    retval = datGet0C_v4(locator, value, len, status);
  }
  HDS_CHECK_STATUS("datGet0C",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================*/
/* datGet0D - Read scalar double value  */
/*======================================*/

int
datGet0D(const HDSLoc * locator, double * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0D",*status);
  if (isv5) {
    retval = datGet0D_v5(locator, value, status);
  } else {
    retval = datGet0D_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0D",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datGet0R - Read scalar float value  */
/*=====================================*/

int
datGet0R(const HDSLoc * locator, float * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0R",*status);
  if (isv5) {
    retval = datGet0R_v5(locator, value, status);
  } else {
    retval = datGet0R_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0R",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datGet0I - Read scalar integer value  */
/*=======================================*/

int
datGet0I(const HDSLoc * locator, int * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0I",*status);
  if (isv5) {
    retval = datGet0I_v5(locator, value, status);
  } else {
    retval = datGet0I_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0I",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datGet0K - Read scalar 64-bit integer value  */
/*================================================*/

int
datGet0K(const HDSLoc * locator, int64_t * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0K",*status);
  if (isv5) {
    retval = datGet0K_v5(locator, value, status);
  } else {
    retval = datGet0K_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0K",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=============================================*/
/* datGet0W - Read scalar short integer value  */
/*=============================================*/

int
datGet0W(const HDSLoc * locator, short * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0W",*status);
  if (isv5) {
    retval = datGet0W_v5(locator, value, status);
  } else {
    retval = datGet0W_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0W",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=============================================*/
/* datGet0UW - Read scalar unsigned short integer value  */
/*=============================================*/

int
datGet0UW(const HDSLoc * locator, unsigned short * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0UW",*status);
  if (isv5) {
    retval = datGet0UW_v5(locator, value, status);
  } else {
    retval = datGet0UW_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0UW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datGet0L - Read scalar logical value  */
/*=======================================*/

int
datGet0L(const HDSLoc * locator, hdsbool_t * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet0L",*status);
  if (isv5) {
    retval = datGet0L_v5(locator, value, status);
  } else {
    retval = datGet0L_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datGet0L",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* DAT_GET1C - Read 1D string array */
/*==================================*/

int
datGet1C(const HDSLoc * locator, size_t maxval, size_t bufsize, char *buffer, char *pntrs[], size_t * actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1C",*status);
  if (isv5) {
    retval = datGet1C_v5(locator, maxval, bufsize, buffer, pntrs, actval, status);
  } else {
    retval = datGet1C_v4(locator, maxval, bufsize, buffer, pntrs, actval, status);
  }
  HDS_CHECK_STATUS("datGet1C",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* DAT_GET1D - Read 1D Double array */
/*==================================*/

int
datGet1D(const HDSLoc * locator, size_t maxval, double values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1D",*status);
  if (isv5) {
    retval = datGet1D_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1D_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1D",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* DAT_GET1I - Read 1D Integer array */
/*==================================*/

int
datGet1I(const HDSLoc * locator, size_t maxval, int values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1I",*status);
  if (isv5) {
    retval = datGet1I_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1I_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1I",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*============================================*/
/* DAT_GET1K - Read 1D 64-bit Integer array */
/*============================================*/

int
datGet1K(const HDSLoc * locator, size_t maxval, int64_t values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1K",*status);
  if (isv5) {
    retval = datGet1K_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1K_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1K",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=========================================*/
/* DAT_GET1W - Read 1D Short Integer array */
/*=========================================*/

int
datGet1W(const HDSLoc * locator, size_t maxval, short values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1W",*status);
  if (isv5) {
    retval = datGet1W_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1W_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1W",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================================*/
/* DAT_GET1UW - Read 1D Unsigned Short Integer array */
/*===================================================*/

int
datGet1UW(const HDSLoc * locator, size_t maxval, unsigned short values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1UW",*status);
  if (isv5) {
    retval = datGet1UW_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1UW_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1UW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* DAT_GET1R - Read 1D REAL array */
/*==================================*/

int
datGet1R(const HDSLoc * locator, size_t maxval, float values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1R",*status);
  if (isv5) {
    retval = datGet1R_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1R_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1R",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* DAT_GET1L - Read 1D Logical array */
/*==================================*/

int
datGet1L(const HDSLoc * locator, size_t maxval, hdsbool_t values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGet1L",*status);
  if (isv5) {
    retval = datGet1L_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGet1L_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGet1L",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* DAT_GETVC - Read vectorized 1D string array */
/*==================================*/

int
datGetVC(const HDSLoc * locator, size_t maxval, size_t bufsize, char *buffer, char *pntrs[], size_t * actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetVC",*status);
  if (isv5) {
    retval = datGetVC_v5(locator, maxval, bufsize, buffer, pntrs, actval, status);
  } else {
    retval = datGetVC_v4(locator, maxval, bufsize, buffer, pntrs, actval, status);
  }
  HDS_CHECK_STATUS("datGetVC",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*==========================================*/
/* DAT_GETVD - Read vectorized Double array */
/*==========================================*/

int
datGetVD(const HDSLoc * locator, size_t maxval, double values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetVD",*status);
  if (isv5) {
    retval = datGetVD_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGetVD_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGetVD",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==========================================*/
/* DAT_GETVI - Read vectorized Integer array */
/*==========================================*/

int
datGetVI(const HDSLoc * locator, size_t maxval, int values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetVI",*status);
  if (isv5) {
    retval = datGetVI_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGetVI_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGetVI",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==========================================*/
/* DAT_GETVK - Read vectorized Int64 array */
/*==========================================*/

int
datGetVK(const HDSLoc * locator, size_t maxval, int64_t values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetVK",*status);
  if (isv5) {
    retval = datGetVK_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGetVK_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGetVK",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==========================================*/
/* DAT_GETVR - Read vectorized REAL array */
/*==========================================*/

int
datGetVR(const HDSLoc * locator, size_t maxval, float values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetVR",*status);
  if (isv5) {
    retval = datGetVR_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGetVR_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGetVR",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==========================================*/
/* DAT_GETVL - Read vectorized Logical array */
/*==========================================*/

int
datGetVL(const HDSLoc * locator, size_t maxval, hdsbool_t values[], size_t *actval, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datGetVL",*status);
  if (isv5) {
    retval = datGetVL_v5(locator, maxval, values, actval, status);
  } else {
    retval = datGetVL_v4(locator, maxval, values, actval, status);
  }
  HDS_CHECK_STATUS("datGetVL",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*======================================*/
/* datIndex - Index into component list */
/*======================================*/

int
datIndex(const HDSLoc *locator1, int index, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datIndex",*status);
  if (isv5) {
    retval = datIndex_v5(locator1, index, locator2, status);
  } else {
    retval = datIndex_v4(locator1, index, locator2, status);
  }
  HDS_CHECK_STATUS("datIndex",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================*/
/* datLen - Inquire primitive length */
/*===================================*/

int
datLen(const HDSLoc *locator, size_t *len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datLen",*status);
  if (isv5) {
    retval = datLen_v5(locator, len, status);
  } else {
    retval = datLen_v4(locator, len, status);
  }
  HDS_CHECK_STATUS("datLen",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===========================*/
/* datMap - Map primitive(s) */
/*===========================*/

int
datMap(const HDSLoc *locator, const char *type_str, const char *mode_str, int ndim, const hdsdim dims[], void **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMap",*status);
  if (isv5) {
    retval = datMap_v5(locator, type_str, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMap_v4(locator, type_str, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMap",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* datMapC - Map _CHAR primitive(s) */
/*==================================*/

int
datMapC(const HDSLoc *locator, const char *mode_str, int ndim, const hdsdim dims[], unsigned char **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapC",*status);
  if (isv5) {
    retval = datMapC_v5(locator, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMapC_v4(locator, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMapC",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*====================================*/
/* datMapD - Map _DOUBLE primitive(s) */
/*====================================*/

int
datMapD(const HDSLoc *locator, const char *mode_str, int ndim, const hdsdim dims[], double **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapD",*status);
  if (isv5) {
    retval = datMapD_v5(locator, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMapD_v4(locator, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMapD",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datMapI - Map _INTEGER primitive(s) */
/*=====================================*/

int
datMapI(const HDSLoc *locator, const char *mode_str, int ndim, const hdsdim dims[], int **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapI",*status);
  if (isv5) {
    retval = datMapI_v5(locator, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMapI_v4(locator, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMapI",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datMapK - Map _INT64 primitive(s) */
/*=====================================*/

int
datMapK(const HDSLoc *locator, const char *mode_str, int ndim, const hdsdim dims[], int **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapK",*status);
  if (isv5) {
    retval = datMapK_v5(locator, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMapK_v4(locator, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMapK",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datMapL - Map _LOGICAL primitive(s) */
/*=====================================*/

int
datMapL(const HDSLoc *locator, const char *mode_str, int ndim, const hdsdim dims[], hdsbool_t **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapL",*status);
  if (isv5) {
    retval = datMapL_v5(locator, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMapL_v4(locator, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMapL",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* datMapR - Map _REAL primitive(s) */
/*==================================*/

int
datMapR(const HDSLoc *locator, const char *mode_str, int ndim, const hdsdim dims[], float **pntr, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapR",*status);
  if (isv5) {
    retval = datMapR_v5(locator, mode_str, ndim, dims, pntr, status);
  } else {
    retval = datMapR_v4(locator, mode_str, ndim, dims, pntr, status);
  }
  HDS_CHECK_STATUS("datMapR",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*========================================*/
/* datMapN - Map primitive as N-dim array */
/*========================================*/

int
datMapN(const HDSLoc *locator, const char *type_str, const char *mode_str, int ndim, void **pntr, hdsdim dims[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapN",*status);
  if (isv5) {
    retval = datMapN_v5(locator, type_str, mode_str, ndim, pntr, dims, status);
  } else {
    retval = datMapN_v4(locator, type_str, mode_str, ndim, pntr, dims, status);
  }
  HDS_CHECK_STATUS("datMapN",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* datMapV - Map vectorized primitive(s) */
/*==================================*/

int
datMapV(const HDSLoc *locator, const char *type_str, const char *mode_str, void **pntr, size_t *actval, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMapV",*status);
  if (isv5) {
    retval = datMapV_v5(locator, type_str, mode_str, pntr, actval, status);
  } else {
    retval = datMapV_v4(locator, type_str, mode_str, pntr, actval, status);
  }
  HDS_CHECK_STATUS("datMapV",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*==================================*/
/* datMould - Alter shape of object */
/*==================================*/

int
datMould(const HDSLoc *locator, int ndim, const hdsdim dims[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datMould",*status);
  if (isv5) {
    retval = datMould_v5(locator, ndim, dims, status);
  } else {
    retval = datMould_v4(locator, ndim, dims, status);
  }
  HDS_CHECK_STATUS("datMould",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================*/
/* datMove - Move object */
/*=======================*/

int
datMove(HDSLoc **locator1, const HDSLoc *locator2, const char *name_str, int *status) {
  /* Requires special code */
  int instat = *status;
  int isv5 = 0;
  int loc1isv5 = 0;
  int loc2isv5 = 0;
  EnterCheck("datMove",*status);
  if (*status != SAI__OK) return *status;
  loc1isv5 = ISHDSv5(*locator1);
  loc2isv5 = ISHDSv5(locator2);
  if (loc1isv5 && loc2isv5) {
    /* Just call the v5 code */
    isv5 = 1;
    datMove_v5(locator1, locator2, name_str, status);
  } else if ( !loc1isv5 && !loc2isv5 ) {
    isv5 = 0;
    datMove_v4(locator1, locator2, name_str, status);
  } else {
    HDSLoc * parenloc = NULL;
    char namestr[DAT__SZNAM+1];
    /* Just do a copy */
    datCopy(*locator1, locator2, name_str, status);
    /* and then erase - HDS API insists that we can not erase
       based on a locator so we need to get the parent and this name. */
    datName(*locator1, namestr, status);
    datParen(*locator1, &parenloc, status);
    datAnnul(locator1, status);
    datErase(parenloc, namestr, status);
    datAnnul(&parenloc, status);
  }
  HDS_CHECK_STATUS("datMove",(isv5 ? "(v5)" : "(v4)"));
  return *status;
}

/*======================================*/
/* datMsg - store filename in EMS token */
/*======================================*/

void
datMsg(const char * token, const HDSLoc * locator) {
  EnterCheck("datMsg",-1);
  if (ISHDSv5(locator)) {
    datMsg_v5(token, locator);
  } else {
    datMsg_v4(token, locator);
  }
  return;
}

/*===============================*/
/* datName - Enquire object name */
/*===============================*/

int
datName(const HDSLoc *locator, char name_str[DAT__SZNAM+1], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datName",*status);
  if (isv5) {
    retval = datName_v5(locator, name_str, status);
  } else {
    retval = datName_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datName",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=========================================*/
/* datNcomp - Inquire number of components */
/*=========================================*/

int
datNcomp(const HDSLoc *locator, int *ncomp, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNcomp",*status);
  if (isv5) {
    retval = datNcomp_v5(locator, ncomp, status);
  } else {
    retval = datNcomp_v4(locator, ncomp, status);
  }
  HDS_CHECK_STATUS("datNcomp",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===============================*/
/* datNew - Create new component */
/*===============================*/

int
datNew(const HDSLoc *locator, const char *name_str, const char *type_str, int ndim, const hdsdim dims[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew",*status);
  if (isv5) {
    retval = datNew_v5(locator, name_str, type_str, ndim, dims, status);
  } else {
    retval = datNew_v4(locator, name_str, type_str, ndim, dims, status);
  }
  HDS_CHECK_STATUS("datNew",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*============================================*/
/* datNewC - Create new _CHAR type component */
/*============================================*/

int
datNewC(const HDSLoc *locator, const char *name_str, size_t len, int ndim, const hdsdim dims[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNewC",*status);
  if (isv5) {
    retval = datNewC_v5(locator, name_str, len, ndim, dims, status);
  } else {
    retval = datNewC_v4(locator, name_str, len, ndim, dims, status);
  }
  HDS_CHECK_STATUS("datNewC",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datNew0 - Create new scalar component */
/*=======================================*/

int
datNew0(const HDSLoc *locator, const char *name_str, const char *type_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0",*status);
  if (isv5) {
    retval = datNew0_v5(locator, name_str, type_str, status);
  } else {
    retval = datNew0_v4(locator, name_str, type_str, status);
  }
  HDS_CHECK_STATUS("datNew0",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===============================================*/
/* datNew0D - Create new scalar double component */
/*===============================================*/

int
datNew0D(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0D",*status);
  if (isv5) {
    retval = datNew0D_v5(locator, name_str, status);
  } else {
    retval = datNew0D_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0D",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datNew0I - Create new scalar integer component */
/*================================================*/

int
datNew0I(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0I",*status);
  if (isv5) {
    retval = datNew0I_v5(locator, name_str, status);
  } else {
    retval = datNew0I_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0I",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=========================================================*/
/* datNew0K - Create new scalar 64-bit integer component */
/*=========================================================*/

int
datNew0K(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0K",*status);
  if (isv5) {
    retval = datNew0K_v5(locator, name_str, status);
  } else {
    retval = datNew0K_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0K",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================================*/
/* datNew0W - Create new scalar short integer component */
/*======================================================*/

int
datNew0W(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0W",*status);
  if (isv5) {
    retval = datNew0W_v5(locator, name_str, status);
  } else {
    retval = datNew0W_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0W",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================================*/
/* datNew0UW - Create new scalar unsigned short integer component */
/*================================================================*/

int
datNew0UW(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0UW",*status);
  if (isv5) {
    retval = datNew0UW_v5(locator, name_str, status);
  } else {
    retval = datNew0UW_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0UW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=============================================*/
/* datNew0R - Create new scalar real component */
/*=============================================*/

int
datNew0R(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0R",*status);
  if (isv5) {
    retval = datNew0R_v5(locator, name_str, status);
  } else {
    retval = datNew0R_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0R",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datNew0L - Create new scalar logical component */
/*================================================*/

int
datNew0L(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0L",*status);
  if (isv5) {
    retval = datNew0L_v5(locator, name_str, status);
  } else {
    retval = datNew0L_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datNew0L",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datNew0L - Create new scalar logical component */
/*================================================*/

int
datNew0C(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew0C",*status);
  if (isv5) {
    retval = datNew0C_v5(locator, name_str, len, status);
  } else {
    retval = datNew0C_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew0C",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}



/*=======================================*/
/* datNew1 - Create new vector component */
/*=======================================*/

int
datNew1(const HDSLoc *locator, const char *name_str, const char *type_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1",*status);
  if (isv5) {
    retval = datNew1_v5(locator, name_str, type_str, len, status);
  } else {
    retval = datNew1_v4(locator, name_str, type_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datNew1C - Create new vector string  */
/*=======================================*/

int
datNew1C(const HDSLoc *locator, const char *name_str, size_t len, size_t nelem, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1C",*status);
  if (isv5) {
    retval = datNew1C_v5(locator, name_str, len, nelem, status);
  } else {
    retval = datNew1C_v4(locator, name_str, len, nelem, status);
  }
  HDS_CHECK_STATUS("datNew1C",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datNew1d - Create new vector double   */
/*=======================================*/

int
datNew1D(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1D",*status);
  if (isv5) {
    retval = datNew1D_v5(locator, name_str, len, status);
  } else {
    retval = datNew1D_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1D",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datNew1I - Create new vector integer  */
/*=======================================*/

int
datNew1I(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1I",*status);
  if (isv5) {
    retval = datNew1I_v5(locator, name_str, len, status);
  } else {
    retval = datNew1I_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1I",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datNew1K - Create new vector 64-bit integer  */
/*================================================*/

int
datNew1K(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1K",*status);
  if (isv5) {
    retval = datNew1K_v5(locator, name_str, len, status);
  } else {
    retval = datNew1K_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1K",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=============================================*/
/* datNew1W - Create new vector short integer  */
/*=============================================*/

int
datNew1W(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1W",*status);
  if (isv5) {
    retval = datNew1W_v5(locator, name_str, len, status);
  } else {
    retval = datNew1W_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1W",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================================*/
/* datNew1UW - Create new vector unsigned short integer  */
/*=======================================================*/

int
datNew1UW(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1UW",*status);
  if (isv5) {
    retval = datNew1UW_v5(locator, name_str, len, status);
  } else {
    retval = datNew1UW_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1UW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datNew1L - Create new vector logical   */
/*=======================================*/

int
datNew1L(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1L",*status);
  if (isv5) {
    retval = datNew1L_v5(locator, name_str, len, status);
  } else {
    retval = datNew1L_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1L",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datNew1R - Create new vector float   */
/*=======================================*/

int
datNew1R(const HDSLoc *locator, const char *name_str, size_t len, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datNew1R",*status);
  if (isv5) {
    retval = datNew1R_v5(locator, name_str, len, status);
  } else {
    retval = datNew1R_v4(locator, name_str, len, status);
  }
  HDS_CHECK_STATUS("datNew1R",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*====================================*/
/* datParen - Locate parent structure */
/*====================================*/

int
datParen(const HDSLoc *locator1, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datParen",*status);
  if (isv5) {
    retval = datParen_v5(locator1, locator2, status);
  } else {
    retval = datParen_v4(locator1, locator2, status);
  }
  HDS_CHECK_STATUS("datParen",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*=====================================*/
/* datPrec - Enquire storage precision */
/*=====================================*/

int
datPrec(const HDSLoc *locator, size_t *nbytes, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPrec",*status);
  if (isv5) {
    retval = datPrec_v5(locator, nbytes, status);
  } else {
    retval = datPrec_v4(locator, nbytes, status);
  }
  HDS_CHECK_STATUS("datPrec",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*====================================*/
/* datPrim - Enquire object primitive */
/*====================================*/

int
datPrim(const HDSLoc *locator, hdsbool_t *prim, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPrim",*status);
  if (isv5) {
    retval = datPrim_v5(locator, prim, status);
  } else {
    retval = datPrim_v4(locator, prim, status);
  }
  HDS_CHECK_STATUS("datPrim",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=========================================================*/
/* datPrmry - Set/Enquire primary/secondary locator status */
/*=========================================================*/

int
datPrmry(hdsbool_t set, HDSLoc **locator, hdsbool_t *prmry, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(*locator);
  EnterCheck("datPrmry",*status);
  if (isv5) {
    retval = datPrmry_v5(set, locator, prmry, status);
  } else {
    retval = datPrmry_v4(set, locator, prmry, status);
  }
  HDS_CHECK_STATUS("datPrmry",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* datPutC - Write _CHAR primitive */
/*==================================*/

int
datPutC(const HDSLoc *locator, int ndim, const hdsdim dims[], const char string[], size_t string_length, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutC",*status);
  if (isv5) {
    retval = datPutC_v5(locator, ndim, dims, string, string_length, status);
  } else {
    retval = datPutC_v4(locator, ndim, dims, string, string_length, status);
  }
  HDS_CHECK_STATUS("datPutC",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*====================================*/
/* datPutD - Write _DOUBLE primitives */
/*====================================*/

int
datPutD(const HDSLoc *locator, int ndim, const hdsdim dims[], const double values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutD",*status);
  if (isv5) {
    retval = datPutD_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutD_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutD",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datPutI - Write _INTEGER primitives */
/*=====================================*/

int
datPutI(const HDSLoc *locator, int ndim, const hdsdim dims[], const int values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutI",*status);
  if (isv5) {
    retval = datPutI_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutI_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutI",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datPutK - Write _INT64 primitives */
/*=====================================*/

int
datPutK(const HDSLoc *locator, int ndim, const hdsdim dims[], const int64_t values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutK",*status);
  if (isv5) {
    retval = datPutK_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutK_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutK",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datPutW - Write _WORD primitives */
/*=====================================*/

int
datPutW(const HDSLoc *locator, int ndim, const hdsdim dims[], const short values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutW",*status);
  if (isv5) {
    retval = datPutW_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutW_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*====================================*/
/* datPutUW - Write _UWORD primitives */
/*====================================*/

int
datPutUW(const HDSLoc *locator, int ndim, const hdsdim dims[], const unsigned short values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutUW",*status);
  if (isv5) {
    retval = datPutUW_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutUW_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutUW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* datPutR - Write _REAL primitives */
/*==================================*/

int
datPutR(const HDSLoc *locator, int ndim, const hdsdim dims[], const float values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutR",*status);
  if (isv5) {
    retval = datPutR_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutR_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutR",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datPutL - Write _LOGICAL primitives */
/*=====================================*/

int
datPutL(const HDSLoc *locator, int ndim, const hdsdim dims[], const hdsbool_t values[], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutL",*status);
  if (isv5) {
    retval = datPutL_v5(locator, ndim, dims, values, status);
  } else {
    retval = datPutL_v4(locator, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPutL",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==========================*/
/* datPut - Write primitive */
/*==========================*/

int
datPut(const HDSLoc *locator, const char *type_str, int ndim, const hdsdim dims[], const void *values, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut",*status);
  if (isv5) {
    retval = datPut_v5(locator, type_str, ndim, dims, values, status);
  } else {
    retval = datPut_v4(locator, type_str, ndim, dims, values, status);
  }
  HDS_CHECK_STATUS("datPut",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datPut0C - Write scalar string value  */
/*=======================================*/

int
datPut0C(const HDSLoc * locator, const char * value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0C",*status);
  if (isv5) {
    retval = datPut0C_v5(locator, value, status);
  } else {
    retval = datPut0C_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0C",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=======================================*/
/* datPut0D - Write scalar double value  */
/*=======================================*/

int
datPut0D(const HDSLoc * locator, double value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0D",*status);
  if (isv5) {
    retval = datPut0D_v5(locator, value, status);
  } else {
    retval = datPut0D_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0D",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*======================================*/
/* datPut0R - Write scalar float value  */
/*======================================*/

int
datPut0R(const HDSLoc * locator, float value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0R",*status);
  if (isv5) {
    retval = datPut0R_v5(locator, value, status);
  } else {
    retval = datPut0R_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0R",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut0I - Write scalar integer value  */
/*========================================*/

int
datPut0I(const HDSLoc * locator, int value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0I",*status);
  if (isv5) {
    retval = datPut0I_v5(locator, value, status);
  } else {
    retval = datPut0I_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0I",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut0I - Write scalar 64-bit integer value  */
/*========================================*/

int
datPut0K(const HDSLoc * locator, int64_t value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0K",*status);
  if (isv5) {
    retval = datPut0K_v5(locator, value, status);
  } else {
    retval = datPut0K_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0K",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==============================================*/
/* datPut0W - Write scalar short integer value  */
/*===============================================*/

int
datPut0W(const HDSLoc * locator, short value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0W",*status);
  if (isv5) {
    retval = datPut0W_v5(locator, value, status);
  } else {
    retval = datPut0W_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0W",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================================*/
/* datPut0UW - Write scalar unsigned short integer value  */
/*========================================================*/

int
datPut0UW(const HDSLoc * locator, unsigned short value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0UW",*status);
  if (isv5) {
    retval = datPut0UW_v5(locator, value, status);
  } else {
    retval = datPut0UW_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0UW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut0L - Write scalar logical value  */
/*========================================*/

int
datPut0L(const HDSLoc * locator, hdsbool_t value, int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut0L",*status);
  if (isv5) {
    retval = datPut0L_v5(locator, value, status);
  } else {
    retval = datPut0L_v4(locator, value, status);
  }
  HDS_CHECK_STATUS("datPut0L",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut1C - Write 1D character array       */
/*========================================*/

int
datPut1C(const HDSLoc * locator, size_t nval, const char *values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1C",*status);
  if (isv5) {
    retval = datPut1C_v5(locator, nval, values, status);
  } else {
    retval = datPut1C_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1C",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut1D - Write 1D double array       */
/*========================================*/

int
datPut1D(const HDSLoc * locator, size_t nval, const double values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1D",*status);
  if (isv5) {
    retval = datPut1D_v5(locator, nval, values, status);
  } else {
    retval = datPut1D_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1D",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut1I - Write 1D int array       */
/*========================================*/

int
datPut1I(const HDSLoc * locator, size_t nval, const int values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1I",*status);
  if (isv5) {
    retval = datPut1I_v5(locator, nval, values, status);
  } else {
    retval = datPut1I_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1I",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut1K - Write 1D 64-bit int array */
/*========================================*/

int
datPut1K(const HDSLoc * locator, size_t nval, const int64_t values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1K",*status);
  if (isv5) {
    retval = datPut1K_v5(locator, nval, values, status);
  } else {
    retval = datPut1K_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1K",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===========================================*/
/* datPut1W - Write 1D short int array       */
/*===========================================*/

int
datPut1W(const HDSLoc * locator, size_t nval, const short values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1W",*status);
  if (isv5) {
    retval = datPut1W_v5(locator, nval, values, status);
  } else {
    retval = datPut1W_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1W",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===============================================*/
/* datPut1UW - Write 1D unsigned short int array */
/*===============================================*/

int
datPut1UW(const HDSLoc * locator, size_t nval, const unsigned short values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1UW",*status);
  if (isv5) {
    retval = datPut1UW_v5(locator, nval, values, status);
  } else {
    retval = datPut1UW_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1UW",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut1R - Write 1D double array       */
/*========================================*/

int
datPut1R(const HDSLoc * locator, size_t nval, const float values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1R",*status);
  if (isv5) {
    retval = datPut1R_v5(locator, nval, values, status);
  } else {
    retval = datPut1R_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1R",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* datPut1L - Write 1D Logical/int array       */
/*========================================*/

int
datPut1L(const HDSLoc * locator, size_t nval, const hdsbool_t values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPut1L",*status);
  if (isv5) {
    retval = datPut1L_v5(locator, nval, values, status);
  } else {
    retval = datPut1L_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPut1L",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datPutVD - Write vectorized double array       */
/*================================================*/

int
datPutVD(const HDSLoc * locator, size_t nval, const double values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutVD",*status);
  if (isv5) {
    retval = datPutVD_v5(locator, nval, values, status);
  } else {
    retval = datPutVD_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPutVD",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datPutVI - Write vectorized int array       */
/*================================================*/

int
datPutVI(const HDSLoc * locator, size_t nval, const int values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutVI",*status);
  if (isv5) {
    retval = datPutVI_v5(locator, nval, values, status);
  } else {
    retval = datPutVI_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPutVI",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datPutVI - Write vectorized int64 array       */
/*================================================*/

int
datPutVK(const HDSLoc * locator, size_t nval, const int64_t values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutVK",*status);
  if (isv5) {
    retval = datPutVK_v5(locator, nval, values, status);
  } else {
    retval = datPutVK_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPutVK",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datPutVR - Write vectorized REAL/float array       */
/*================================================*/

int
datPutVR(const HDSLoc * locator, size_t nval, const float values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutVR",*status);
  if (isv5) {
    retval = datPutVR_v5(locator, nval, values, status);
  } else {
    retval = datPutVR_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPutVR",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datPutVL - Write vectorized Logical array       */
/*================================================*/

int
datPutVL(const HDSLoc * locator, size_t nval, const hdsbool_t values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutVL",*status);
  if (isv5) {
    retval = datPutVL_v5(locator, nval, values, status);
  } else {
    retval = datPutVL_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPutVL",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datPutVC - Write vectorized character array       */
/*================================================*/

int
datPutVC(const HDSLoc * locator, size_t nval, const char *values[], int * status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datPutVC",*status);
  if (isv5) {
    retval = datPutVC_v5(locator, nval, values, status);
  } else {
    retval = datPutVC_v4(locator, nval, values, status);
  }
  HDS_CHECK_STATUS("datPutVC",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*========================================*/
/* datRef - Enquire object reference name */
/*========================================*/

int
datRef(const HDSLoc * locator, char * ref, size_t reflen, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datRef",*status);
  if (isv5) {
    retval = datRef_v5(locator, ref, reflen, status);
  } else {
    retval = datRef_v4(locator, ref, reflen, status);
  }
  HDS_CHECK_STATUS("datRef",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================================*/
/* datRefct - Enquire container file reference count */
/*===================================================*/

int
datRefct(const HDSLoc *locator, int *refct, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datRefct",*status);
  if (isv5) {
    retval = datRefct_v5(locator, refct, status);
  } else {
    retval = datRefct_v4(locator, refct, status);
  }
  HDS_CHECK_STATUS("datRefct",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=============================*/
/* datRenam - Rename an object */
/*=============================*/

int
datRenam(const HDSLoc *locator, const char *name_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datRenam",*status);
  if (isv5) {
    retval = datRenam_v5(locator, name_str, status);
  } else {
    retval = datRenam_v4(locator, name_str, status);
  }
  HDS_CHECK_STATUS("datRenam",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================*/
/* datReset - Reset object state */
/*================================*/

int
datReset(const HDSLoc *locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datReset",*status);
  if (isv5) {
    retval = datReset_v5(locator, status);
  } else {
    retval = datReset_v4(locator, status);
  }
  HDS_CHECK_STATUS("datReset",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================*/
/* datRetyp - Change object type */
/*================================*/

int
datRetyp(const HDSLoc *locator, const char *type_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datRetyp",*status);
  if (isv5) {
    retval = datRetyp_v5(locator, type_str, status);
  } else {
    retval = datRetyp_v4(locator, type_str, status);
  }
  HDS_CHECK_STATUS("datRetyp",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=================================*/
/* datShape - Enquire object shape */
/*=================================*/

int
datShape(const HDSLoc *locator, int maxdim, hdsdim dims[], int *actdim, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datShape",*status);
  if (isv5) {
    retval = datShape_v5(locator, maxdim, dims, actdim, status);
  } else {
    retval = datShape_v4(locator, maxdim, dims, actdim, status);
  }
  HDS_CHECK_STATUS("datShape",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===============================*/
/* datSize - Enquire object size */
/*===============================*/

int
datSize(const HDSLoc *locator, size_t *size, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datSize",*status);
  if (isv5) {
    retval = datSize_v5(locator, size, status);
  } else {
    retval = datSize_v4(locator, size, status);
  }
  HDS_CHECK_STATUS("datSize",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================*/
/* datSlice - Locate object slice */
/*================================*/

int
datSlice(const HDSLoc *locator1, int ndim, const hdsdim lower[], const hdsdim upper[], HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datSlice",*status);
  if (isv5) {
    retval = datSlice_v5(locator1, ndim, lower, upper, locator2, status);
  } else {
    retval = datSlice_v4(locator1, ndim, lower, upper, locator2, status);
  }
  HDS_CHECK_STATUS("datSlice",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=================================*/
/* datState - Enquire object state */
/*=================================*/

int
datState(const HDSLoc *locator, hdsbool_t *state, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datState",*status);
  if (isv5) {
    retval = datState_v5(locator, state, status);
  } else {
    retval = datState_v4(locator, state, status);
  }
  HDS_CHECK_STATUS("datState",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================*/
/* datStruc - Enquire object structure */
/*=====================================*/

int
datStruc(const HDSLoc *locator, hdsbool_t *struc, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datStruc",*status);
  if (isv5) {
    retval = datStruc_v5(locator, struc, status);
  } else {
    retval = datStruc_v4(locator, struc, status);
  }
  HDS_CHECK_STATUS("datStruc",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===================================*/
/* datTemp - Create temporary object */
/*===================================*/

int
datTemp(const char *type_str, int ndim, const hdsdim dims[], HDSLoc **locator, int *status) {
  int retval = 0;
  int instat = *status;
  EnterCheck("datTemp",*status);
  if (*status != SAI__OK) return *status;
  retval = datTemp_v5(type_str, ndim, dims, locator, status);
  HDS_CHECK_STATUS("datTemp","(v5)");
  return retval;
}

/*=========================================*/
/* datThere - Enquire component existence */
/*=========================================*/

int
datThere(const HDSLoc *locator, const char *name_c, hdsbool_t *there, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datThere",*status);
  if (isv5) {
    retval = datThere_v5(locator, name_c, there, status);
  } else {
    retval = datThere_v4(locator, name_c, there, status);
  }
  HDS_CHECK_STATUS("datThere",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===============================*/
/* datType - Enquire object type */
/*===============================*/

int
datType(const HDSLoc *locator, char type_str[DAT__SZTYP + 1], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datType",*status);
  if (isv5) {
    retval = datType_v5(locator, type_str, status);
  } else {
    retval = datType_v4(locator, type_str, status);
  }
  HDS_CHECK_STATUS("datType",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=========================*/
/* datUnmap - Unmap object */
/*=========================*/

int
datUnmap(const HDSLoc *locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datUnmap",*status);
  if (isv5) {
    retval = datUnmap_v5(locator, status);
  } else {
    retval = datUnmap_v4(locator, status);
  }
  HDS_CHECK_STATUS("datUnmap",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* datValid - Enquire locator valid */
/*==================================*/

int
datValid(const HDSLoc *locator, hdsbool_t *valid, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datValid",*status);
  if (isv5) {
    retval = datValid_v5(locator, valid, status);
  } else {
    retval = datValid_v4(locator, valid, status);
  }
  HDS_CHECK_STATUS("datValid",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===========================*/
/* datVec - Vectorise object */
/*===========================*/

int
datVec(const HDSLoc *locator1, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("datVec",*status);
  if (isv5) {
    retval = datVec_v5(locator1, locator2, status);
  } else {
    retval = datVec_v4(locator1, locator2, status);
  }
  HDS_CHECK_STATUS("datVec",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================================*/
/* datWhere - Find primitive position in HDS file */
/*            Currently not part of the public    */
/*            C API                               */
/*================================================*/

/*==================================================*/
/* hdsCopy - Copy an object to a new container file */
/*==================================================*/

int
hdsCopy(const HDSLoc *locator, const char *file_str, const char name_str[DAT__SZNAM], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsCopy",*status);
  if (isv5) {
    retval = hdsCopy_v5(locator, file_str, name_str, status);
  } else {
    retval = hdsCopy_v4(locator, file_str, name_str, status);
  }
  HDS_CHECK_STATUS("hdsCopy",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=================================*/
/* hdsErase - Erase container file */
/*=================================*/

int
hdsErase(HDSLoc **locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(*locator);
  EnterCheck("hdsErase",*status);
  if (isv5) {
    retval = hdsErase_v5(locator, status);
  } else {
    retval = hdsErase_v4(locator, status);
  }
  HDS_CHECK_STATUS("hdsErase",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*===============================================================*/
/* hdsEwild - End a wild card search for HDS container files     */
/*===============================================================*/

int
hdsEwild(int *iwld, int *status) {
  /* Requires special code */
  printf("Aborting. Special code required in: %s\n", "hdsEwild(iwld, status);");
  abort();
  return *status;
}

/*================================*/
/* hdsFlush - Flush locator group */
/*=================================*/

int
hdsFlush(const char *group_str, int *status) {
  /* We are only allowed to flush a group that actually exists */
  int instat = *status;
  EnterCheck("hdsFlush",*status);
  if (*status != SAI__OK) return *status;

  /* We need a new API that will let us query whether a group
     exists before we try to flush it. _v5 triggers an error
     if the group doesn't exist but v4 does not trigger such an error.
     For now we catch the specific error from v5 and assume that means
     v4 will deal with it. */
  hdsFlush_v5(group_str, status);
  if (*status == DAT__GRPIN) emsAnnul(status);
  hdsFlush_v4(group_str, status);
  HDS_CHECK_STATUS("hdsFlush", "(both)");
  return *status;
}

/*===============================*/
/* hdsFree - Free container file */
/*===============================*/

int
hdsFree(const HDSLoc *locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsFree",*status);
  if (isv5) {
    retval = hdsFree_v5(locator, status);
  } else {
    retval = hdsFree_v4(locator, status);
  }
  HDS_CHECK_STATUS("hdsFree",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*==================================*/
/* hdsGroup - Enquire locator group */
/*==================================*/

int
hdsGroup(const HDSLoc *locator, char group_str[DAT__SZGRP+1], int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsGroup",*status);
  if (isv5) {
    retval = hdsGroup_v5(locator, group_str, status);
  } else {
    retval = hdsGroup_v4(locator, group_str, status);
  }
  HDS_CHECK_STATUS("hdsGroup",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=========================================*/
/* hdsGtune - Get HDS tuning parameter     */
/*=========================================*/

int
hdsGtune(const char *param_str, int *value, int *status) {
  int instat = *status;
  EnterCheck("hdsGtune",*status);
  if (*status != SAI__OK) return *status;
  hdsGtune_v5(param_str, value, status);
  hdsGtune_v4(param_str, value, status);
  if (*status != SAI__OK) {
    emsRepf("hdsGtune_wrap", "hdsGtune: Error obtaining value of tuning parameter '%s'",
            status, param_str);
  }
  HDS_CHECK_STATUS("hdsGtune", "(both)");
  return *status;
}

/*=========================================*/
/* hdsGtune - Get HDS status integers      */
/*=========================================*/

int
hdsInfoI(const HDSLoc* locator, const char *topic_str, const char *extra, int *result, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsInfoI",*status);
  if (isv5) {
    retval = hdsInfoI_v5(locator, topic_str, extra, result, status);
  } else {
    retval = hdsInfoI_v4(locator, topic_str, extra, result, status);
  }
  HDS_CHECK_STATUS("hdsInfoI",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=================================*/
/* hdsLink - Link locator to group */
/*=================================*/

int
hdsLink(const HDSLoc *locator, const char *group_str, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsLink",*status);
  if (isv5) {
    retval = hdsLink_v5(locator, group_str, status);
  } else {
    retval = hdsLink_v4(locator, group_str, status);
  }
  HDS_CHECK_STATUS("hdsLink",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*================================*/
/* hdsLock - Lock container file */
/*================================*/

int
hdsLock(const HDSLoc *locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsLock",*status);
  if (isv5) {
    retval = hdsLock_v5(locator, status);
  } else {
    retval = hdsLock_v4(locator, status);
  }
  HDS_CHECK_STATUS("hdsLock",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*====================================*/
/* hdsNew - Create new container file */
/*====================================*/

int
hdsNew(const char *file_str, const char *name_str, const char *type_str, int ndim, const hdsdim dims[], HDSLoc **locator, int *status) {
  int retval = 0;
  int instat = *status;
  EnterCheck("hdsNew",*status);
  if (*status != SAI__OK) return *status;
  retval = hdsNew_v5(file_str, name_str, type_str, ndim, dims, locator, status);
  HDS_CHECK_STATUS("hdsNew","(v5)");
  return retval;
}

/*========================================*/
/* hdsOpen - Open existing container file */
/*========================================*/

int
hdsOpen(const char *file_str, const char *mode_str, HDSLoc **locator, int *status) {
  int instat = *status;
  EnterCheck("hdsOpen",*status);
  if (*status != SAI__OK) return *status;
  hdsOpen_v5(file_str, mode_str, locator, status);
  if (*status != SAI__OK) {
    emsAnnul(status);
    hdsOpen_v4(file_str, mode_str, locator, status);
  }
  HDS_CHECK_STATUS( "hdsOpen", file_str);
  return *status;
}

/*===============================*/
/* hdsShow - Show HDS statistics */
/*===============================*/

int
hdsShow(const char *topic_str, int *status) {
  /* Requires special code */
  printf("Aborting. Special code required in: %s\n", "hdsShow(topic_str, status);");
  abort();
  return *status;
}

/*===============================================*/
/* hdsState - Enquire the current state of HDS   */
/*===============================================*/

int
hdsState(hdsbool_t *state, int *status) {
  int retval = 0;
  int instat = *status;
  EnterCheck("hdsState",*status);
  if (*status != SAI__OK) return *status;
  retval = hdsState_v5(state, status);
  retval = hdsState_v4(state, status);
  HDS_CHECK_STATUS("hdsState", "(both)");
  return retval;
}

/*============================*/
/* hdsStop - Close down HDS   */
/*============================*/

int
hdsStop(int *status) {
  int retval = 0;
  int instat = *status;
  EnterCheck("hdsStop",*status);
  if (*status != SAI__OK) return *status;
  retval = hdsStop_v5(status);
  retval = hdsStop_v4(status);
  HDS_CHECK_STATUS("hdsStop", "(both)");
  return retval;
}

/*==============================*/
/* hdsTrace - Trace object path */
/*==============================*/

int
hdsTrace(const HDSLoc *locator, int *nlev, char *path_str, char *file_str, int *status, size_t path_length, size_t file_length) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("hdsTrace",*status);
  if (isv5) {
    retval = hdsTrace_v5(locator, nlev, path_str, file_str, status, path_length, file_length);
  } else {
    retval = hdsTrace_v4(locator, nlev, path_str, file_str, status, path_length, file_length);
  }
  HDS_CHECK_STATUS("hdsTrace",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*========================================*/
/* hdsTune - Set HDS tuning parameter     */
/*========================================*/

int
hdsTune(const char *param_str, int value, int *status) {
  int retval = 0;
  int instat = *status;
  EnterCheck("hdsTune",*status);
  if (*status != SAI__OK) return *status;
  retval = hdsTune_v5(param_str, value, status);
  retval = hdsTune_v4(param_str, value, status);
  HDS_CHECK_STATUS("hdsTune", "(both)");
  return retval;
}

/*=================================================================*/
/* hdsWild - Perform a wild-card search for HDS container files   */
/*=================================================================*/

int
hdsWild(const char *fspec, const char *mode, int *iwld, HDSLoc **locator, int *status) {
  /* Requires special code */
  printf("Aborting. Special code required in: %s\n", "hdsWild(fspec, mode, iwld, locator, status);");
  abort();
  return *status;
}

/*=================================================================*/
/*  Deprecated routines!                                           */
/*=================================================================*/

/*========================================*/
/* datConv - Enquire conversion possible? */
/*========================================*/

int
datConv(const HDSLoc *locator, const char *type_str, hdsbool_t *conv, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator);
  EnterCheck("datConv",*status);
  if (isv5) {
    retval = datConv_v5(locator, type_str, conv, status);
  } else {
    retval = datConv_v4(locator, type_str, conv, status);
  }
  HDS_CHECK_STATUS("datConv",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}

/*=====================================================*/
/* hdsClose - Close container file (Obsolete routine!) */
/*=====================================================*/

int
hdsClose(HDSLoc **locator, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(*locator);
  EnterCheck("hdsClose",*status);
  if (isv5) {
    retval = hdsClose_v5(locator, status);
  } else {
    retval = hdsClose_v4(locator, status);
  }
  HDS_CHECK_STATUS("hdsClose",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}


/*===================================================================*/
/* hdsFind - Find an object (Fortran routine, requires hdsf library) */
/*===================================================================*/

int
hdsFind(const HDSLoc *locator1, const char *name, const char *mode, HDSLoc **locator2, int *status) {

  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5(locator1);
  EnterCheck("hdsFind",*status);
  if (isv5) {
    retval = hdsFind_v5(locator1, name, mode, locator2, status);
  } else {
    retval = hdsFind_v4(locator1, name, mode, locator2, status);
  }
  HDS_CHECK_STATUS("hdsFind",(isv5 ? "(v5)" : "(v4)"));
  return retval;
}



*+  HRESET - changes status of an HDS data object to undefined
      SUBROUTINE HRESET(STATUS)

*    Description :

*    Parameters :

*     INP  = CHAR  - the name of the object
*    Method :
*    Deficiencies :
*    Bugs :
*    Authors :
*             (BHVAD::RJV)
*    History :
*    Type Definitions :
      IMPLICIT NONE
*    Global constants :
      INCLUDE 'SAE_PAR'
      INCLUDE 'DAT_PAR'
*    Import :
*    Import-export :
*    Export :
*    Status :
      INTEGER STATUS
*    Local Constants :
      CHARACTER*30 VERSION
      PARAMETER (VERSION='HRESET Version 1.0-0')
*    Local variables :
      CHARACTER*(DAT__SZLOC) LOC
*-
      CALL MSG_PRNT(VERSION)

      CALL DAT_ASSOC('INP','UPDATE',LOC,STATUS)

      CALL DAT_RESET(LOC,STATUS)

      CALL DAT_ANNUL(LOC,STATUS)

      CALL AST_ERR(STATUS)

      END


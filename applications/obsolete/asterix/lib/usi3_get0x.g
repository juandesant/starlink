      SUBROUTINE USI3_GET0<T>( PAR, VALUE, STATUS )
*+
*  Name:
*     USI3_GET0<T>

*  Purpose:
*     Read a <COMM> parameter value using the FTOOLS parameter system

*  Language:
*     Starlink Fortran

*  Invocation:
*     Read a <TYPE> parameter value using the FTOOLS parameter system.
*     We read the value into a string, check for special values ! and !!,
*     and then convert the data.

*  Description:
*     {routine_description}

*  Arguments:
*     PAR = CHARACTER*(*) (given)
*        The name of the parameter
*     VALUE = <TYPE> (returned)
*        The value of the parameter
*     STATUS = INTEGER (given and returned)
*        The global status.

*  Examples:
*     {routine_example_text}
*        {routine_example_description}

*  Pitfalls:
*     {pitfall_description}...

*  Notes:
*     {routine_notes}...

*  Prior Requirements:
*     {routine_prior_requirements}...

*  Side Effects:
*     {routine_side_effects}...

*  Algorithm:
*     {algorithm_description}...

*  Accuracy:
*     {routine_accuracy}

*  Timing:
*     {routine_timing}

*  External Routines Used:
*     {name_of_facility_or_package}:
*        {routine_used}...

*  Implementation Deficiencies:
*     {routine_deficiencies}...

*  References:
*     USI Subroutine Guide : http://www.sr.bham.ac.uk:8080/asterix-docs/Programmer/Guides/usi.html

*  Keywords:
*     package:usi, usage:private

*  Copyright:
*     Copyright (C) University of Birmingham, 1995

*  Authors:
*     DJA: David J. Allan (Jet-X, University of Birmingham)
*     {enter_new_authors_here}

*  History:
*     20 Mar 1995 (DJA):
*        Original version.
*     {enter_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'PAR_ERR'

*  Arguments Given:
      CHARACTER*(*)		PAR			! Parameter name

*  Arguments Returned:
      <TYPE>			VALUE			! Parameter value

*  Status:
      INTEGER 			STATUS             	! Global status

*  External References:
      EXTERNAL			CHR_LEN
        INTEGER			CHR_LEN

*  Local Constants:
      INTEGER			MAXTRY
        PARAMETER		( MAXTRY = 5 )

*  Local Variables:
      CHARACTER*300		CVAL			! Transfer value

      INTEGER			CLEN			! Length of CVAL used
      INTEGER			NTRY			! Number of tries
*.

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Read text string
      CLEN = 0
      NTRY = 0
      DO WHILE ( (CLEN.EQ.0) .AND. (NTRY.LT.MAXTRY)
     :           .AND. (STATUS.EQ.SAI__OK) )

*    Get value from environment
        CALL UCLGST( PAR, CVAL, CLEN )

*    Increment try counter
        NTRY = NTRY + 1

*    Trap special values
        IF ( CLEN .EQ. 0 ) THEN
        ELSE IF ( CVAL(:CLEN) .EQ. '!' ) THEN
          STATUS = PAR__NULL

        ELSE IF ( CVAL(:CLEN) .EQ. '!!' ) THEN
          STATUS = PAR__ABORT

        ELSE
          CALL CHR_CTO<T>( CVAL, VALUE, STATUS )

        END IF

*    Duff status? Try again unless too many times already
        IF ( (STATUS.NE.SAI__OK) .AND. (STATUS.NE.PAR__NULL) .AND.
     :       (STATUS.NE.PAR__ABORT) .AND. (NTRY .LT. MAXTRY) ) THEN
          CALL ERR_ANNUL( STATUS )
        END IF

      END DO

*  Report any errors
      IF ( STATUS .NE. SAI__OK ) CALL AST_REXIT( 'USI3_GET0<T>', STATUS )

      END

      SUBROUTINE WCSTRAN( STATUS )
*+
*  Name:
*     WCSTRAN

*  Purpose:
*     Transform a position from one NDF co-ordinate Frame to another.

*  Language:
*     Starlink Fortran 77

*  Type of Module:
*     ADAM A-task

*  Invocation:
*     CALL WCSTRAN( STATUS )

*  Arguments:
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Description:
*     This application transforms a position from one NDF co-ordinate Frame 
*     to another. The input and output Frames may be chosen freely from the
*     Frames available in the WCS component of the supplied NDF. The
*     transformed position is formatted for display and written to the screen 
*     and also to an output parameter.

*  Usage:
*     wcstran ndf posin framein [frameout]

*  ADAM Parameters:
*     EPOCHIN = _DOUBLE (Read)
*        If a "Sky Co-ordinate System" specification is supplied (using 
*        parameter FRAMEIN) for a celestial co-ordinate system, then an epoch 
*        value is needed to qualify it. This is the epoch at which the 
*        supplied sky position was determined. It should be given as a 
*        decimal years value, with or without decimal places  ("1996.8" for 
*        example). Such values are interpreted as a Besselian epoch if less 
*        than 1984.0 and as a Julian epoch otherwise. 
*     EPOCHOUT = _DOUBLE (Read)
*        If a "Sky Co-ordinate System" specification is supplied (using 
*        parameter FRAMEOUT) for a celestial co-ordinate system, then an epoch 
*        value is needed to qualify it. This is the epoch at which the 
*        transformed sky position is required. It should be given as a 
*        decimal years value, with or without decimal places  ("1996.8" for 
*        example). Such values are interpreted as a Besselian epoch if less 
*        than 1984.0 and as a Julian epoch otherwise. 
*     FRAMEIN = LITERAL (Read)
*        A string specifying the co-ordinate Frame in which the input
*        position is supplied (see parameter POSIN). If a null parameter 
*        value is supplied, then the current Frame in the NDF is used. The 
*        string can be one of the following:
*
*        - A domain name such as SKY, AXIS, PIXEL, etc. The two
*        "pseudo-domains" WORLD and DATA may be supplied and will be
*        translated into PIXEL and AXIS respectively, so long as the WCS
*        component of the NDF does not contain Frames with these domains.
*
*        - An integer value giving the index of the required Frame within
*        the WCS component.
*
*        - A "Sky Co-ordinate System" (SCS) value such as EQUAT(J2000) (see 
*        section "Sky Co-ordinate Systems" in SUN/95).
*
*     FRAMEOUT = LITERAL (Read)
*        A string specifying the co-ordinate Frame in which the transformed
*        position is required. If a null parameter value is supplied, then 
*        the current Frame in the NDF is used. The string can be one of the 
*        following:
*
*        - A domain name such as SKY, AXIS, PIXEL, etc. The two
*        "pseudo-domains" WORLD and DATA may be supplied and will be
*        translated into PIXEL and AXIS respectively, so long as the WCS
*        component of the NDF does not contain Frames with these domains.
*
*        - An integer value giving the index of the required Frame within
*        the WCS component.
*
*        - A "Sky Co-ordinate System" (SCS) value such as EQUAT(J2000) (see 
*        section "Sky Co-ordinate Systems" in SUN/95). [!]
*
*     NDF = NDF (Read and Write)
*        The NDF data structure containing the required co-ordinate Frames.
*     POSIN = LITERAL (Read)
*        The co-ordinates of the position to be transformed, in the 
*        co-ordinate Frame specified by parameter FRAMEIN (supplying 
*        a colon ":" will display details of the required co-ordinate Frame). 
*        The position should be supplied as a list of formatted axis values 
*        separated by spaces or commas. 
*     POSOUT = LITERAL (Write)
*        The formatted co-ordinates of the transformed position, in the 
*        co-ordinate Frame specified by parameter FRAMEOUT. The position
*        will be stored as a list of formatted axis values separated by 
*        spaces or commas. 
*     QUIET = _LOGICAL (Read)
*        If TRUE, the transformed position is not written to the screen
*        (it is still written to the output parameter OUTPOS). [FALSE]

*  Examples:
*     wcstran m51 "100.1 21.5" pixel
*        This transforms the pixel position "100.1 21.5" into the current
*        co-ordinate Frame of the NDF m51. The results are displayed on
*        the screen and written to the output parameter POSOUT.
*     wcstran m51 "1:00:00 -12:30" equ(B1950) pixel quiet
*        This transforms the RA/DEC position "1:00:00 -12:30" (referred
*        to the J2000 equinox) into pixel co-ordinates within the NDF m51. 
*        The results are written to the output parameter POSOUT, but are
*        not displayed on the screen.
*     wcstran m51 "1:00:00 -12:30" equ(B1950) equ(j2000) quiet
*        This is like the previous example except that the position is
*        transformed into RA/DEC referred to the B1950 equinox, instead of
*        pixel co-ordinates.

*  Related Applications:
*     KAPPA: LISTMAKE, LISTSHOW, WCSFRAME, NDFTRACE, WCSATTRIB

*  Authors:
*     DSB: David Berry (STARLINK)
*     {enter_new_authors_here}

*  History:
*     4-NOV-1998 (DSB):
*        Original version.
*     25-AUG-1999 (DSB):
*        Add TOKEN arg in call to KPG1_ASFRM
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-
*  Type Definitions:
      IMPLICIT NONE              ! no default typing allowed

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'NDF_PAR'          ! NDF constants 
      INCLUDE 'AST_PAR'          ! AST constants and function declarations

*  Status:
      INTEGER STATUS

*  Local Variables:
      CHARACTER TEXT*128         ! Formatted text
      DOUBLE PRECISION GRID( NDF__MXDIM )  ! GRID Frame position
      DOUBLE PRECISION POSIN( NDF__MXDIM ) ! Input position
      DOUBLE PRECISION POSOUT( NDF__MXDIM )! Output position
      INTEGER FRMIN              ! Pointer to requested input Frame
      INTEGER FRMOUT             ! Pointer to requested output Frame
      INTEGER IAT                ! No. of characters in the TEXT variable
      INTEGER ICURR              ! Index of original NDF current Frame
      INTEGER INDF               ! NDF identifier
      INTEGER IWCS               ! AST pointer for WCS FrameSet
      INTEGER NGRID              ! No. of GRID axes
      INTEGER NIN                ! No. of input axes
      INTEGER NOUT               ! No. of output axes
      LOGICAL QUIET              ! Suppress screen output?
*.

*  Check the inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Begin an AST context.
      CALL AST_BEGIN( STATUS )

*  Obtain an identifier for the NDF.
      CALL NDF_ASSOC( 'NDF', 'READ', INDF, STATUS )

*  Create an AST FrameSet from the WCS component of the NDF.
      CALL KPG1_GTWCS( INDF, IWCS, STATUS )

*  Get the number of Base Frame (GRID) axes.
      NGRID = AST_GETI( IWCS, 'NIN', STATUS )

*  Save the index of the current Frame.
      ICURR = AST_GETI( IWCS, 'CURRENT', STATUS )

*  Set the current Frame to be the required input Frame specified by 
*  parameter FRAMEIN. If "WORLD" co-ordinates are requested, use PIXEL. 
*  If "DATA" co-ordinates are requested, use "AXIS".
      CALL NDF_MSG( 'NDF', INDF )
      CALL KPG1_ASFRM( 'FRAMEIN', 'EPOCHIN', IWCS, 'PIXEL', 'AXIS',
     :                 .TRUE., '^NDF', STATUS )

*  Get a pointer to the input Frame.
      FRMIN = AST_GETFRAME( IWCS, AST__CURRENT, STATUS )

*  Get the number of input axes.
      NIN = AST_GETI( FRMIN, 'NAXES', STATUS )

*  Get the input position. Do not supply a dynamic default.
      POSIN( 1 ) = AST__BAD
      CALL KPG1_GTPOS( 'POSIN', FRMIN, POSIN, 0.0D0, STATUS )

*  Transform the position into GRID co-ordinates.
      CALL AST_TRANN( IWCS, 1, NIN, 1, POSIN, .FALSE., NGRID, 1, GRID, 
     :                STATUS )

*  Re-instate the original current Frame.
      CALL AST_SETI( IWCS, 'CURRENT', ICURR, STATUS )

*  Set the current Frame to be the required output Frame specified by 
*  parameter FRAMEOUT. If "WORLD" co-ordinates are requested, use PIXEL. 
*  If "DATA" co-ordinates are requested, use "AXIS".
      CALL NDF_MSG( 'NDF', INDF )
      CALL KPG1_ASFRM( 'FRAMEOUT', 'EPOCHOUT', IWCS, 'PIXEL', 'AXIS',
     :                 .TRUE., '^NDF', STATUS )

*  Get a pointer to the output Frame.
      FRMOUT = AST_GETFRAME( IWCS, AST__CURRENT, STATUS )

*  Get the number of output axes.
      NOUT = AST_GETI( FRMOUT, 'NAXES', STATUS )

*  Transform the GRID position into the output Frame.
      CALL AST_TRANN( IWCS, 1, NGRID, 1, GRID, .TRUE., NOUT, 1, POSOUT, 
     :                STATUS )

*  See if the results are to be displayed on the screen.
      CALL PAR_GET0l( 'QUIET', QUIET, STATUS )
      IF( .NOT. QUIET ) THEN

*  If so, format the input position including axis symbols.
         TEXT = ' '
         IAT =  0
         
         CALL KPG1_ASPTP( FRMIN, NIN, POSIN, .TRUE., '  ', TEXT, IAT, 
     :                    STATUS )

*  Add "  -->  " to the displayed text.
         CALL CHR_APPND( '  -->', TEXT, IAT )
         IAT = IAT + 2

*  Format the transformed position, including axis symbols.
         CALL KPG1_ASPTP( FRMOUT, NOUT, POSOUT, .TRUE., '  ', TEXT, IAT, 
     :                    STATUS )

*  Display the text, between blank lines.
         CALL MSG_BLANK( STATUS )
         CALL MSG_SETC( 'TEXT', TEXT )
         CALL MSG_OUT( 'WCSTRAN_MSG', '  ^TEXT', STATUS )
         CALL MSG_BLANK( STATUS )

      END IF

*  Format the transformed position again, this time without axis symbols.
      TEXT = ' '
      IAT = 0
      CALL KPG1_ASPTP( FRMOUT, NOUT, POSOUT, .FALSE., '  ', TEXT, IAT, 
     :                    STATUS )

*  Write this text to the output parameter.
      CALL PAR_PUT0C( 'POSOUT', TEXT( : IAT ), STATUS )

*  Annul the NDF identifier.
      CALL NDF_ANNUL( INDF, STATUS )

*  End the AST context.
      CALL AST_END( STATUS )

*  If an error occurred, then report a contextual message.
      IF ( STATUS .NE. SAI__OK ) THEN
         CALL ERR_REP( 'WCSTRAN_ERR', 'WCSTRAN: Failed to transform '//
     :                 'a position between two NDF co-ordinate Frames.',
     :                 STATUS )
      END IF

      END

      INTEGER FUNCTION COF_WCSEX( FC, INDF, ENCOD0, NATIVE, EXCLUD,
     :                            STATUS )
*+
*  Name:
*     COF_WCSEX

*  Purpose:
*     Exports WCS information from the supplied NDF into the supplied
*     FitsChan.

*  Language:
*     Starlink Fortran 77

*  Invocation:
*     RESULT = COF_WCSEX( FC, INDF, ENCOD0, NATIVE, STATUS )

*  Description:
*     The AST FrameSet (see SUN/210) describing the coordinate systems
*     of the supplied NDF is obtained. Any Frames which can be generated
*     automatically are removed from this FrameSet (i.e. the PIXEL Frame,
*     and also the AXIS Frame if it is equivalent to the PIXEL Frame). If
*     more than one Frame (i.e. the GRID Frame) remains, FITS header cards
*     are stored in the supplied FitsChan containing desriptions (known as
*     "encodings") of the FrameSet. Keywords produced by these encodings may
*     over-write corresponding keywords stored in the supplied FitsChan.
*
*     Unless an explicit encoding has been supplied using ENCOD, an attempt
*     is made to create the FITS header using one of the available AST
*     encodings. The first encoding to be tried is the default encoding
*     implied by the contents of the NDFs FITS extension. If a non-blank
*     value for ENCOD is supplied, then the specified encoding is used. If
*     argument NATIVE is .TRUE., a NATIVE encoding is also added (unless
*     ENCOD produces a native encoding).

*  Arguments:
*     FC = INTEGER (Given)
*        An AST pointer to the FitsChan in which to store the NDF's WCS
*        information.
*     INDF = INTEGER (Given)
*        The NDF identifier.
*     ENCOD0 = CHARACTER * ( * ) (Given)
*        The encoding to use. If this is blank, then a default encoding
*        is chosen based on the contents of the FITS extension. The
*        supplied string should be a recognised AST encoding such as 'DSS',
*        'FITS-WCS', 'NATIVE', etc (or a blank string).
*     NATIVE = LOGICAL (Given)
*        Should a NATIVE encoding be included in the header? Ignored if
*        ENCOD is "NATIVE".
*     EXCLUD = CHARACTER * ( * ) (Given)
*        A comma-separated list of Frames to exclude from the FitsChan.
*        A blank value indicates that no extra Frames are to be removed.
*        GRID, PIXEL, and FRACTION Frames will be ignored.
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Function Value:
*     COF_WCSEX = INTEGER
*        The number of encodings succesfully written to the FITS header.

*  Copyright:
*     Copyright (C) 1997-2001, 2003-2004 Central Laboratory of the
*     Research Councils. Copyright (C) 2007, 2011 Science & Technology
*     Facilities Council. All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either Version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful, but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
*     02111-1307, USA.

*  Authors:
*     DSB: David S. Berry (STARLINK)
*     MJC: Malcolm J. Currie (STARLINK)
*     {enter_new_authors_here}

*  History:
*     11-NOV-1997 (DSB):
*        Original version.
*     9-NOV-1998 (DSB):
*        Modified to include FITS-IRAF encoding. Changed the argument
*        list to exclude ENCODS and NENCOD, and to include NATIVE.
*     22-JUN-1999 (DSB):
*        Added ENCOD argument.
*     11-APR-2000 (DSB):
*        Added FITS-AIPS and FITS-PC encodings, and gave preference to the
*        encoding implied by the FITS extension when chosing a default
*        encoding.
*     8-AUG-2001 (DSB):
*        Modified to copy Label attribute values to Symbol attributes
*        within AXIS Frames (since the Labels are usually more useful as
*        CTYPE values than the default Symbols created by the NDF library).
*     31-JAN-2003 (DSB):
*        Modified the way in which existing WCS cards are removed from
*        the FitsChan before new ones are added. Now uses the new FitsChan
*        "Clean" attribute.
*     9-JUN-2003 (DSB):
*        - Annul any error reported by AST_WRITE sop that any remaining
*        encodings can be checked.
*        - Ignore any trailing degenerate pixel axes if the number of
*        pixel axes exceeds the number of WCS axes.
*     27-AUG-2004 (DSB):
*        - Added FITS-CLASS encoding.
*     14-MAR-2007 (DSB):
*        Added support for modifiers at the end of the encoding name
*        (currently just the "(CD)" modifier).
*     30-JUL-2009 (DSB):
*        Do not annull errors produced by AST_WRITE (inappropriate
*        FrameSets should result in a a function value of zero being
*        returned by AST_WRITE but no error being reported - any errors
*        indicate serious failure within AST).
*     4-AUG-2009 (DSB):
*        Remove the FRACTION Frame (generated by the NDF library) from the
*        FrameSet.
*     6-DEC-2010 (DSB):
*        Modified to prefer FITS-WCS encoding over FITS-IRAF (FITS-IRAF
*        does not support spectral axes).
*     2011 March 2 (MJC):
*        Added EXCLUD argument.
*     {enter_further_changes_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants
      INCLUDE 'NDF_PAR'          ! NDF public constants
      INCLUDE 'DAT_PAR'          ! DAT public constants
      INCLUDE 'AST_PAR'          ! AST public constants
      INCLUDE 'MSG_PAR'          ! MSG public constants

*  Arguments Given:
      INTEGER FC
      INTEGER INDF
      CHARACTER ENCOD0*(*)
      LOGICAL NATIVE
      CHARACTER*( * ) EXCLUD

*  Status:
      INTEGER STATUS             ! Global status

*  External References:
      INTEGER CHR_LEN            ! Used length of a string
      LOGICAL CHR_SIMLR          ! Case blind string comparison

*  Local Constants:
      INTEGER DEFNCD             ! Number of AST non-native encodings.
      PARAMETER ( DEFNCD = 7 )

*  Local Variables:
      CHARACTER ASTCOD( DEFNCD )*13 ! The AST non-native encoding names
      CHARACTER ATTR*10          ! Attribute name
      CHARACTER AXLAB*30         ! Axis label
      CHARACTER CARD*80          ! The found FITS header card
      CHARACTER COD*80           ! The encoding to use
      CHARACTER DEFENC*9         ! The default encoding to use
      CHARACTER ENCOD*13         ! The basic AST encoding to use
      CHARACTER EXCNAM*80        ! Name of a Frame to exclude
      CHARACTER FCATTR*100       ! String holding FitsChan attributes
      CHARACTER FTLOC * ( DAT__SZLOC ) ! Locator to whole FITS extension
      CHARACTER FTLOCI * ( DAT__SZLOC ) ! Locator to element of FITS extension
      INTEGER AXFRM              ! Pointer to AXIS Frame
      INTEGER END                ! Index of last char. in EXCLUD element
      INTEGER EXCFRM             ! Index of Frame to exclude
      INTEGER FCT                ! Temporary FitsChan
      INTEGER I, J               ! Loop indices
      INTEGER IAT                ! Used length of string
      INTEGER IAXIS              ! Index of AXIS Frame within the NDF FrameSet
      INTEGER ICURR0             ! Index of original Current Frame
      INTEGER IENCOD             ! Index of the current encoding
      INTEGER IFRAC              ! Index of FRACTION Frame within the NDF FrameSet
      INTEGER INDF2              ! Section of supplied NDF
      INTEGER IPIXEL             ! Index of PIXEL Frame within the NDF FrameSet
      INTEGER IWCS               ! AST identifier for NDF's WCS information
      INTEGER LBND( NDF__MXDIM ) ! NDF lower pixel bounds
      INTEGER LLEN               ! Used length of Label string
      INTEGER MAP                ! AST identifier for PIXEL to AXIS Mapping
      INTEGER NCARD              ! No. of cards
      INTEGER NCARDP             ! Previous no. of cards in FitsChan
      INTEGER NDIM               ! Number of NDF pixel axes
      INTEGER NEXCH              ! Number of characters in EXCLUD
      INTEGER NWCS               ! Number of WCS axes
      INTEGER NOBJ               ! No. of objects written to the FitsChan
      INTEGER OBJ                ! Object read from FitsChan
      INTEGER P1                 ! Index of first opening parenthesis in ENCOD0
      INTEGER START              ! Index of first char in EXCLUD element
      INTEGER UBND( NDF__MXDIM ) ! NDF upper pixel bounds
      LOGICAL FITSPR             ! True if FITS extension is present
      LOGICAL MORE               ! Read another object/excluded Frame?
      LOGICAL OK                 ! Are all excess pixel axes degenerate?
*.

*  Initialise.
      COF_WCSEX = 0

*  Check inherited global status.
      IF ( STATUS .NE. SAI__OK ) RETURN

*  Extract any modifiers from the end of the supplied encoding, and
*  create a string holding any corresponding AST attribute settings to
*  use with the FitsChan.
      FCATTR = ' '
      P1 = INDEX( ENCOD0, '(' )
      IF( P1 .GT. 0 ) THEN
         ENCOD = ENCOD0( : P1 - 1 )
         IF( CHR_SIMLR( ENCOD0( P1 : ), '(CD)' ) ) THEN
            FCATTR = 'CDMatrix=1'
         ELSE
            STATUS = SAI__ERROR
            CALL MSG_SETC( 'M', ENCOD0( P1: ) )
            CALL MSG_SETC( 'E', ENCOD0 )
            CALL ERR_REP( 'COF_WCSEX_ERR1','Unknown modifier "^M" at '//
     :                    'end of WCS encoding string "^E".', STATUS )
            GO TO 999
         END IF
      ELSE
         ENCOD = ENCOD0
      END IF

*  Assign the attribute values to the supplied FitsCHan.
      IF( FCATTR .NE. ' ' ) CALL AST_SET( FC, FCATTR, STATUS )

*  Store names of AST non-native encodings to use. They are in the order
*  in which they should be used if no explicit encoding has been requested.
      ASTCOD( 1 ) = 'FITS-WCS'
      ASTCOD( 2 ) = 'FITS-IRAF'
      ASTCOD( 3 ) = 'DSS'
      ASTCOD( 4 ) = 'FITS-AIPS'
      ASTCOD( 5 ) = 'FITS-AIPS++'
      ASTCOD( 6 ) = 'FITS-PC'
      ASTCOD( 7 ) = 'FITS-CLASS'

*  If we need to choose a default encoding, we need to decide on the
*  order in which to use the available encodings. The first to be used
*  will be the encoding implied by the WCS information in the NDFs FITS
*  extension (if any). We now find what this encoding is.
      IF( ENCOD .EQ. ' ' ) THEN

*  Check for the presence of an NDF FITS extension.
         CALL NDF_XSTAT( INDF, 'FITS', FITSPR, STATUS )
         IF ( FITSPR ) THEN

*  Get a locator to the NDF FITS extension, and determine the number of
*  cards in the extension.
            CALL NDF_XLOC( INDF, 'FITS', 'READ', FTLOC, STATUS )
            CALL DAT_SIZE( FTLOC, NCARD, STATUS )

*  Create an AST FitsChan to hold the headers in the FITS extension.
            FCT = AST_FITSCHAN( AST_NULL, AST_NULL, FCATTR, STATUS )

*  We want to guard against bad headers in the FITS extension causing the
*  whole conversion process to abort. Therefore we will annul any errors
*  which occur while reading the FITS extension into the FitsChan. First
*  we need to check that an error has not already occurred.
            IF( STATUS .EQ. SAI__OK ) THEN

*  Loop for each header in the NDF FITS extension.
               DO I = 1, NCARD

*  Get a locator to successive elements in the FITS extension.
                  CALL DAT_CELL( FTLOC, 1, I, FTLOCI, STATUS )

*  Read the FITS string, and extract the keyword and value.
                  CALL DAT_GET0C( FTLOCI, CARD, STATUS )

*  Store the card in the FitsChan.
                  CALL AST_PUTFITS( FCT, CARD, .FALSE., STATUS )

*  Annul any error so that bad headers do not cause the whole conversion
*  process to abort.
                  IF( STATUS .NE. SAI__OK ) CALL ERR_ANNUL( STATUS )

               END DO

*  Determine the default WCS encoding on the basis of the contents of the
*  FITS header.
               DEFENC = AST_GETC( FCT, 'ENCODING', STATUS )

*  Annul the FitsChan used to determine the default WCS encoding.
               CALL AST_ANNUL( FCT, STATUS )

*  Annul any error, and ensure the default encoding is blank.
               IF( STATUS .NE. SAI__OK ) THEN
                  CALL ERR_ANNUL( STATUS )
                  DEFENC = ' '
               END IF

*  Re-order the list of foreign encodings to put the default encoding
*  (if any) first. The order of the others is retained.
               J = DEFNCD
               DO I = DEFNCD, 1, -1
                  IF( DEFENC .NE. ASTCOD( I ) ) THEN
                     ASTCOD( J ) = ASTCOD( I )
                     J = J - 1
                  END IF
               END DO

               IF( J .GT. 0 ) ASTCOD( J ) = DEFENC

            END IF

         END IF

      END IF

*  Get the WCS information from the NDF, in the form of an AST FrameSet.
      CALL NDF_GTWCS( INDF, IWCS, STATUS )

*  AST cannot produce foreign FITS headers if the number of pixel axes
*  exceeds the number of WCS axes. However, if this is the case, we can
*  ignore any trailing pixel axes which span only a single pixel. This
*  allowance may make it possible to get the number of pixel axes down to
*  an acceptable level. Check to see if there are more pixel axes than
*  primary WCS axes.
      CALL NDF_BOUND( INDF, NDF__MXDIM, LBND, UBND, NDIM, STATUS )
      NWCS = AST_GETI( IWCS, 'NAXES', STATUS )
      IF( NDIM .GT. NWCS ) THEN

* If so, see if all the excess pixel axes are degenerate.
         OK = .TRUE.
         DO I = NWCS + 1, NDIM
            IF( LBND( I ) .NE. UBND( I ) ) OK = .FALSE.
         END DO

*  If so, we use the WCS FrameSet associated with a section of the NDF
*  which excludes the trailing degenerate axes.
         IF( OK ) THEN
            NDIM = NWCS
            CALL NDF_SECT( INDF, NDIM, LBND, UBND, INDF2, STATUS )
            CALL AST_ANNUL( IWCS, STATUS )
            CALL NDF_GTWCS( INDF2, IWCS, STATUS )
            CALL NDF_ANNUL( INDF2, STATUS )
         END IF

      END IF

*  Remove Frames from the FrameSet if they can be regenerated automatically
*  without needing any extra WCS information. We can remove the PIXEL and
*  FRACTION Frames from the WCS FrameSet because they can be regenerated
*  from the GRID Frame and do not need to be written out, thus reducing the
*  size of the FITS header. We can also remove the AXIS frame if it is
*  equivalent to the PIXEL Frame.
*  ========================================================================

*  The action of finding a Frame within a FrameSet modifies the current
*  Frame. So we save the index of the Current Frame first so that it can be
*  reinstated later.
      ICURR0 = AST_GETI( IWCS, 'CURRENT', STATUS )

*  Remove any FRACTION Frame from the FrameSet (unless it is the current
*  Frame). Update the index of the current Frame in case it has been
*  changed by the removal of the FRACTION Frame.
      IF( AST_FINDFRAME( IWCS, AST_FRAME( NDIM, ' ', STATUS ),
     :                   'FRACTION', STATUS ) .NE. AST__NULL ) THEN
         IFRAC = AST_GETI( IWCS, 'CURRENT', STATUS )
         IF( IFRAC .NE. ICURR0 ) THEN
            CALL AST_REMOVEFRAME( IWCS, IFRAC, STATUS )
            ICURR0 = AST_GETI( IWCS, 'CURRENT', STATUS )
         END IF
      END IF

*  Now try to find the AXIS Frame in the NDF. If succesful, the AXIS Frame
*  will become the current Frame. Save the index of the AXIS Frame.
      IF( AST_FINDFRAME( IWCS, AST_FRAME( NDIM, ' ', STATUS ), 'AXIS',
     :                   STATUS ) .NE. AST__NULL ) THEN
         IAXIS = AST_GETI( IWCS, 'CURRENT', STATUS )

*  If the AXIS Frame could not be found, indicate this by assigning the
*  "bad" value AST__NOFRAME to the AXIS Frame index.
      ELSE
         IAXIS = AST__NOFRAME
      END IF

*  Now try to find the PIXEL Frame in the NDF in the same way. The PIXEL
*  Frame becomes the current Frame if found.
      IF( AST_FINDFRAME( IWCS, AST_FRAME( NDIM, ' ', STATUS ), 'PIXEL',
     :                   STATUS ) .NE. AST__NULL ) THEN
         IPIXEL = AST_GETI( IWCS, 'CURRENT', STATUS )
      ELSE
         IPIXEL = AST__NOFRAME
      END IF

*  If both Frames were found, we now check to see if they are equivalent
*  (in which case the AXIS Frame can be removed). Get the Mapping between them.
      IF( IPIXEL .NE. AST__NOFRAME .AND. IAXIS .NE. AST__NOFRAME ) THEN
         MAP = AST_GETMAPPING( IWCS, IPIXEL, IAXIS, STATUS )

*  Remove the AXIS Frame if this mapping simplifies to a UnitMap (i.e. if
*  the two Frames are equivalent).
         IF( AST_ISAUNITMAP( AST_SIMPLIFY( MAP, STATUS ),
     :                       STATUS ) ) THEN
            CALL AST_REMOVEFRAME( IWCS, IAXIS, STATUS )

*  If the Frame was removed, the index of any Frames added after the
*  AXIS Frame will have been reduced by one. Correct the indices
*  of the original current Frame and the PIXEL Frame for this.
*  Invalidate the index of the AXIS Frame.
            IF( ICURR0 .GT. IAXIS .AND. ICURR0 .NE. AST__NOFRAME ) THEN
               ICURR0 = ICURR0 - 1
            ELSE IF( ICURR0 .EQ. IAXIS ) THEN
               ICURR0 = AST__NOFRAME
            END IF

            IF( IPIXEL .GT. IAXIS .AND. IPIXEL .NE. AST__NOFRAME ) THEN
               IPIXEL = IPIXEL - 1
            ELSE IF( IPIXEL .EQ. IAXIS ) THEN
               IPIXEL = AST__NOFRAME
            END IF

            IAXIS = AST__NOFRAME

         END IF
      END IF

*  If found, remove the PIXEL Frame from the FrameSet, correct the
*  AXIS Frame index, the current Frame index, and invalidate the PIXEL Frame
*  index. If the PIXEL Frame was the original Current Frame, then make the
*  AXIS Frame the current Frame.
      IF( IPIXEL .NE. AST__NOFRAME ) THEN
         CALL AST_REMOVEFRAME( IWCS, IPIXEL, STATUS )

         IF( IAXIS .GT. IPIXEL .AND. IAXIS .NE. AST__NOFRAME ) THEN
            IAXIS = IAXIS - 1
         END IF

         IF( ICURR0 .GT. IPIXEL .AND. ICURR0 .NE. AST__NOFRAME ) THEN
            ICURR0 = ICURR0 - 1
         ELSE IF( ICURR0 .EQ. IPIXEL ) THEN
            ICURR0 = IAXIS
         END IF

         IPIXEL = AST__NOFRAME
      END IF

*  Reinstate the current Frame if it is still there.
      IF( ICURR0 .NE. AST__NOFRAME ) THEN
         CALL AST_SETI( IWCS, 'CURRENT', ICURR0, STATUS )
      END IF

*  The AST FitsChan class (used by the AST_WRITE method) uses the Symbol
*  attributes of the AXIS Frame as the value for the FITS CTYPE keyword.
*  But the NDF library always sets these to "a1" "a2" etc, which is not
*  very useful. In some cases the Label attribute may be more useful.
*  We transfer the Label value to the Symbol value so long as the label
*  value is not a long descriptive phrase (as indicated by the presence
*  of any embedded spaces within the Label).
      IF( IAXIS .NE. AST__NOFRAME ) THEN
         AXFRM = AST_GETFRAME( IWCS, IAXIS, STATUS )

         DO IAXIS = 1, NDIM
            ATTR = 'LABEL('
            IAT = 6
            CALL CHR_PUTI( IAXIS, ATTR, IAT )
            CALL CHR_APPND( ')', ATTR, IAT )

            AXLAB = AST_GETC( AXFRM, ATTR( : IAT ), STATUS )
            IF( AXLAB .NE. ' ' ) THEN
               CALL CHR_LDBLK( AXLAB )
               LLEN = CHR_LEN( AXLAB )
               IF( INDEX( AXLAB( : LLEN ), ' ' ) .EQ. 0 ) THEN
                  CALL AST_CLEAR( AXFRM, ATTR( : IAT ), STATUS )

                  ATTR = 'SYMBOL('
                  IAT = 7
                  CALL CHR_PUTI( IAXIS, ATTR, IAT )
                  CALL CHR_APPND( ')', ATTR, IAT )
                  CALL AST_SETC( AXFRM, ATTR( : IAT ), AXLAB( : LLEN ),
     :                           STATUS )

               END IF
            END IF
         END DO

         CALL AST_ANNUL( AXFRM, STATUS )

      END IF

*  Exclude requested Frames.
*  =========================
      IF ( EXCLUD .NE. ' ' ) THEN

*  Look for the list.
         NEXCH = CHR_LEN( EXCLUD )
         IAT = 1
         MORE = .TRUE.
         START = 1
         DO WHILE ( MORE )
            CALL CHR_TOCHR( ',', EXCLUD, .TRUE., IAT )

*  No more commas, so this is the last name.
            IF ( IAT .GT. NEXCH ) THEN
               MORE = .FALSE.

*  Set the index of the final character of the current name.
               END = NEXCH
            ELSE
               END = IAT - 1
            END IF

*  Extract the current Frame in the list.
            EXCNAM = EXCLUD( START: END )
            CALL CHR_LDBLK( EXCNAM )
            CALL CHR_UCASE( EXCNAM )

*  Ignore PIXEL and FRACTION as these are already handled.  GRID
*  should remain.
            IF ( EXCNAM .NE. 'GRID' .AND.
     :           EXCNAM .NE. 'PIXEL' .AND.
     :           EXCNAM .NE. 'FRACTION' ) THEN

               IF ( AST_FINDFRAME( IWCS, AST_FRAME( NDIM, ' ', STATUS ),
     :                            EXCNAM, STATUS ) .NE. AST__NULL ) THEN
                  EXCFRM = AST_GETI( IWCS, 'CURRENT', STATUS )

*  If found, remove the chosen Frame from the FrameSet, correct the AXIS
*  Frame index, the current Frame index, and invalidate the PIXEL Frame
*  index. If the PIXEL Frame was the original Current Frame, then make
*  the AXIS Frame the current Frame.
                  CALL AST_REMOVEFRAME( IWCS, EXCFRM, STATUS )
               END IF
            END IF

*  Ready to extract the next name.
            IF ( MORE ) THEN
               IAT = IAT + 1
               START = IAT
            END IF
         END DO
      END IF

*  Write out any remaining WCS information.
*  ========================================

*  If the current Frame in the FrameSet is the GRID Frame then we do not
*  need to write any WCS information out since the GRID Frame is
*  implied by the FITS data array.
      IF( AST_GETC( AST_GETFRAME( IWCS, AST__CURRENT, STATUS ),
     :              'DOMAIN', STATUS ) .NE. 'GRID' ) THEN

*  We now clean the supplied FitsChan by removing any keywords which
*  may interfere with the interpretation of keywords added later. For
*  instance, if the supplied FitsChan contains astrometric information using
*  a CDi_j matrix (as produced by STSDAS), then writing a FITS-WCS encoding
*  to it will add a PCiiijjj matrix, but will not remove the original CDi_j
*  matrix. Firsrt, ensure the Encoding attribute is cleared first so that
*  all possible encodings will be read.
         CALL AST_CLEAR( FC, 'ENCODING', STATUS )

*  We will be ignoring any errors that occur whilst cleaning the
*  FitsChan, so start a new error context.
         CALL ERR_BEGIN( STATUS )

*  Set the Clean attribute in the FitsChan. This means that all cards
*  used in an attempt to read an object from the FitsChan will be removed
*  from the FitsChan, even if an error prevents the final object from
*  being created. Without this, an error during the read would result in
*  no cards being removed from the FitsChan. We want to remove WCS cards
*  even if the cards currently present are not legal.
         CALL AST_SETL( FC, 'Clean', .TRUE., STATUS )

*  Loop until all WCS cards have been removed from the FitsChan.
         MORE = .TRUE.
         DO WHILE( MORE )

*  Rewind the FitsChan so that the next call to AST_READ will read from
*  the first card.
            CALL AST_CLEAR( FC, 'CARD', STATUS )

*  Note the number of cards in the FitsChan, then attempt to read an
*  Object from the FitsChan (annul any error so that we can continue).
*  Check the number of cards remaining in the FitsChan afterwards. If no
*  cards have been removed by the read, then we leave the loop. Note, we
*  do not check whether an object was actually read or not since, even
*  though an error may have prevented an Object from being created, some
*  cards may have been used up in the attempt.
            NCARDP = AST_GETI( FC, 'Ncard', STATUS )
            OBJ = AST_READ( FC, STATUS )
            IF( STATUS .NE. SAI__OK ) CALL ERR_FLUSH( STATUS )
            NCARD = AST_GETI( FC, 'Ncard', STATUS )
            IF( NCARD .EQ. NCARDP ) MORE = .FALSE.

         END DO

*  End the error context and rest the FitsChan Clean flag.
         CALL ERR_END( STATUS )
         CALL AST_CLEAR( FC, 'Clean', STATUS )

*  Set the FitsChan to "end-of-file" so that new keywords get written
*  to the end of the FitsChan.
         CALL AST_SETI( FC, 'CARD', AST_GETI( FC, 'NCARD', STATUS ) + 1,
     :                  STATUS )

*  Attempt to write the object using each foreign encoding in turn
*  (native encodings are produced later).
         DO IENCOD = 1, DEFNCD

*  Get the next encoding.
            COD = ASTCOD( IENCOD )

*  Only use this encoding if it matches the requested encoding, or if
*  no encoding was requested.
            IF( ENCOD .EQ. ' ' .OR. COD .EQ. ENCOD ) THEN

*  Set the current encoding scheme used by the FitsChan.
               CALL AST_SETC( FC, 'ENCODING', COD, STATUS )

*  Abort if an error has occurred.
               IF( STATUS .NE. SAI__OK ) GO TO 999

*  Attempt to write the FrameSet to the FitsChan. If it was written
*  succesfully, inrement the number of encodings written to the header
*  and leave the encodings loop.
               IF( AST_WRITE( FC, IWCS, STATUS ) .EQ. 1 ) THEN
                  COF_WCSEX = COF_WCSEX + 1
                  GO TO 10
               END IF

            END IF

         END DO

 10      CONTINUE

*  If requested, ensure a NATIVE encoding is present in the header.
         IF( ENCOD .EQ. 'NATIVE' .OR. NATIVE .OR.
     :       ( ENCOD .EQ. ' ' .AND. COF_WCSEX .EQ. 0 ) ) THEN
            CALL AST_SETC( FC, 'ENCODING', 'NATIVE', STATUS )
            NOBJ = AST_WRITE( FC, IWCS, STATUS )
            IF( NOBJ .GT. 0 ) COF_WCSEX = COF_WCSEX + 1
         END IF

      END IF

 999  CONTINUE

      END

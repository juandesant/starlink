      SUBROUTINE DISPLAY( STATUS )
*+
*  Name:
*     DISPLAY

*  Purpose:
*     Displays a one- or two-dimensional NDF.

*  Language:
*     Starlink Fortran 77

*  Type of Module:
*     ADAM A-task

*  Invocation
*     CALL DISPLAY( STATUS )

*  Description:
*     This application displays a one- or two-dimensional NDF as an 
*     image on the current image-display device.  The minimum and 
*     maximum data values to be displayed can be selected in several 
*     ways (see parameter MODE).  Data values outside these limits are
*     displayed with the colour of the nearest limit.  A key showing the
*     relationship between colour and data value can be displayed (see
*     parameter KEY).
*
*     Annotated axes or a simple border can be drawn around the image
*     (see  parameters AXES and BORDER).  The appearance of these may be
*     controlled in detail (see parameters STYLE and BORSTYLE).  
*
*     A specified colour lookup table may optionally be loaded prior to 
*     displaying the image (see parameter LUT). For devices which reset 
*     the colour table when opened (such as postscript files), this may
*     be the only way of controlling the colour table.
*
*     The image is produced within the current graphics database picture
*     The co-ordinates at the centre of the image, and the scale of the
*     image can be controlled using parameters CENTRE, XMAGN and YMAGN.
*     Only the parts of the image that lie within the current picture
*     are visible; the rest is clipped.  The image is padded with bad
*     pixels if necessary.

*  Usage:
*     display in [comp] clear [device] mode [centre] [xmagn] [ymagn]
*        [out] { low=? high=?
*              { percentiles=?
*              { sigmas=?
*              mode

*  ADAM Parameters:
*     AXES = _LOGICAL (Read)
*        TRUE if labelled and annotated axes are to be drawn around the
*        image.  These display co-ordinates in the current co-ordinate 
*        Frame of the supplied NDF, and may be changed using application
*        WCSFRAME (see also parameter USEAXIS). The width of the margins
*        left for the annotation may be controlled using parameter
*        MARGIN.  The appearance of the axes (colours, founts, etc.) 
*        can be controlled using the STYLE parameter. [current value]
*     BADCOL = LITERAL (Read)
*        The colour with which to mark any bad (i.e. missing) pixels in
*        the display.  There are a number of options described below:
*
*        - "MAX" -- The maximum colour index used for the display of the
*        image.
*
*        - "MIN" -- The minimum colour index used for the display of the
*        image.
*
*        - An integer -- The actual colour index.  It is constrained 
*        between 0 and the maximum colour index available on the device.
*
*        - A named colour -- Uses the named colour from the palette, and
*        if it is not present, the nearest colour from the palette is 
*        selected.
*
*        If the colour is to remain unaltered as the lookup table is
*        manipulated choose an integer between 0 and 15, or a named
*        colour.  The suggested default is the current value. 
*        [current value]
*     BORDER = _LOGICAL (Read)
*        TRUE if a border is to be drawn around the regions of the
*        displayed image containing valid co-ordinates in the current
*        co-ordinate Frame of the NDF.  For instance, if the NDF 
*        contains an Aitoff all-sky map, then an elliptical border will
*        be drawn if the current co-ordinate Frame is galactic longitude
*        and latitude.  This is because pixels outside this ellipse have
*        undefined positions in galactic co-ordinates.  If, instead, the
*        current co-ordinate Frame had been pixel co-ordinates, then a
*        simple box would have been drawn containing the whole image.
*        This is because every pixel has a defined position in pixel
*        co-ordinates.  The appearance of the border (colour, width,
*        etc.) can be controlled using parameter BORSTYLE. 
*        [current value]
*     BORSTYLE = GROUP (Read)
*        A group of attribute settings describing the plotting style to
*        use for the border (see parameter BORDER). 
*
*        A comma-separated list of strings should be given in which each
*        string is either an attribute setting, or the name of a text
*        file preceded by an up-arrow character "^".  Such text files 
*        should contain further comma-separated lists which will be 
*        read and interpreted in the same manner.  Attribute settings 
*        are applied in the order in which they occur within the list, 
*        with later settings overriding any earlier settings given for 
*        the same attribute.
*
*        Each individual attribute setting should be of the form:
*
*           <name>=<value>
*        
*        where <name> is the name of a plotting attribute, and <value>
*        is the value to assign to the attribute.  Default values will
*        be used for any unspecified attributes.  All attributes will be
*        defaulted if a null value (!) is supplied.  See section 
*        "Plotting Attributes" in SUN/95 for a description of the 
*        available attributes.  Any unrecognised attributes are ignored 
*        (no error is reported). [current value] 
*     CENTRE = LITERAL (Read)
*        The co-ordinates of the data pixel to be placed at the centre
*        of the image, in the current co-ordinate Frame of the NDF 
*        (supplying a colon ":" will display details of the current
*        co-ordinate Frame).  The position should be supplied as a list 
*        of formatted axis values separated by spaces or commas.  See 
*        also parameter USEAXIS.  A null (!) value causes the centre of 
*        the image to be used. [!]
*     CLEAR = _LOGICAL (Read)
*        TRUE if the current picture is to be cleared before the image 
*        is displayed. [current value]
*     COMP = LITERAL (Read)
*        The NDF array component to be displayed.  It may be "Data",
*        "Quality", "Variance", or "Error" (where "Error" is an
*        alternative to "Variance" and causes the square root of the
*        variance values to be displayed).  If "Quality" is specified,
*        then the quality values are treated as numerical values (in
*        the range 0 to 255). ["Data"]
*     DEVICE = DEVICE (Read)
*        The name of the graphics device used to display the image.
*        The device must have at least 24 colour indices or greyscale 
*        intensities. [current image-display device]
*     FILL = _LOGICAL (Read)
*        If FILL is set to TRUE, then the image will be "stretched" to 
*        fill the current picture in both directions.  This can be 
*        useful when displaying images with markedly different
*        dimensions, such as two-dimensional spectra.  The dynamic
*        default is TRUE if the array being displayed is 
*        one-dimensional, and FALSE otherwise. []
*     HIGH = _DOUBLE (Read)
*        The data value corresponding to the highest pen in the colour 
*        table.  All larger data values are set to the highest colour
*        index when HIGH is greater than LOW, otherwise all data values
*        greater than HIGH are set to the lowest colour index.  The
*        dynamic default is the maximum data value.  There is an
*        efficiency gain when both LOW and HIGH are given on the
*        command line, because the extreme values need not be computed.
*        (Scale mode)
*     IN = NDF (Read)
*        The input NDF structure containing the data to be displayed.
*     KEY = _LOGICAL (Read)
*        TRUE if a key to the colour table is to be produced to the
*        right of the display.  This can take the form of a colour ramp,
*        a coloured histogram of pen indices, or graphs of RGB
*        intensities, all annotated with data value.  The form and 
*        appearance of this key can be controlled using parameter 
*        KEYSTYLE, and its horizontal position can be controlled using 
*        parameter KEYPOS.  If the key is required in a different 
*        location, set KEY=NO and use application LUTVIEW after 
*        displaying the image. [TRUE]
*     KEYPOS = _REAL (Read)
*        A value giving the gap between the right-hand edge of the
*        display and the left-hand edge of the key, given as a fraction
*        of the width of the current picture.  If a key is produced, 
*        then the right-hand margin specified by parameter MARGIN is 
*        ignored, and the value supplied for KEYPOS is used instead.
*        [current value]
*     KEYSTYLE = GROUP (Read)
*        A group of attribute settings describing the plotting style to 
*        use for the key (see parameter KEY). 
*
*        A comma-separated list of strings should be given in which each
*        string is either an attribute setting, or the name of a text 
*        file preceded by an up-arrow character "^".  Such text files
*        should contain further comma-separated lists which will be 
*        read and interpreted in the same manner.  Attribute settings 
*        are applied in the order in which they occur within the list, 
*        with later settings overriding any earlier settings given for 
*        the same attribute.
*
*        Each individual attribute setting should be of the form:
*
*           <name>=<value>
*        
*        where <name> is the name of a plotting attribute, and <value>
*        is the value to assign to the attribute.  Default values will
*        be used for any unspecified attributes.  All attributes will be
*        defaulted if a null value (!) is supplied.  See section 
*        "Plotting Attributes" in SUN/95 for a description of the 
*        available attributes.  Any unrecognised attributes are ignored 
*        (no error is reported). 

*        Axis 1 is always the "data value" axis.  So for instance, to
*        set the label for the data-value axis, assign a value to
*        "Label(1)" in the supplied style. 
*
*        To get a ramp key (the default), specify "form=ramp".  To
*        get a histogram key (a coloured histogram of pen indices), 
*        specify "form=histogram".  To get a graph key (three curves of
*        RGB intensities), specify "form=graph".  If a histogram key 
*        is produced, the population axis can be either logarithmic or 
*        linear.  To get a logarithmic population axis, specify 
*        "logpop=1".  To get a linear population axis, specify 
*        "logpop=0" (the default).  To annotate the long axis with pen
*        numbers instead of pixel value, specify "pennums=1" (the 
*        default, "pennums=0", shows pixel values). [current value]
*     LOW = _DOUBLE (Read)
*        The data value corresponding to the lowest pen in the colour 
*        table.  All smaller data values are set to the lowest colour
*        index when LOW is less than HIGH, otherwise all data values
*        smaller than LOW are set to the highest colour index.   The
*        dynamic default is the minimum data value.  There is an
*        efficiency gain when both LOW and HIGH are given on the
*        command line, because the extreme values need not be computed.
*        (Scale mode)
*     LUT = NDF (Read)
*        Name of the NDF containing a colour lookup table in its Data 
*        array; the lookup table is written to the image-display's 
*        colour table.  The purpose of this parameter is to provide a 
*        means of controlling the appearance of the image on certain 
*        devices, such as colour printers, that do not have a dynamic 
*        colour table (i.e. the colour table is reset when the device is
*        opened).  If used with dynamic devices (such as X-windows),
*        the new colour table remains after this application has
*        completed. A null value (! ) causes the existing colour table 
*        to be used.
*
*        The LUT must be two-dimensional, the dimension of the first
*        axis being 3, and the second being arbitrary.  The method used 
*        to compress or expand the colour table if the second dimension
*        is different from the number of unreserved colour indices is
*        controlled by parameter NN.  Also the LUT's values must lie in 
*        the range 0.0--1.0. [!]
*     MARGIN( 4 ) = _REAL (Read)
*        The widths of the margins to leave around the image for axis 
*        annotations, given as fractions of the corresponding dimension 
*        of the current picture.  The actual margins used may be 
*        increased to preserve the aspect ratio of the data.  Four 
*        values may be given, in the order: bottom, right, top, left.
*        If fewer than four values are given, extra values are used 
*        equal to the first supplied value.  If these margins are too 
*        narrow any axis annotation may be clipped.  If a null (!) value
*        is supplied, the value used is (for all edges); 0.15 if 
*        annotated axes are being produced; 0.04, if a simple border is
*        being produced; and 0.0 if neither border nor axes are being 
*        produced. [current value]
*     MODE = LITERAL (Read)
*        The method by which the maximum and minimum data values to be 
*        displayed are chosen.  The options are:
*
*        - "Faint" -- The image is scaled between the mean data value 
*        minus one standard deviation and the mean data value plus seven
*        standard deviations.  The scaling values are reported so that 
*        the faster Scale mode may be utilised later. 
*
*        - "Flash" -- The image is flashed onto the screen without any 
*        scaling at all.  This is the fastest option.
*
*        - "Percentiles" -- The image is scaled between the data values 
*        corresponding to two percentiles.  The scaling values are 
*        reported so that the faster Scale mode may be used later. 
*
*        - "Range" -- The image is scaled between the minimum and 
*        maximum data values.
*
*        - "Scale" -- You define the upper and lower limits between 
*        which the image is to be scaled.  The application reports the
*        maximum and the minimum data values for reference and makes 
*        these the suggested defaults.
*
*        - "Sigmas" -- The image is scaled between two 
*        standard-deviation limits.  The scaling values used are 
*        reported so that the faster Scale mode may be utilised later. 
*     NN = _LOGICAL (Read)
*        If TRUE the input lookup table is mapped to the colour table by
*        using the nearest-neighbour method.  This preserves sharp
*        edges and is better for lookup tables with blocks of colour.
*        If NN is FALSE linear interpolation is used, and this is
*        suitable for smoothly varying colour tables.  NN is ignored
*        unless LUT is not null. [FALSE]
*     NUMBIN  =  _INTEGER (Read)
*        The number of histogram bins used to compute percentiles for
*        scaling. (Percentiles mode) [2048]
*     OUT = NDF (Write)
*        A scaled copy of the displayed section of the image. Values in 
*        this output image are integer colour indices shifted to exclude
*        the indices reserved for the palette (i.e. the value zero 
*        refers to the first colour index following the palette).  The 
*        output NDF is intended to be used as the input data in 
*        conjunction with SCALE=FALSE.  If a null value (!) is supplied,
*        no output NDF will be created.  This parameter is not accessed 
*        when SCALE=FALSE. [!]
*     PERCENTILES( 2 ) = _REAL (Read)
*        The percentiles that define the scaling limits. For example,
*        [25,75] would scale between the quartile values. (Percentile
*        mode)
*     SCAHIGH = _DOUBLE (Write)
*        On exit, this holds the data value which corresponds to the
*        maximum colour index in the displayed image.  In Flash mode or
*        when there is  no scaling the highest colour index is returned.
*     SCALE = _LOGICAL (Read)
*        If TRUE the input data are to be scaled according to the value
*        of parameter MODE.  If it is FALSE, MODE is ignored, and the
*        input data are displayed as is (i.e. the data values are simply
*        converted to integer type and used as indices into the colour
*        table).  A value of zero refers to the first pen following the 
*        palette.  A FALSE value is intended to be used with data 
*        previously scaled by this or similar applications which have 
*        already performed the required scaling (see parameter OUT).  It
*        provides the quickest method of image display within this
*        application. [TRUE]
*     SCALOW = _DOUBLE (Write)
*        The data value scaled to the minimum colour index for display.
*        In Flash mode or when there is no scaling the lowest colour
*        index is used.  The current display linear-scaling minimum is
*        set to this value.
*     SIGMAS( 2 ) = _REAL (Read)
*        The standard-deviation bounds that define the scaling limits.
*        To obtain values either side of the mean both a negative and
*        a positive value are required.  Thus [-2,3] would scale
*        between the mean minus two and the mean plus three standard
*        deviations.  [3,-2] would give the negative of that.
*     STYLE = GROUP (Read)
*        A group of attribute settings describing the plotting style to
*        use for the annotated axes (see parameter AXES). 
*
*        A comma-separated list of strings should be given in which each
*        string is either an attribute setting, or the name of a text 
*        file preceded by an up-arrow character "^".  Such text files
*        should contain further comma-separated lists which will be 
*        read and interpreted in the same manner.  Attribute settings 
*        are applied in the order in which they occur within the list, 
*        with later settings overriding any earlier settings given for 
*        the same attribute.
*
*        Each individual attribute setting should be of the form:
*
*           <name>=<value>
*        
*        where <name> is the name of a plotting attribute, and <value>
*        is the value to assign to the attribute.  Default values will
*        be used for any unspecified attributes.  All attributes will be
*        defaulted if a null value (!) is supplied.  See section 
*        "Plotting Attributes" in SUN/95 for a description of the 
*        available attributes.  Any unrecognised attributes are ignored 
*        (no error is reported). [current value] 
*     USEAXIS = GROUP (Read)
*        USEAXIS is only accessed if the current co-ordinate Frame of 
*        the NDF has more than two axes.  A group of two strings should
*        be supplied specifying the two axes which are to be used when
*        annotating the image, and when supplying a value for parameter
*        CENTRE.  Each axis can be specified either by its integer index
*        within the current Frame (in the range 1 to the number of axes
*        in the current Frame), or by its symbol string.  A list of
*        acceptable values is displayed if an illegal value is supplied.
*        If a null (!) value is supplied, the axes with the same indices
*        as the two used pixel axes within the NDF are used. [!]
*     XMAGN = _REAL (Read)
*        The horizontal magnification for the image.  The default
*        value of 1.0 corresponds to 'normal' magnification in which the
*        the image fills the available space in at least one dimension.
*        A value larger than 1.0 makes each data pixel wider.  If this
*        results in the image being wider than the available space then
*        the the image will be clipped to display fewer pixels.  See
*        also parameters YMAGN, CENTRE and FILL. [1.0]
*     YMAGN = _REAL (Read)
*        The vertical magnification for the image.  The default
*        value of 1.0 corresponds to 'normal' magnification in which the
*        the image fills the available space in at least one dimension.
*        A value larger than 1.0 makes each data pixel taller.  If this
*        results in the image being taller than the available space then
*        the image will be clipped to display fewer pixels.  See also
*        parameters XMAGN, CENTRE and FILL.  If a null (!) value is
*        supplied, the value used is the value supplied for XMAGN. [!]

*  Arguments:
*     STATUS = INTEGER (Given and Returned)
*        The global status.

*  Examples:
*     display ngc6872 mode=p percentiles=[10,90] noaxes
*        Displays the NDF called ngc6872 on the current image-display
*        device.  The scaling is between the 10 and 90 per cent
*        percentiles of the image. No annotated axes are produced.
*     display vv256 mode=flash noaxes border
*             borstyle="colour=blue,style=2"
*        Displays the NDF called vv256 on the current image-display
*        device.  There is no scaling of the data; instead the modulus
*        of each pixel with respect to the number of colour-table
*        indices is shown.  No annotated axes are drawn, but a blue 
*        border is drawn around the image using PGPLOT line style 
*        number 2 (i.e. dashed lines).
*     display mode=fa axes style="^sty,grid=1" margin=0.2 clear 
*             out=video \
*        Displays the current NDF data component with annotated axes
*        after clearing the current picture on the current image-display
*        device. The appearance of the axes is specified in the text 
*        file sty, but this is modified by setting the Grid attribute
*        to 1 so that a co-ordinate grid is drawn across the plot. The 
*        margins around the image containing the axes are made slightly 
*        wider than normal. The scaling is between the -1 and +7 
*        standard deviations of the image around its mean.  The scaled 
*        data are stored in an NDF called video.
*     display video noscale \
*        Displays the data component of the NDF called video (created
*        in the previous example) without scaling within the current
*        picture on the current image-display device.
*     display in=cgs4a comp=v mode=sc low=1 high=5.2 device=xwindows
*        Displays the variance component of NDF cgs4a on the xwindows
*        device, scaling between 1 and 5.2.
*     display mydata centre="12:23:34 -22:12:23" xmagn=2 badcol="red" \
*        Displays the NDF called mydata centred on the position 
*        RA=12:23:34, DEC=-22:12:23. This assumes that the current
*        co-ordinate Frame in the NDF is an equatorial (RA/DEC) Frame.
*        The image is displayed with a magnification of 2 so that each
*        data pixel appears twice as large (on each axis) as normal. 
*        Fewer data pixels may be displayed to ensure the image fits 
*        within the available space in the current picture.  The current
*        scaling is used, and bad pixels are shown in red.
*     display ngc6872 mode=ra device=lj250 lut=pizza
*        Displays the NDF called ngc6872 on the LJ250 device. The
*        lookup table in the NDF called pizza is mapped on the LJ250's
*        colour table.  The scaling is between the minimum and maximum
*        of the image.

*  Notes:
*     -  For large images the resolution of the graphics device may 
*     allow only a fraction of the detail in the data to be plotted.  
*     Therefore, large images will be compressed by block averaging when
*     this can be done without loss of resolution in the displayed 
*     image.  This saves time scaling the data and transmitting them to
*     the image display.  Note that the default values for parameters
*     LOW and HIGH are the minimum and maximum values in the compressed
*     floating-point data.
*     -  The Title component in the NDF is used as the default title 
*     for the annotated axes.  If the NDF does not have a Title 
*     component, then the default title is taken from current 
*     co-ordinate Frame in the NDF.  This default may be overridden by 
*     specifying a value for the Title attribute using the STYLE
*     parameter. 
*     -  The application stores a number of pictures in the graphics
*     database in the following order: a FRAME picture containing the 
*     annotated axes, the image area, and the border; a DATA picture 
*     containing just the image area.  Note, the FRAME picture is only
*     created if annotated axes or a border have been drawn, or if
*     non-zero margins were specified using parameter MARGIN.  The world
*     co-ordinates in the DATA picture will be pixel co-ordinates.  A
*     reference to the supplied NDF, together with a copy of the WCS 
*     information in the NDF are stored in the DATA picture.  On exit 
*     the current database picture for the chosen device reverts to the 
*     input picture.
*     -  The data type of the output NDF depends on the number of colour
*     indices: _UBYTE for no more than 256, _UWORD for 257 to 65535,
*     and _INTEGER otherwise.   The output NDF will not contain any
*     extensions, UNITS, QUALITY, and VARIANCE; but LABEL, TITLE, WCS 
*     and AXIS information are propagated from the input NDF.  The 
*     output NDF does not become the new current data array.  It is a
*     Simple NDF (because the bad-pixel flag is set to false in order to
*     access the maximum colour index, and to handle sections),
*     therefore only NDF-compliant applications can process it.

*  Related Applications:
*     KAPPA: WCSFRAME, PICDEF; Figaro: IGREY, IMAGE; SPECDRE: MOVIE.

*  Implementation Status:
*     -  This routine correctly processes the AXIS, DATA, QUALITY,
*     VARIANCE, LABEL, TITLE, WCS and UNITS components of the input NDF.
*     -  Processing of bad pixels and automatic quality masking are
*     supported.
*     -  This application will handle data in all numeric types, though
*     type conversion to integer will occur for unsigned byte and word
*     images.  However, when there is no scaling only integer data will
*     not be type converted, but this is not expensive for the expected
*     byte-type data.

*  Authors:
*     MJC: Malcolm J. Currie (STARLINK)
*     DSB: David S. Berry (STARLINK)
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     {enter_new_authors_here}

*  History:
*     1990 July 12 (MJC):
*        Original version based on the pre-V0.7 DISPLAY.
*     1990 August 19 (MJC):
*        Only the visible portion is scaled and displayed.  Redefined
*        CENTRE (ex XCENTR and YCENTR) in terms of image pixels.
*     1990 November 9 (MJC):
*        Block averaging added.
*     1991 February 7 (MJC):
*        Added NDF reference into the database, AGI context control and
*        removed fuzzy-picture fudge.
*     1991 March 19 (MJC):
*        Added output parameters for the scaling limits.
*     1991 April 4 (MJC):
*        Added data co-ordinate transformation.
*     1991 May 7 (MJC):
*        Added the input and output of NDFs containing scaled data.
*     1991 May 14 (MJC):
*        Added direct processing of _BYTE and _WORD data.
*     1991 July 22 (MJC):
*        Added coloured-border option, and user-controlled colouring of
*        bad pixels.
*     1991 July 31 (MJC):
*        No longer redefines first SGS pen to white if workstation has
*        dynamic colour representation, now there is palette control.
*     1991 August 20 (MJC):
*        Added FONT parameter.
*     1991 August 22 (MJC):
*        Added input lookup-table facility via LUT and NN parameters,
*        and an illustrative example of same.
*     1992 January 22 (MJC):
*        Fixed a bug that could cause the NOSCALE option to scale.
*     1992 March 3 (MJC):
*        Replaced AIF parameter-system calls by the extended PAR
*        library.
*     1992 March 30 (MJC):
*        Revised defaulting behaviour of BADCOL and BCOLOUR, and
*        corrected the notes.  Added an example of their use.  Handles
*        arbitrary user-defined two-dimensional sections.
*     1992 November 27 (MJC):
*        Does not use non-monotonic axis centres.
*     1992 December 17 (MJC):
*        Added the FILL option.
*     1995 October 19 (MJC):
*        Supports Error component.
*     1997 May 28 (MJC):
*        QUALITY and HISTORY no longer propagated to the OUT NDF.
*     5-JUN-1998 (DSB):
*        Added propagation of the WCS component.
*     14-JUL-1998 (DSB):
*        Use '_DOUBLE' instead of 'DOUBLE' in call to NDF_MTYPE.
*     26-AUG-1998 (DSB)
*        Radical changes to use AST and PGPLOT.
*     3-SEP-1999 (DSB):
*        Added NULL argument to KPG1_GTPOS call.
*     18-OCT-1999 (DSB):
*        Added parameters KEY, KEYPOS and KEYSTYLE.
*     26-OCT-1999 (DSB):
*        Made MARGIN give margins as fraction of current picture instead
*        of DATA picture.
*     23-OCT-2001 (DSB):
*        Changed to remove limit on size of colour table.
*     2004 September 3 (TIMJ):
*        Use CNF_PVAL
*     {enter_further_changes_here}

*  Bugs:
*     {note_new_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE

*  Global Constants:
      INCLUDE 'SAE_PAR'        ! Global SSE definitions
      INCLUDE 'NDF_PAR'        ! NDF_ constants
      INCLUDE 'PRM_PAR'        ! NUM_ constants
      INCLUDE 'AST_PAR'        ! AST_ constants
      INCLUDE 'PAR_ERR'        ! Parameter-system error definitions
      INCLUDE 'PAR_PAR'        ! Parameter-system constants
      INCLUDE 'CTM_PAR'        ! Colour-table management constants
      INCLUDE 'CNF_PAR'        ! For CNF_PVAL function

*  Status:
      INTEGER STATUS

*  External References:
      INTEGER CHR_LEN          ! Used length of a string
      INTEGER KPG1_CEIL        ! Smallest integer .GE. a given floating 
                               ! value
      INTEGER KPG1_FLOOR       ! Largest integer .LE. a given floating
                               ! value

*  Local Constants:
      REAL KW                  ! Width of key picture as a fraction of
      PARAMETER ( KW = 0.13 )  ! the width of the current picture

      INTEGER MINCOL           ! Minimum number of colour indices on
                               ! device to be classed as an image
                               ! display
      PARAMETER ( MINCOL = 8 + CTM__RSVPN )

      INTEGER NDIM             ! Dimensionality required
      PARAMETER( NDIM = 2 )

*  Local Variables:
      CHARACTER COMP*8         ! Component to be displayed
      CHARACTER FORM*( NDF__SZFRM ) ! Form of the output data array
      CHARACTER LABEL*255      ! Label for key
      CHARACTER MCOMP*8        ! Component to be mapped
      CHARACTER NDFNAM*255     ! Full NDF specification 
      CHARACTER OTYPE*( NDF__SZTYP )! Processing type of the output
                               ! image
      CHARACTER TITLE*255      ! Default title for the plot
      DOUBLE PRECISION BOX( 4 )! Bounds of image in pixel co-ordinates
      DOUBLE PRECISION CC( 2 ) ! Current Frame co-ords at image centre
      DOUBLE PRECISION DHI     ! Upper displayed data value limit
      DOUBLE PRECISION DLO     ! Lower displayed data value limit
      DOUBLE PRECISION GC( 2 ) ! GRID co-ords at image centre
      INTEGER BPCI             ! Bad-pixel colour index
      INTEGER DIMS( NDIM )     ! Dimensions of input array
      INTEGER EL               ! Number of elements in the mapped array
      INTEGER I                ! General variable
      INTEGER IPICK            ! AGI identifier for KEY picture
      INTEGER IERR             ! Position of first conversion error
      INTEGER INDF1            ! NDF identifier for input NDF
      INTEGER INDF2            ! NDF identifier for displayed NDF
                               ! section
      INTEGER INDF3            ! NDF identifier for output NDF
      INTEGER INDF4            ! NDF identifier for NDF containing LUT
      INTEGER IPCOL            ! Pointer to colour index array
      INTEGER IPIC0            ! AGI id. for original current picture
      INTEGER IPICD            ! AGI id. for DATA picture
      INTEGER IPICF            ! AGI id. for new FRAME picture
      INTEGER IPLOT            ! Pointer to AST Plot for DATA picture
      INTEGER IPWORK           ! Pointer to work array for key
      INTEGER IWCS             ! Pointer to WCS FrameSet from the NDF
      INTEGER LBND( NDF__MXDIM )! Lower pixel-index bounds of the image
      INTEGER LDIMS( NDIM )    ! Dimensions of LUT arrays
      INTEGER LEL              ! Number of elements in i/p and o/p LUT 
                               ! arrays
      INTEGER LP               ! Lowest pen with which to display image
      INTEGER LPNTR( 1 )       ! Pointer to input colour table
      INTEGER NC               ! Number of characters in NDFNAM
      INTEGER NDIMS            ! Total number of NDF dimensions
      INTEGER NERR             ! Number of conversion errors
      INTEGER NFRM             ! Frame index increment between IWCS and
                               ! IPLOT
      INTEGER NMARG            ! No. of margin values given
      INTEGER NX               ! First dimension of colour index array
      INTEGER NY               ! Second dimension of colour index array
      INTEGER OPNTR( 1 )       ! Pointer to output array data
      INTEGER SDIM( NDIM )     ! The significant NDF axes
      INTEGER SLBND( NDIM )    ! Significant lower bounds of the image
      INTEGER STATE            ! State of a parameter
      INTEGER SUBND( NDIM )    ! Significant upper bounds of the image
      INTEGER UBND( NDF__MXDIM )! Upper pixel-index bounds of the image
      INTEGER UP               ! Highest pen with which to display image
      INTEGER WDIM( NDIM )     ! Dimensions in pixels of PGPLOT window
      INTEGER WILBND( NDIM )   ! Lower pixel-index bounds of NDF section
      INTEGER WIUBND( NDIM )   ! Upper pixel-index bounds of NDF section
      LOGICAL ALIGN            ! DATA picture aligned with a previous
                               ! picture?
      LOGICAL AXES             ! Annotated axes are to be drawn?
      LOGICAL BAD              ! Bad pixels are present in the image?
      LOGICAL BORDER           ! Border to be drawn?
      LOGICAL DEVCAN           ! Cancel DEVICE parameter?
      LOGICAL KEY              ! Produce a colour ramp as a key?
      LOGICAL NN               ! Map the LUT via nearest-neighbour
                               ! method?
      LOGICAL SCALE            ! Does the input array need to be scaled?
      REAL ASP0                ! Aspect ratio of the available space
      REAL ASPD                ! Aspect ratio of the data array
      REAL ASPECT              ! Aspect ratio of the DATA picture
      REAL DEFMAR              ! Default MARGIN value
      REAL GLBND( NDIM )       ! Low grid co-ord bounds of PGPLOT window
      REAL GUBND( NDIM )       ! High grid co-ord bounds of PGPLOT
                               ! window
      REAL KEYPOS              ! Horizontal position of key
      REAL KWID                ! Width to reserve for the KEY (if any)
      REAL MARGIN( 4 )         ! Width of margins round DATA picture
      REAL OPLBND( NDIM )      ! Low pixel co-ord bounds of NDF overlap
      REAL OPUBND( NDIM )      ! High pixel co-ord bounds of NDF overlap
      REAL PCLBND( NDIM )      ! Low pixel co-ord bounds of PGPLOT
                               ! window
      REAL PCUBND( NDIM )      ! High pixel co-ord bounds of PGPLOT
                               ! window
      REAL WPLBND( NDIM )      ! Low pixel co-ord bounds of NDF section
      REAL WPUBND( NDIM )      ! High pixel co-ord bounds of NDFsection
      REAL X1, X2, Y1, Y2      ! Bounds of current picture viewport in
                               ! millimetres
      REAL XMAGN               ! X magnification
      REAL XTENT               ! Extent in X of the PGPLOT window in 
                               ! NDF pixels
      REAL XVMAG               ! X magnification factor for viewport
      REAL XWMAG               ! X magnification factor for window
      REAL YMAGN               ! y magnification
      REAL YTENT               ! Extent in Y of the PGPLOT window in 
                               ! NDF pixels
      REAL YVMAG               ! Y magnification factor for viewport
      REAL YWMAG               ! Y magnification factor for window

*.

*  Check the inherited global status.
      IF( STATUS .NE. SAI__OK ) RETURN

*  Begin an AST context.
      CALL AST_BEGIN( STATUS )

*  Begin an NDF context.
      CALL NDF_BEGIN

*  Get the main parameters of the data to be displayed.
*  =====================================================

*  Obtain the identifier of the NDF to be displayed.
      CALL LPG_ASSOC( 'IN', 'READ', INDF1, STATUS )

*  Find which component to display.  MCOMP is for use with NDF_MAP and 
*  may be set to 'Error'.  COMP is for use with all other NDF routines 
*  (which do not accept 'Error' as an NDF component name), and has
*  'Variance' in place of 'Error'.
      CALL KPG1_ARCOG( 'COMP', INDF1, MCOMP, COMP, STATUS )

*  Get an AST pointer to a FrameSet describing the co-ordinate Frames
*  present in the NDF's WCS component.  Modify it to ensure that the 
*  Base, PIXEL and Current frames all have two dimensions.  The NDF must
*  have no more than two significant dimensions (i.e. axes spanning more
*  than one pixel).  A single significant axis is allowed.
      CALL KPG1_ASGET( INDF1, NDIM, .FALSE., .TRUE., .TRUE., SDIM, 
     :                 SLBND, SUBND, IWCS, STATUS )

*  Store the size of each significant dimension.
      DIMS( 1 ) = SUBND( 1 ) - SLBND( 1 ) + 1
      DIMS( 2 ) = SUBND( 2 ) - SLBND( 2 ) + 1

*  Get the name of the NDF.  This is later stored in the graphics
*  database.
      CALL NDF_MSG( 'NDF', INDF1 )
      CALL MSG_LOAD( ' ', '^NDF', NDFNAM, NC, STATUS )

*  Determine the width of the margins to leave around the DATA picture.
*  ====================================================================

*  See if annotated axes are required. 
      CALL PAR_GET0L( 'AXES', AXES, STATUS )

*  See if a border is also required.
      CALL PAR_GET0L( 'BORDER', BORDER, STATUS )

*  Set the dynamic default for the margins to place around the DATA
*  picture (a single value is used for all edges), and then get the
*  margins to use.  Negative margins can be used, but the sum of the
*  two margins in one any dimension must be greater than -1.0. 
*  Therefore limit each margin to be greater than -0.49.
      IF( AXES ) THEN
         DEFMAR = 0.15
      ELSE IF( BORDER ) THEN
         DEFMAR = 0.04
      ELSE
         DEFMAR = 0.0
      END IF

      CALL PAR_DEF1R( 'MARGIN', 1, DEFMAR, STATUS )

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

      CALL PAR_GDRVR( 'MARGIN', 4, -0.49, 10.0, MARGIN, NMARG, STATUS )
      IF ( STATUS .EQ. PAR__NULL ) THEN
         CALL ERR_ANNUL( STATUS )
         NMARG = 1
         MARGIN( 1 ) = DEFMAR
      END IF

      NMARG = MIN( 4, NMARG )

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  Use the first value for any unspecified edges.
      DO I = NMARG + 1, 4
         MARGIN( I ) = MARGIN( 1 )
      END DO

*  See if a key is also required.
      CALL PAR_GET0L( 'KEY', KEY, STATUS )

*  If so, see how large a gap is required between the DATA picture and
*  the key. This replaces the MARGIN value for the right hand edge.
      IF( KEY ) THEN
         CALL PAR_GDR0R( 'KEYPOS', 0.0, -1.0, 0.99 - MARGIN( 4 ) - KW, 
     :                   .FALSE., KEYPOS, STATUS )
         IF( KEYPOS .GE. 0.0 ) THEN
            MARGIN( 2 ) = KEYPOS
         ELSE
            MARGIN( 2 ) = KEYPOS - KW
         END IF
         KWID = KW
      ELSE
         KWID = 0.0
      END IF

*  Report an error if the margins do not leave any room for the DATA
*  picture.
      IF( ( 1.0 - MARGIN( 1 ) - MARGIN( 3 ) .LE. 0.005 .OR.
     :      1.0 - MARGIN( 2 ) - MARGIN( 4 ) - KWID .LE. 0.005 ) .AND. 
     :    STATUS .EQ. SAI__OK ) THEN
         STATUS = SAI__ERROR

         IF( KEY ) THEN
            CALL ERR_REP( 'DISPLAY_ERR1', 'No room left for the DATA '//
     :                    'picture (try reducing the size of the '//
     :                    'margins or key - see parameters MARGIN '//
     :                    'and KEYPOS).', STATUS )
         ELSE 
            CALL ERR_REP( 'DISPLAY_ERR2', 'No room left for the DATA '//
     :                    'picture (try reducing the size of the '//
     :                    'margins - see parameter MARGIN).', STATUS )
         END IF

         GO TO 999

      END IF

*  Determine the section of the supplied NDF to be displayed.
*  ==========================================================

*  First get the aspect ratio of the available space in which the image
*  can be displayed.  This is the current picture, minus any requested
*  margins.  To do this we need to temporarily open the AGI database and
*  PGPLOT plotting package, so that we can have a look at the current
*  picture.  Do not clear the picture (it may be cleared when it is next
*  opened within KPG1_PLOT).
      CALL AGP_ASSOC( 'DEVICE', 'UPDATE', ' ', .FALSE., IPIC0, STATUS )

*  If the device could not be opened, indicate that the parameter
*  association should be cancelled when the device is closed so that a
*  new device name will be obtained when the parameter is next accessed.
      DEVCAN = .FALSE.

      IF( STATUS .NE. SAI__OK .AND. STATUS .NE. PAR__ABORT .AND.
     :   STATUS .NE. PAR__NULL ) THEN
         DEVCAN = .TRUE.
     
*  If successful, store the bounds of the viewport for the current
*  picture (in millimetres).
      ELSE IF( STATUS .EQ. SAI__OK ) THEN
         CALL PGQVP( 2, X1, X2, Y1, Y2 )

*  Check the viewport does not have zero area.
         IF( X2 .EQ. X1 ) THEN
            STATUS = SAI__ERROR
            CALL ERR_REP( 'DISPLAY_3', 'Current AGI picture has zero '//
     :                    'width.', STATUS )

         ELSE IF( Y2 .EQ. Y1 ) THEN
            STATUS = SAI__ERROR
            CALL ERR_REP( 'DISPLAY_4', 'Current AGI picture has zero '//
     :                    'height.', STATUS )
         END IF

      END IF

*  Close down the graphics device and AGI database.
      CALL ERR_BEGIN( STATUS ) 
      CALL AGP_DEASS( 'DEVICE', DEVCAN, STATUS )
      CALL ERR_END( STATUS )

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  Now find the aspect ratio of the available space (i.e. the current
*  picture minus the margins). 
      ASP0 = ( ( Y2 - Y1 )*( 1.0 - MARGIN( 1 ) - MARGIN( 3 ) ) ) /
     :       ( ( X2 - X1 )*( 1.0 - MARGIN( 2 ) - MARGIN( 4 ) - KWID ) ) 

*  Get the aspect ratio of the supplied data array.
      ASPD = REAL( DIMS( 2 ) )/ REAL( DIMS( 1 ) )

*  If there would be space at left and right when the array fills the
*  height of the available space, then the viewport used for the data
*  array can be expanded in X but not in Y.  Store the maximum allowable
*  magnifications of the viewport used to display the data in X and Y.
      IF( ASPD .GT. ASP0 ) THEN
         XVMAG = ASPD / ASP0
         YVMAG = 1.0

*  If there would be space at top and bottom when the array fills the
*  width of the available space, then the viewport used for the data
*  array can be expanded in Y but not in X.
      ELSE
         XVMAG = 1.0
         YVMAG = ASP0 / ASPD
      END IF

*  Get the positive magnifications required for both axes.  Use a 
*  dynamic default for YMAGN equal to the value supplied for XMAGN.  A 
*  magnification of 1.0 results in the whole image being displayed 
*  within the current picture so that it fills the available space in
*  at least one dimension.
      CALL PAR_GDR0R( 'XMAGN', 0.0, 1.0E-6, 1.0E6, .FALSE., XMAGN, 
     :                STATUS )
      CALL PAR_GDR0R( 'YMAGN', XMAGN, 1.0E-6, 1.0E6, .TRUE., YMAGN, 
     :                STATUS )

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  If the magnification in X is greater than the maximum allowed by just
*  expanding the width of the viewport, then we also need to reduce the
*  size of the window (so that fewer pixels are visible within the
*  expanded viewport).
      IF( XMAGN .GT. XVMAG ) THEN
         XWMAG = XVMAG/XMAGN

*  Otherwise, we can achieve the requested magnification without
*  reducing the window.
      ELSE
         XVMAG = XMAGN
         XWMAG = 1.0
      END IF

*  Do the same for the Y axis.
      IF( YMAGN .GT. YVMAG ) THEN
         YWMAG = YVMAG / YMAGN
      ELSE
         YVMAG = YMAGN
         YWMAG = 1.0
      END IF

*  Determine the aspect ratio of the viewport for the DATA picture.
      ASPECT = ASPD * YVMAG / XVMAG

*  Determine the extent of the corresponding window in pixels.
      XTENT = REAL( DIMS( 1 ) ) * XWMAG
      YTENT = REAL( DIMS( 2 ) ) * YWMAG
      
*  Get the GRID co-ordinates at the centre of the supplied NDF.  GRID
*  co-ordinates are (1.0,1.0) at the centre of the first pixel.
      GC( 1 ) = ( 1.0D0 + DBLE( DIMS( 1 ) ) ) / 2.0D0
      GC( 2 ) = ( 1.0D0 + DBLE( DIMS( 2 ) ) ) / 2.0D0

*  Convert these into the Current Frame of the NDF. 
      CALL AST_TRAN2( IWCS, 1, GC( 1 ), GC( 2 ), .TRUE., CC( 1 ), 
     :                CC( 2 ), STATUS )

*  Try to convert these back to grid.  The current Frame is not suitable
*  for specifying a centre position if any of the returned values are
*  bad.
      CALL AST_TRAN2( IWCS, 1, CC( 1 ), CC( 2 ), .FALSE., GC( 1 ), 
     :                GC( 2 ), STATUS )
      IF( GC( 1 ) .EQ. AST__BAD .OR. GC( 2 ) .EQ. AST__BAD ) THEN
         IF( STATUS .EQ. SAI__OK ) THEN
            STATUS = SAI__ERROR
            CALL ERR_REP( 'DISPLAY_ERR4B', 'The Mapping from the '//
     :                    'current WCS co-ordinate Frame to pixel '//
     :                    'co-ordinates is undefined at the image '//
     :                    'centre.', STATUS )
            CALL ERR_REP( 'DISPLAY_ERR4C',  'Try displaying a section'//
     :                    ' of the image, or changing the current '//
     :                    'Frame to PIXEL using the KAPPA wcsframe '//
     :                    'command.', STATUS )
         END IF
         GO TO 999
      END IF

*  If the centre pixel of the supplied NDF has no defined position then 
*  we only access the CENTRE parameter if a value was supplied on the
*  command line.  Otherwise, just use the centre of the GRID frame as 
*  the centre for the displayed image.
      CALL LPG_STATE( 'CENTRE', STATE, STATUS )
      IF( ( CC( 1 ) .NE. AST__BAD .AND. CC( 2 ) .NE. AST__BAD ) .OR.
     :    STATE .EQ. PAR__ACTIVE ) THEN

*  Obtain the Current Frame co-ordinates (returned in CC) to put at the 
*  centre of the picture using parameter CENTRE.  Use the Current Frame 
*  co-ordinates at the centre of the image as the dynamic default (they 
*  will be ignored if they are bad).  KPG1_GTPOS loops until 
*  co-ordinates are obtained that are valid in the Base Frame (i.e. 
*  GRID Frame in our case) of the supplied FrameSet.  These GRID 
*  co-ordinates are returned in GC.
         CALL KPG1_GTPOS( 'CENTRE', IWCS, .TRUE., CC, GC, STATUS )

      END IF

*  Find the corresponding upper and lower bounds in GRID co-ordinates.
      GLBND( 1 ) = REAL( GC( 1 ) ) - 0.5*XTENT 
      GUBND( 1 ) = GLBND( 1 ) + XTENT 
      GLBND( 2 ) = REAL( GC( 2 ) ) - 0.5*YTENT 
      GUBND( 2 ) = GLBND( 2 ) + YTENT 

*  Find the equivalent bounds in pixel co-ordinates. 
      PCLBND( 1 ) = GLBND( 1 ) - 1.5 + REAL( SLBND( 1 ) )
      PCUBND( 1 ) = GUBND( 1 ) - 1.5 + REAL( SLBND( 1 ) )
      PCLBND( 2 ) = GLBND( 2 ) - 1.5 + REAL( SLBND( 2 ) )
      PCUBND( 2 ) = GUBND( 2 ) - 1.5 + REAL( SLBND( 2 ) )

*  Find the pixel-index bounds of the NDF section.
      WILBND( 1 ) = KPG1_FLOOR( PCLBND( 1 ) ) + 1
      WIUBND( 1 ) = KPG1_CEIL( PCUBND( 1 ) )
      WILBND( 2 ) = KPG1_FLOOR( PCLBND( 2 ) ) + 1
      WIUBND( 2 ) = KPG1_CEIL( PCUBND( 2 ) )

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  These bounds are for the two significant axes which will, in general,
*  correspond to different axes in the supplied NDF.  Store the bounds 
*  of the section to be displayed within the supplied NDF.
      CALL NDF_BOUND( INDF1, NDF__MXDIM, LBND, UBND, NDIMS, STATUS )
      LBND( SDIM( 1 ) ) = WILBND( 1 )
      UBND( SDIM( 1 ) ) = WIUBND( 1 )
      LBND( SDIM( 2 ) ) = WILBND( 2 )
      UBND( SDIM( 2 ) ) = WIUBND( 2 )

*  Obtain an NDF identifier for this section of the supplied NDF.
      CALL NDF_SECT( INDF1, NDIMS, LBND, UBND, INDF2, STATUS ) 

*  Store the significant dimensions of this NDF section.
      WDIM( 1 ) = WIUBND( 1 ) - WILBND( 1 ) + 1
      WDIM( 2 ) = WIUBND( 2 ) - WILBND( 2 ) + 1

*  Store the pixel co-ordinate bounds of the section (this may be
*  different to PCLBND/PCUBND due to rounding of the pixel indices).
      WPLBND( 1 ) = REAL( WILBND( 1 ) ) - 1.0
      WPUBND( 1 ) = REAL( WIUBND( 1 ) )
      WPLBND( 2 ) = REAL( WILBND( 2 ) ) - 1.0
      WPUBND( 2 ) = REAL( WIUBND( 2 ) )

*  Find the pixel co-ordinate bounds of the section of the original NDF
*  that falls within the DATA picture.
      OPLBND( 1 ) = MIN( REAL( SUBND( 1 ) ), 
     :                   MAX( REAL( SLBND( 1 ) ) - 1.0, WPLBND( 1 ) ) )
      OPUBND( 1 ) = MIN( REAL( SUBND( 1 ) ), 
     :                   MAX( REAL( SLBND( 1 ) ) - 1.0, WPUBND( 1 ) ) )
      OPLBND( 2 ) = MIN( REAL( SUBND( 2 ) ), 
     :                   MAX( REAL( SLBND( 2 ) ) - 1.0, WPLBND( 2 ) ) )
      OPUBND( 2 ) = MIN( REAL( SUBND( 2 ) ), 
     :                   MAX( REAL( SLBND( 2 ) ) - 1.0, WPUBND( 2 ) ) )

*  Report an error if there is no overlap.
      IF( OPLBND( 1 ) .EQ. OPUBND( 1 ) .OR. OPLBND( 2 ) .EQ. OPUBND( 2 )
     :    .AND. STATUS .EQ. SAI__OK ) THEN
         STATUS = SAI__ERROR
         CALL ERR_REP( 'DISPLAY_5', 'The values supplied for '//
     :                 'parameters %CENTRE, %XMAGN and %YMAGN would '//
     :                 'result in no part of the image being '//
     :                 'displayed.', STATUS )
      END IF

*  Start the graphics system.
*  ==========================

*  If the array being displayed is one-dimensional, set a dynamic
*  default of TRUE for parameter FILL.
      IF( WDIM( 1 ) .EQ. 1 .OR. WDIM( 2 ) .EQ. 1 ) THEN
         CALL PAR_DEF0L( 'FILL', .TRUE., STATUS )
      ELSE
         CALL PAR_DEF0L( 'FILL', .FALSE., STATUS )
      END IF

*  Store the pixel co-ordinate bounds to be stored with the new DATA
*  picture in the AGI database.
      BOX( 1 ) = DBLE( PCLBND( 1 ) )
      BOX( 2 ) = DBLE( PCLBND( 2 ) )
      BOX( 3 ) = DBLE( PCUBND( 1 ) )
      BOX( 4 ) = DBLE( PCUBND( 2 ) )

*  Start up the graphics system again.  This stores a new DATA picture 
*  in the AGI database with the given pixel co-ordinate bounds
*  (enclosing FRAME and KEY pictures that may also be created).  The 
*  PGPLOT viewport is set so that it matches the area of the DATA 
*  picture.  World co-ordinates within the PGPLOT window are set to 
*  millimetres from the bottom-left corner of the view surface.  An AST 
*  Plot is returned for drawing in the DATA picture.  The Base 
*  (GRAPHICS) Frame in the Plot corresponds to millimetres from the 
*  bottom-left corner of the view surface, and the Current Frame is 
*  inherited form the NDF's WCS FrameSet.

*  First deal with cases where a key is required...
      IF( KEY ) THEN

*  Start up the graphics system, creating a KEY picture.
         CALL KPG1_PLOT( IWCS, 'NEW', 'KAPPA_DISPLAY', NDFNAM( : NC ), 
     :                   MARGIN, 1, 'KEY', 'R', KW, ASPECT, 'PIXEL', 
     :                   BOX, IPICD, IPICF, IPICK, IPLOT, NFRM, ALIGN, 
     :                   STATUS )

*  Otherwise, start up the graphics system, creating no KEY picture.
      ELSE
         CALL KPG1_PLOT( IWCS, 'NEW', 'KAPPA_DISPLAY', NDFNAM( : NC ), 
     :                   MARGIN, 0, ' ', ' ', 0.0, ASPECT, 'PIXEL', 
     :                   BOX, IPICD, IPICF, IPICK, IPLOT, NFRM, ALIGN, 
     :                   STATUS )
      END IF

*  If the user did not specify a Plot title (as indicated by the Plot 
*  title being the same as the WCS title), make the NDF Title the
*  default Title for the Plot.  We have to be careful about the timing 
*  of this change to the Title.  If we did it before KPG1_PLOT (i.e. if 
*  we set the Title in IWCS) it may prevent alignment ocurring within 
*  KPG1_PLOT since alignment fails if the Title of two Frames differ.
      IF( AST_GETC( IWCS, 'TITLE', STATUS ) .EQ. 
     :    AST_GETC( IPLOT, 'TITLE', STATUS ) ) THEN

         TITLE = ' '
         CALL NDF_CGET( INDF1, 'TITLE', TITLE, STATUS ) 

         IF( TITLE .NE. ' ' ) THEN
            CALL AST_SETC( IPLOT, 'TITLE', TITLE( : CHR_LEN( TITLE ) ), 
     :                     STATUS )
         END IF

      END IF

*  Check whether chosen device is an 'image display' with a suitable
*  minimum number of colour indices, and obtain the number of colour 
*  indices.
      CALL KPG1_PQVID( 'DEVICE', 'IMAGE_DISPLAY,IMAGE_OVERLAY,'//
     :                 'WINDOW,MATRIX_PRINTER', ' ', MINCOL, UP, 
     :                 STATUS )

*  Define the lowest pen number for display of the image.  0 is
*  reserved for the background.  Others are reserved for annotations.
      LP = CTM__RSVPN

*  Obtain the colour for bad pixels as a colour index in the palette.
      CALL KPG1_PACOL( 'BADCOL', 0, CTM__RSVPN - 1, BPCI, STATUS )
      IF ( STATUS .EQ. PAR__ABORT ) GOTO 999

*  Obtain a lookup table and write it to the colour table.
*  =======================================================

*  Start a new error context.
      CALL ERR_MARK

*  Start an NDF context.
      CALL NDF_BEGIN
      
*  Obtain the NDF identifier and pointer of the input lookup table.
*  Validate the LUT.
      CALL KPG1_AVLUT( 'LUT', INDF4, LPNTR, LEL, STATUS )

*  Obtain the array dimensions.
      CALL NDF_DIM( INDF4, NDIM, LDIMS, NDIMS, STATUS )

*  Null status means do not read a new lookup table.  Instead, we will 
*  use the existing table.
      IF ( STATUS .EQ. PAR__NULL ) THEN

*  Annul the error.
         CALL ERR_ANNUL( STATUS )

*  If a new lookup table was supplied, load it instead of the users LUT.
      ELSE IF ( STATUS .EQ. SAI__OK ) THEN

*  Map the lookup table to the colour table by interpolation or by
*  nearest neighbour?
         CALL PAR_GTD0L( 'NN', .FALSE., .TRUE., NN, STATUS )

*  Install the lookup table into image-display colour table.
         CALL KPG1_PGLUT( LDIMS( 2 ), %VAL( CNF_PVAL( LPNTR( 1 ) ) ), 
     :                    LP, UP, NN,
     :                    STATUS )

      END IF

*  End the NDF context.
      CALL NDF_END( STATUS )

*  End error context.
      CALL ERR_RLSE
      IF ( STATUS .NE. SAI__OK ) GOTO 999

*  Produce an array of colour indices.
*  ===================================

*  See if the supplied NDF contains pre-scaled colour index values.
      CALL PAR_GET0L( 'SCALE', SCALE, STATUS )

*  If so, map the data as an _INTEGER array of colour indices.
      IF( .NOT. SCALE ) THEN
         CALL NDF_MAP( INDF2, MCOMP, '_INTEGER', 'READ', IPCOL, EL,
     :                 STATUS )
         NX = WDIM( 1 )
         NY = WDIM( 2 )
         DLO = LP + SLBND( 2 ) - 1
         DHI = LP + SUBND( 2 ) - 1

*  Otherwise, scale the supplied data to produce colour indices.
      ELSE 
         CALL KPS1_DISCL( INDF2, WDIM, MCOMP, LP, UP, BPCI, WPLBND, 
     :                    WPUBND, IPCOL, NX, NY, DLO, DHI, STATUS )

*  Store the scaling limits in the output parameters.
         CALL PAR_PUT0D( 'SCALOW', DLO, STATUS )
         CALL PAR_PUT0D( 'SCAHIGH', DHI, STATUS )

      END IF

*  Produce the display.
*  ====================

*  Abort if an error has occurred.
      IF( STATUS .NE. SAI__OK ) GO TO 999

*  Draw the image, extending over the pixel co-ordinate bounds of the
*  DATA picture.
      CALL KPG1_PGPIX( IPLOT, 'PIXEL', WPLBND, WPUBND, NX, NY, 
     :                 %VAL( CNF_PVAL( IPCOL ) ), STATUS )

*  Draw the axes grid if required.
      IF( AXES ) CALL KPG1_ASGRD( IPLOT, IPICF, .TRUE., STATUS )

*  If a border is required, ensure that the relavant Plot attributes are
*  cleared, set the new style, and draw it.
      IF( BORDER ) THEN
         CALL AST_CLEAR( IPLOT, 'COLOUR', STATUS )
         CALL AST_CLEAR( IPLOT, 'WIDTH', STATUS )
         CALL AST_CLEAR( IPLOT, 'STYLE', STATUS )
         CALL KPG1_ASSET( 'KAPPA_DISPLAY', 'BORSTYLE', IPLOT, STATUS )
         CALL KPG1_ASGRD( IPLOT, IPICF, .FALSE., STATUS )
      END IF

*  First create the key if required.
      IF( KEY ) THEN

*  Create a label, nidicating the array component and NDF name (without 
*  directory path to reduce the length of the label). 
         CALL KPG1_NDFNM( INDF1, NDFNAM, NC, STATUS )

         LABEL = ' '
         NC = 0
         CALL CHR_APPND( MCOMP, LABEL, NC )
         CALL CHR_APPND( ' value in', LABEL, NC )
         NC = NC + 1
         CALL CHR_APPND( NDFNAM, LABEL, NC )

*  Allocate a work array.
         CALL PSX_CALLOC( UP - LP + 1, '_INTEGER', IPWORK, STATUS )
         
*  Create the key.
         CALL KPG1_LUTKY( IPICK, 'KEYSTYLE', REAL( DHI ), REAL( DLO ), 
     :                    LABEL( : NC ), 'KAPPA_DISPLAY', LP, UP, 0.1,
     :                    ( Y2 - Y1 )*0.1, ( Y2 - Y1 )*0.1, 'CL',
     :                    NX*NY, %VAL( CNF_PVAL( IPCOL ) ), STATUS )

*  Report a context message if anything went wrong.
         IF( STATUS .NE. SAI__OK ) THEN
            CALL ERR_REP( 'DISPLAY_ERR6', 'Failed to display the key.',
     :                     STATUS )
         END IF

*  Deallocate the work array.
         CALL PSX_FREE( IPWORK, STATUS )

      END IF

*  Create the output NDF.
*  ======================

*  Only do this if the input image was not already scaled.
      IF ( SCALE ) THEN
      
*  Begin an NDF context.
         CALL NDF_BEGIN

*  Begin a new error context.
         CALL ERR_MARK

*  Create the output NDF structure based on the displayed section of the
*  input NDF.
         CALL LPG_PROP( INDF2, 'AXIS,NOQUALITY,NOHISTORY,NOVARIANCE,'//
     :                  'NOEXTENSION(),WCS', 'OUT', INDF3, STATUS )

         IF ( STATUS .EQ. PAR__NULL ) THEN
            CALL ERR_ANNUL( STATUS )

*  Check the image size has not been changed by the conversion to colour
*  indices.  If this is the case, then the array of scaled values does
*  not correspond pixel-for-pixel to the input NDF and so cannot be
*  saved.  Report an error and delete the output NDF.
         ELSE IF( NX .NE. WDIM( 1 ) .OR. NY .NE. WDIM( 2 ) ) THEN
            CALL MSG_OUT( 'DISPLAY_NOOUT', 'Cannot produce an output '//
     :                    'NDF holding colour indices because the '//
     :                    'image has been reduced in size to fit the '//
     :                    'display.', STATUS )
            CALL NDF_DELET( INDF3, STATUS )

*  Otherwise...
         ELSE

*  Define the output data type.  Note bad-pixel flag should be
*  switched off once KAPPA is fully converted to the NDF.
            IF ( UP - LP + 1 .LE. NUM__MAXUB ) THEN
               OTYPE = '_UBYTE'
            ELSE IF ( UP - LP + 1 .LE. NUM__MAXUW ) THEN
               OTYPE = '_UWORD'
            ELSE
               OTYPE = '_INTEGER'
            END IF

*  Set the data type of the data component to be one of the above
*  rather  than the type of the input NDF.
            CALL NDF_STYPE( OTYPE, INDF3, 'Data', STATUS )

*  Map the NDF's data component for WRITE access.
            CALL NDF_MAP( INDF3, 'Data', OTYPE, 'WRITE', OPNTR, EL,
     :                    STATUS )

*  There are no bad values by definition.
            BAD = .FALSE.

*  Output the bad pixel flag value accordingly unless the output NDF is 
*  primitive.
            CALL NDF_FORM( INDF3, 'Data', FORM, STATUS )
            IF ( FORM .NE. 'PRIMITIVE' ) THEN
               CALL NDF_SBAD( BAD, INDF3, 'Data', STATUS )
            END IF

*  Move the contents from the scaled array to the NDF data component. 
*  Since  there can be no conversion errors by definition, the count
*  returned by the conversion routine is ignored.
            IF ( OTYPE .EQ. '_UBYTE' ) THEN
               CALL VEC_ITOUB( BAD, EL, %VAL( CNF_PVAL( IPCOL ) ),
     :                         %VAL( CNF_PVAL( OPNTR( 1 ) ) ), 
     :                         IERR, NERR, STATUS )

            ELSE IF ( OTYPE .EQ. '_UWORD' ) THEN
               CALL VEC_ITOUW( BAD, EL, %VAL( CNF_PVAL( IPCOL ) ),
     :                         %VAL( CNF_PVAL( OPNTR( 1 ) ) ), 
     :                         IERR, NERR, STATUS )

            ELSE IF ( OTYPE .EQ. '_INTEGER' ) THEN
               CALL VEC_ITOI( BAD, EL, %VAL( CNF_PVAL( IPCOL ) ),
     :                        %VAL( CNF_PVAL( OPNTR( 1 ) ) ), 
     :                        IERR, NERR, STATUS )
            END IF

         END IF

*  End the error context.
         CALL ERR_RLSE

*  End the NDF context.
         CALL NDF_END( STATUS )

*  Free the workspace used for scaling.
         CALL PSX_FREE( IPCOL, STATUS )

      END IF


*  Shutdown procedure.
*  ===================

  999  CONTINUE

*  Shutdown PGPLOT and the graphics database.
      CALL KPG1_PGCLS( 'DEVICE', .FALSE., STATUS )

*  End the NDF context.
      CALL NDF_END( STATUS )

*  End the AST context.
      CALL AST_END( STATUS )

*  Add a context report if anything went wrong.
      IF( STATUS .NE. SAI__OK ) THEN
         CALL ERR_REP( 'DISPLAY_ERR', 'DISPLAY: Failed to display an '//
     :                 'image of a one- or two-dimensional data set.', 
     :                 STATUS )
      END IF

      END


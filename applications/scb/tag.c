/*
*+
*  Name:
*     tag.c
*
*  Type of module:
*     C source code.
*
*  Purpose:
*     Common routines used by lex and yacc files for source code tagging.
*
*  Authors:
*     MBT: Mark Taylor (STARLINK)
*
*  History:
*     25-NOV-1999 (MBT):
*        Intial version.
*
*-
*/


#include <stdio.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

extern int yy_flex_debug;
extern int yydebug;
int strict;

   main( int argc, char **argv ) {
/*
*+
*  Name:
*     main
*
*  Purpose:
*     Harness routine for calling the yacc-generated parser.
*
*  Flags:
*     -s
*        If present, this enforces strict parsing; any parse errors lead
*        to program termination.  By default, parse errors are silently
*        tolerated.
*     -d
*        Debugging.  It may be followed immediately by the letters 'l',
*        'y' or both.  This turns on the debugging messages in the lex
*        part and/or the yacc part of the processor respectively.
*
*  Authors:
*     MBT: Mark Taylor (STARLINK)
*
*  History:
*     25-NOV-1999 (MBT):
*        Original version.
*-
*/

/* Declare external function. */
      int yyparse();

/* Declare local variables. */
      char *name, *usagef;
      char c;

/* Get name of program etc. */
      name = *(argv++);
      argc--;
      usagef = "Usage: %s [-d[l][y]] [-s] [ in [ out ] ]\n";

/* Work through any command line flags. */
      yy_flex_debug = 0;
      yydebug = 0;
      strict = 0;
      while ( argc > 0 && **argv == '-' ) {
         switch( *(++(*argv)) ) {
            case 'd':
               while ( c = *(++(*argv)) )
                  switch( c ) {
                     case 'l':
                        yy_flex_debug = 1;
                        break;
                     case 'y':
                        yydebug = 1;
                        break;
                  };
               break;
            case 's':
               strict = 1;
               break;
            default:
               printf( usagef, name );
               exit( 1 );
         }
         argv++;
         argc--;
      }
   
/* Open standard input and output appropriately according to command line
   arguments. */
      switch( argc ) {
         case 2:
            if ( freopen( argv[ 1 ], "w", stdout ) == NULL ) {
               perror( argv[ 1 ] );
               exit( 1 );
            }
         case 1:
            if ( freopen( argv[ 0 ], "r", stdin ) == NULL ) {
               perror( argv[ 0 ] );
               exit( 1 );
            }
         case 0:
            break;
         default:
            printf( usagef, name );
            exit( 1 );
      }
         
/* Call the parser and return. */
      return( yyparse() );
   }


   char *snew( char *str ) {
/*+
*  Name:
*     snew
*
*  Invocation:
*     string = snew( str );
*
*  Purpose:
*     Copy a string into malloc'd space.
*
*  Arguments:
*     str = char *
*        The string to be copied (must end with '\0').
*
*  Return Value:
*     string = char *
*        A string with the same contents as str, but in a newly malloc'd
*        location.
*
*  Description:
*     This routine just mallocs some space and copies the given string
*     into it.  The purpose of this is so that the resulting pointer
*     can be passed to routines which assume their arguments have been
*     malloc'd and may be free'd.
*-
*/
      char *string;

      string = malloc( strlen( str ) + 1 );
      strcpy( string, str );
      return( string );
   }


   char *scat( int n, ... ) {
/*+
*  Name:
*     scat
*
*  Invocation:
*     string = scat( n, ... )
*
*  Purpose:
*     Concatenate a list of strings.
*
*  Arguments:
*     n = int
*        The number of strings to be concatenated.
*     sp1, sp2, ... = char *
*        The other arguments are all strings, and there are n of them.
*        free() is called on each of them, so they must have been malloc'd
*        at some time in the past, and must not be used subsequent to
*        passing to this function.
*
*  Return value:
*     string = char *
*        The return value is a string containing the concatenation of
*        all the strings supplied.  It is obtained using malloc, so
*        should be free'd at some time in the future.
*
*  Description:
*     This routine returns a newly malloc'd string which is the concatenation
*     of all the strings supplied to it as arguments.  Each of those arguments
*     gets free'd by this routine, so they must have been malloc'd (probably
*     by this routine) in the past, and must not be referred to again after
*     calling this routine.
*-
*/

/* Local variables. */
      va_list ap;
      int len, i;
      char *string, *sp;

/* Work out the length of the final string. */
      len = 1;
      va_start( ap, n );
      for ( i = 0; i < n; i++ ) {
         sp = va_arg( ap, char * );
         len += strlen( sp );
      }
      va_end( ap );

/* Allocate the memory we will need, and initialise it. */
      string = malloc( len + 1 );
      *string = '\0';

/* Copy the arguments into the allocated space, calling free() on each
 * one as we go along. */
      va_start( ap, n );
      for ( i = 0; i < n; i++ ) {
         sp = va_arg( ap, char * );
         strcat( string, sp );
         free( sp );
      }
      va_end( ap );

/* Return. */
      return( string );
   }


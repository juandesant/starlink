#define astCLASS
#include "axis.h"
#include "channel.h"
#include "cmpframe.h"
#include "cmpmap.h"
#include "dssmap.h"
#include "fitschan.h"
#include "frame.h"
#include "frameset.h"
#include "intramap.h"
#include "loader.h"
#include "lutmap.h"
#include "mapping.h"
#include "mathmap.h"
#include "matrixmap.h"
#include "object.h"
#include "pcdmap.h"
#include "permmap.h"
#include "plot.h"
#include "pointset.h"
#include "skyaxis.h"
#include "skyframe.h"
#include "specframe.h"
#include "specmap.h"
#include "slamap.h"
#include "sphmap.h"
#include "unitmap.h"
#include "wcsmap.h"
#include "winmap.h"
#include "zoommap.h"

#include "error.h"
#include "ast_err.h"
#include <stddef.h>
#include <string.h>

/*
*+
*  Copyright:
*     <COPYRIGHT_STATEMENT>

*  Authors:
*     RFWS: R.F. Warren-Smith (Starlink)
*     DSB: David S. Berry (Starlink)

*  History:
*     18-NOV-1997 (RFWS):
*        Original version.
*     18-MAR-1998 (RFWS):
*        Added the IntraMap class.
*     3-JUN-1999 (RFWS):
*        Added the PcdMap class.
*     17-AUG-1999 (RFWS):
*        Added the MathMap class.
*     8-JAN-2003 (DSB):
*        Added the SpecMap and SpecFrame classes.
*-
*/

AstLoaderType *astGetLoader( const char *class ) {
   if ( !astOK ) return NULL;

#define LOAD(name) \
if ( !strcmp( class, #name ) ) return (AstLoaderType *) astLoad##name##_

   LOAD(Axis);
   LOAD(Channel);
   LOAD(CmpFrame);
   LOAD(CmpMap);
   LOAD(DssMap);
   LOAD(FitsChan);
   LOAD(Frame);
   LOAD(FrameSet);
   LOAD(IntraMap);
   LOAD(LutMap);
   LOAD(Mapping);
   LOAD(MathMap);
   LOAD(MatrixMap);
   LOAD(Object);
   LOAD(PcdMap);
   LOAD(PermMap);
   LOAD(Plot);
   LOAD(PointSet);
   LOAD(SkyAxis);
   LOAD(SkyFrame);
   LOAD(SlaMap);
   LOAD(SpecFrame);
   LOAD(SpecMap);
   LOAD(SphMap);
   LOAD(UnitMap);
   LOAD(WcsMap);
   LOAD(WinMap);
   LOAD(ZoomMap);

   astError( AST__OCLUK, "astGetLoader: Object of unknown class \"%s\" cannot "
                         "be loaded.", class );
   return NULL;
#undef LOAD
}

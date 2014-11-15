#!python

"""
Given the HDS include file, generate a wrapper that calls
the v4 or v5 APIs depending on the type of the input
locator.

Reads through hds.h and for every API definition, generates
a wrapper routine.

Notes:

- datCcopy is only routine that takes two locators and returns a third.
- Routines taking two input locators:
    datCopy, datMove

All other routines take one locator and may or may not return a
locator. That locator will be of the correct type.

The copy/move routines will need special code to be able to handle
locators from different implementations.

Routines that don't take a locator at all:
   datCctyp - call v5
   datChscn - call v5
   datErmsg - Use wrapper implementation that uses largest error table (default to v5)
   hdsEwild - Wrapper implementation
   hdsFlush - Call both (only error if both fail)
   hdsGtune - call both (only error if both fail or both different)
   hdsShow  - call one or both depending on whether we have used any v5 or v4 locators.
   hdsState - call both (error if different)
   hdsStop - call both
   hdsInfoI - calls both and adds the result
   hdsTune  - call both routines

Routines that open a file:

   hdsOpen - Try v5 and if that fails try v4
   hdsWild - presumably will use hdsOpen internally.
             This routine has to be implemented in the wrapper
             so that the wrapped hdsOpen can be called.

Creating a file:

   hdsNew - always creates v5 unless environment variable
            indicates to only open v4.
   datTemp - uses same logic as for hdsNew

Ideally v5 files would have a different file ending to v4
but that will require lots of code changes in other packages
that are assuming just one file extension.

Will not be run repeatedly as there will eventually be special
code for these notable routines. File is retained for historical
interest.

"""

import re

# Pattern match to find a function
hfunc_re = re.compile(r"^((dat|hds)[A-Z][A-Za-z0-9]+)\(")

def version_names(line):
    v4 = line.replace("(", "_v4(")
    v5 = line.replace("(", "_v5(")
    return (v4,v5)

# Code for the different type of functions
def func_simple(func,line):
    (v4,v5) = version_names(line)
    # are we dealing with locator or locator1?
    locvar = "locator"
    if line.find("locator1") >= 0:
        locvar = "locator1"
    elif line.startswith("datAnnul") or line.startswith("datPrmry") or line.startswith("hdsErase") or line.startswith("hdsClose"):
        locvar = "*locator"
    print("""
  int retval = 0;
  int instat = *status;
  int isv5 = ISHDSv5({0});
  EnterCheck(\"{3}\",*status);
  if (isv5) {{
    retval = {1}
  }} else {{
    retval = {2}
  }}
  HDS_CHECK_STATUS(\"{3}\",(isv5 ? "(v5)" : "(v4)"));
  return retval;""".format(locvar, v5, v4, func))


def func_special(func,line):
    print("  /* Requires special code */")
    print('  printf("Aborting. Special code required in: %s\\n", "{0}");'.format(line))
    print("  abort();")
    if line.find("status") > -1:
        print("  return *status;")

def func_both(func,line):
    (v4,v5) = version_names(line)
    print("""  int retval = 0;
  int instat = *status;
  EnterCheck(\"{2}\",*status);
  if (*status != SAI__OK) return *status;
  retval = {0}
  retval = {1}
  HDS_CHECK_STATUS(\"{2}\", "(both)");
  return retval;""".format(v5,v4,func))

def func_void(func, line):
    (v4,v5) = version_names(line)
    print("""  EnterCheck(\"{3}\",-1);
  if (ISHDSv5({0})) {{
    {1}
  }} else {{
    {2}
  }}
  return;""".format("locator", v5, v4,func))

def func_v5void(func,line):
    (v4,v5) = version_names(line)
    print('  EnterCheck("'+func+'",-1);')
    print("  "+v5)
    print("  return;")

def func_v5(func,line):
    (v4,v5) = version_names(line)
    if line.find("status)") > -1:
        print("""  int retval = 0;
  int instat = *status;
  EnterCheck(\"{1}\",*status);
  if (*status != SAI__OK) return *status;
  retval = {0}
  HDS_CHECK_STATUS(\"{1}\","(v5)");
  return retval;""".format(v5,func))
    else:
        print('  EnterCheck("'+func+'",-1);')
        print("  return " +v5)

def func_copy(func,line):
    (v4,v5) = version_names(line)
    vXtoY = line.replace("(", "XtoY(")
    vXtoY = vXtoY.replace("datC","dat1C")
    loc1 = "locator1"
    if line.startswith("datMove"):
        loc1 = "*locator1"
    print("""  /* Requires special code */
  int instat = *status;
  int isv5 = 0;
  int loc1isv5 = 0;
  int loc2isv5 = 0;
  EnterCheck(\"{3}\",*status);
  if (*status != SAI__OK) return *status;
  loc1isv5 = ISHDSv5({0});
  loc2isv5 = ISHDSv5(locator2);
  if (loc1isv5 && loc2isv5) {{
    /* Just call the v5 code */
    isv5 = 1;
    {1}
  }} else if ( !loc1isv5 && !loc2isv5 ) {{
    isv5 = 0;
    {2}
  }} else {{
    /* Manual copy of X to Y */
    if (loc1isv5) {{
      isv5 = -1;
    }} else {{
      isv5 = -2;
    }}
    {4}
  }}
  {{
    const char *helptxt = "(unexpected)";
    if (isv5 == 1) {{
      helptxt = "(v5)";
    }} else if (isv5 == 0) {{
      helptxt = "(v4)";
    }} else if (isv5 == -1) {{
      helptxt = "(v5->v4)";
    }} else if (isv5 == -2) {{
      helptxt = "(v4->v5)";
    }}
    HDS_CHECK_STATUS(\"{3}\",helptxt);
  }}
  return *status;""".format(loc1,v5,v4,func,vXtoY))

def func_datMove(func,line):
    (v4,v5) = version_names(line)
    print("""  /* Requires special code */
  int instat = *status;
  int isv5 = 0;
  int loc1isv5 = 0;
  int loc2isv5 = 0;
  EnterCheck(\"{2}\",*status);
  if (*status != SAI__OK) return *status;
  loc1isv5 = ISHDSv5(*locator1);
  loc2isv5 = ISHDSv5(locator2);
  if (loc1isv5 && loc2isv5) {{
    /* Just call the v5 code */
    isv5 = 1;
    {0}
  }} else if ( !loc1isv5 && !loc2isv5 ) {{
    isv5 = 0;
    {1}
  }} else {{
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
  }}
  HDS_CHECK_STATUS(\"{2}\",(isv5 ? "(v5)" : "(v4)"));
  return *status;""".format(v5,v4,func))

def func_hdsOpen(line):
    print("""  int instat = *status;
  EnterCheck(\"hdsOpen\",*status);
  if (*status != SAI__OK) return *status;
  /* HDSv4 can reliably spot when a file is not v4
     format so for now we open in v4 and catch that specific error */
  hdsOpen_v4(file_str, mode_str, locator, status);
  if (*status == DAT__INCHK || *status == DAT__FILIN) {
    emsAnnul(status);
    hdsOpen_v5(file_str, mode_str, locator, status);
  }
  HDS_CHECK_STATUS( "hdsOpen", file_str);
  return *status;""")

def func_hdsGtune(line):
    print("""  int instat = *status;
  EnterCheck(\"hdsGtune\",*status);
  if (*status != SAI__OK) return *status;
  hdsGtune_v5(param_str, value, status);
  hdsGtune_v4(param_str, value, status);
  if (*status != SAI__OK) {
    emsRepf("hdsGtune_wrap", "hdsGtune: Error obtaining value of tuning parameter '%s'",
            status, param_str);
  }
  HDS_CHECK_STATUS("hdsGtune", "(both)");
  return *status;""")

def func_hdsInfoI(line):
    print("""
  int retval = 0;
  int instat = *status;
  *result = 0;
  const char * used = "(both)";
  EnterCheck("hdsInfoI",*status);
  if (*status != SAI__OK) return *status;
  /* Call both versions and sum the result if we have a NULL locator */
  if (!locator) {
    int res_v4 = 0;
    int res_v5 = 0;
    hdsInfoI_v4(locator, topic_str, extra, &res_v4, status);
    hdsInfoI_v5(locator, topic_str, extra, &res_v5, status);
    retval = *status;
    *result = res_v4 + res_v5;
  } else if (ISHDSv5(locator)) {
    retval = hdsInfoI_v5(locator, topic_str, extra, result, status);
    used = "(v5)";
  } else {
    used = "(v4)";
    retval = hdsInfoI_v4(locator, topic_str, extra, result, status);
  }
  HDS_CHECK_STATUS("hdsInfoI", used);
  return retval;""")

def func_hdsFlush(line):
    print("""  /* We are only allowed to flush a group that actually exists */
  int instat = *status;
  EnterCheck(\"hdsFlush\",*status);
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
  return *status;""")

def func_hdsCopy(line):
    print("""  int instat = *status;
  int ndim = 0;
  hdsdim dims[DAT__MXDIM];
  char type_str[DAT__SZTYP+1];
  HDSLoc * outloc = NULL;
  hdsbool_t struc = 0;
  EnterCheck(\"hdsCopy\",*status);
  if (*status != SAI__OK) return *status;
  /* We always want to end up with output files that match
     the format currently in use for hdsNew (which may depend
     on an environment variable). We can not simply call hdsCopy_v5.
     so we have to do some manual leg work. Would be a bit easier if
     we had a function in this file that returned the default output
     format version so we could call the native version.
   */
  datType( locator, type_str, status );
  datShape( locator, DAT__MXDIM, dims, &ndim, status );
  /* Unfortunately this locator is one level down */
  hdsNew(file_str, name_str, type_str, ndim, dims, &outloc, status );
  /* So we need to walk through and can not simply use datCopy
    - we can use two routines used by dat1CopyXtoY though. */
  datStruc(locator, &struc, status);
  if (struc) {
    dat1CopyStrucXtoY( locator, outloc, status );
  } else {
    dat1CopyPrimXtoY( locator, outloc, status );
  }
  datAnnul(&outloc, status);
  HDS_CHECK_STATUS("hdsCopy", (ISHDSv5(locator) ? "(v5)" : "(v4)"));
  return *status;""")

# Dictionary indicating special cases
special = dict({
    "datCcopy": "copy",
    "datCctyp": "v5+void",
    "datChscn": "v5",
    "datCopy": "copy",
    "datErmsg": "v5",
    "datMove": "datMove",
    "datMsg": "void",
    "datTemp": "v5",
    "hdsCopy": "hdsCopy",
    "hdsEwild": "special",
    "hdsFlush": "hdsFlush",
    "hdsGtune": "hdsGtune",
    "hdsInfoI": "hdsInfoI",
    "hdsNew":  "v5",
    "hdsOpen": "hdsOpen",
    "hdsShow": "both",
    "hdsState": "both",
    "hdsStop": "both",
    "hdsTune": "both",
    "hdsWild": "special"
})

in_prologue = 1
for line in open("hds.h"):
    line = line.strip()
    if line.startswith("int hds"):
        # This is a function that should be ignored
        continue
    func_match = hfunc_re.search(line)
    if func_match:
        hds_function = func_match.group(1)
        print( line[:-1] + " {")  # Without the semi-colon
        # Now we have to convert the prototype to a function call
        # ie datXxx( type1 var1, type2 var2); to datXxx(var1,var2);
        openparen = line.find("(")
        closeparen = line.find(")")
        argsin = line[openparen:closeparen].split(",")
        argsout = []
        for a in argsin:
            # Get rid of array [] specifiers
            arraypos = a.find("[")
            if arraypos > -1:
                a = a[:arraypos]
            parts = a.split()
            varname = parts[-1]
            # Remember to drop pointer derefs
            argsout.append( varname.replace("*", "") )
        # put the line back together
        line = hds_function + "(" + ", ".join(argsout) + ");"
        if hds_function in special:
            mode = special[hds_function]
            if mode == "both":
                func_both(hds_function,line)
            elif mode == "special":
                func_special(hds_function,line)
            elif mode == "void":
                func_void(hds_function,line)
            elif mode == "v5+void":
                func_v5void(hds_function,line)
            elif mode == "v5":
                func_v5(hds_function,line)
            elif mode == "datMove":
                func_datMove(hds_function,line)
            elif mode == "hdsOpen":
                func_hdsOpen(line)
            elif mode == "hdsGtune":
                func_hdsGtune(line)
            elif mode == "hdsFlush":
                func_hdsFlush(line)
            elif mode == "hdsCopy":
                func_hdsCopy(line)
            elif mode == "hdsInfoI":
                func_hdsInfoI(line)
            elif mode == "copy":
                func_copy(hds_function,line)
            else:
                raise ValueError("Unrecognized mode {0} for function {1}".format(mode,hds_function))
        else:
            func_simple(hds_function,line)
        print("}")
    else:
        if in_prologue and line.startswith("/*=="):
            print('#include <stdlib.h>')  # For abort()
            print('#include <stdio.h>')  # For printf()
            print('#include "sae_par.h"')
            print('#include "dat_par.h"')
            print('#include "dat1.h"')
            print('#include "hds_types.h"')
            print('#include "ems.h"')
            print('#include "hds.h"')
            print('#include "dat_err.h"')
            print('#include "star/hds_v4.h"')
            print('#include "star/hds_v5.h"')
            print('#define ISHDSv5(loc) ((loc) && (loc)->hds_version >= 5)')
            print('#if DEBUG_HDS')
            print('#define HDS_CHECK_STATUS(func,txt) if (*status != instat && *status != SAI__OK) { emsRepf("wrap_" func, func ": Error in call to HDS %s", status, txt); printf("Bad status from %s %s: %d\\n", func, txt, *status);}')
            print('static void EnterCheck( const char * func, int status ) { printf("Enter HDS routine: %s [%d]\\n", func,status); }')
            print("#else")
            print('#  define HDS_CHECK_STATUS(func,txt) if (*status != instat && *status != SAI__OK) { emsRepf("wrap_" func, func ": Error in call to HDS %s", status, txt);}')
            print("#  define EnterCheck(A,B) ;")
            print('#endif')
            print("")
            print(line)
            in_prologue = 0
        elif in_prologue:
            # We want to ignore the prologue and write our own
            pass
        elif line.startswith("/* STAR_HDS_H"):
            # this is the end of the include file
            in_prologue = 1
        else:
            print(line)


## Process this file with automake to produce Makefile.in

dist_bin_SCRIPTS = aryk_link aryk_link_adam
cincludedir = $(includedir)/star
dist_noinst_SCRIPTS = make_h

lib_LTLIBRARIES = libaryk.la

libaryk_la_SOURCES = \
	$(PUBLIC_INCLUDES) \
	$(include_MESSAGES) \
	$(C_ROUTINES) \
	$(PRIVATE_INCLUDES)

# Make all library code position independent. This is handy for creating
# shareable libraries from the static ones (Java JNI libraries).
if !NOPIC
libaryk_la_CFLAGS = $(AM_CFLAGS) -prefer-pic
endif

include_HEADERS = $(PUBLIC_INCLUDES)

stardocs_DATA = @STAR_LATEX_DOCUMENTATION@

PRIVATE_INCLUDES = aryk1.h aryk1_cgen.h aryk1_types.h aryk_dlt.h

PUBLIC_F_INCLUDES = ARY_PAR ARYK_PAR ARY_ERR
PUBLIC_C_INCLUDES = aryk.h aryk_types.h
PUBLIC_INCLUDES = $(PUBLIC_F_INCLUDES) $(PUBLIC_C_INCLUDES)
include_MESSAGES = aryk_err.h ARYK_ERR

C_ROUTINES = $(BUILT_C_ROUTINES) aryk1Accok.c aryk1Anl.c aryk1Annpl.c \
aryk1Antmp.c aryk1Bad.c aryk1Bpp.c aryk1Ccpy.c aryk1Chacc.c aryk1Chbpp.c \
aryk1Chcma.c aryk1Chmod.c aryk1Chscn.c aryk1Cln.c aryk1Cmtmp.c \
aryk1Cpdlt.c aryk1Cpscl.c aryk1Cpy.c aryk1Cpync.c aryk1Crnba.c aryk1Cut.c \
aryk1DCBLock.c aryk1Danl.c aryk1Dbad.c aryk1Dbnd.c aryk1Dcpy.f aryk1Dcre.c \
aryk1Dcrep.c aryk1Defr.c aryk1Del.c aryk1Dfppl.c aryk1Dfrm.c aryk1Dimp.c \
aryk1Dlshp.c aryk1Dmod.c aryk1Dobj.c aryk1Dp2s.c aryk1Drst.c aryk1Dsbd.c \
aryk1Dsbnd.c aryk1Dscl.c aryk1Dsft.c aryk1Dsta.c aryk1Dstp.c aryk1Dtyp.c \
aryk1Dvfy.c aryk1Expid.c aryk1Extyp.c aryk1Ffs.c aryk1Get0C.c aryk1Gmrb.c \
aryk1Gtdlt.c aryk1Gtn.c aryk1Hunmp.c aryk1Id2ac.c aryk1Imp.c aryk1Impid.c \
aryk1Inbnd.c aryk1Intyp.c aryk1Iobw.c aryk1IsValid.c aryk1Maps.c \
aryk1Mpsr.c aryk1Mpsw.c aryk1Nel.c aryk1Nxtsl.c aryk1Pbnd.c aryk1Ptn.c \
aryk1Rebnd.c aryk1Retyp.c aryk1Rls.c aryk1S2dlt.c aryk1Sbd.c aryk1Sbnd.c \
aryk1Sft.c aryk1Stp.c aryk1Tcnam.c aryk1Temp.c aryk1Trace.c aryk1Ump.c \
aryk1Umps.c aryk1Undlt.c aryk1Upsr.c aryk1Upsw.c aryk1Vbad.c aryk1Vbnd.c \
aryk1Vftp.c aryk1Vmmd.c aryk1Vscl.c aryk1Vtyp.c aryk1Vzero.c aryk1Xsbnd.c \
arykAnnul.c arykBad.c arykBase.c arykBound.c arykClone.c arykCmplx.c \
arykCopy.c arykDelet.c arykDelta.c arykDim.c arykDupe.c arykFind.c \
arykForm.c arykFtype.c arykGtdlt.c arykImprt.c arykIsacc.c arykIsbas.c \
arykIsmap.c arykIstmp.c arykLoc.c arykLock.c arykLocked.c arykMap.c \
arykMapz.c arykMsg.c arykNdim.c arykNew.c arykNewp.c arykNoacc.c arykOffs.c \
arykPlace.c arykReset.c arykSame.c arykSbad.c arykSbnd.c arykSctyp.c \
arykSect.c arykShift.c arykSize.c arykSsect.c arykState.c arykStype.c \
arykTemp.c arykTrace.c arykType.c arykUnlock.c arykUnmap.c arykValid.c \
arykVerfy.c fortran_interface.c

CGENERIC_ROUTINES = aryk1Badx.cgen aryk1Zerx.cgen aryk1Ptnx.cgen \
aryk1Pt1x.cgen aryk1Cvfx.cgen aryk1Gtnx.cgen aryk1Gt1x.cgen aryk1Cvtx.cgen \
aryk1Bppx.cgen arykGtszx.cgen arykPtszx.cgen

# The .c files which are built from the above .cgen files.
BUILT_C_ROUTINES = $(CGENERIC_ROUTINES:.cgen=.c)

BUILT_SOURCES = $(include_MESSAGES) $(BUILT_C_ROUTINES) aryk1_cgen.h

aryk1_cgen.h: aryk1_cgen.h.source make_h
	./make_h

aryk_cgen.h: aryk_cgen.h.source make_h
	./make_h

ARY_PAR: ARYK_PAR
	sed -e 's/ARYK/ARY/g' ARYK_PAR | sed -e 's/64 bit/32 bit/' > ARY_PAR

ARY_ERR: ARYK_ERR
	sed -e 's/ARYK/ARY/g' ARYK_ERR | sed -e 's/64 bit/32 bit/' > ARY_ERR

# Testing

TESTS = arykTest aryk_test ary_test
check_PROGRAMS = arykTest aryk_test ary_test
arykTest_SOURCES = arykTest.c
aryk_test_SOURCES = aryk_test.f
ary_test_SOURCES = ary_test.f

# LDADD is the expansion of aryk_link, with -laryk replaced by libaryk.la
arykTest_LDADD = libaryk.la `ast_link` `hds_link` `err_link` `chr_link` `prm_link`
aryk_test_LDADD = libaryk.la `ast_link` `hds_link` `err_link` `chr_link` `prm_link`
ary_test_LDADD = libaryk.la `ast_link` `hds_link` `err_link` `chr_link` `prm_link`

dist_starnews_DATA = aryk.news
dist_pkgdata_DATA = LICENCE

CLEANFILES = ARYK_ERR

# Create the .c file which defines type-specific code generated from each
# .cgen file.
$(CGENERIC_ROUTINES:.cgen=.c):
	echo "/*" > $@
	echo "*  Name:" >> $@
	echo "*    $@" >> $@
	echo "" >> $@
	echo "*  Purpose:" >> $@
	echo "*    This file expands the generic C code held in $(@:.c=.cgen) to provide" >> $@
	echo "*    the required type-specific implementations which can be called by" >> $@
	echo "*    other functions." >> $@
	echo "" >> $@
	echo "*  Notes:" >> $@
	echo "*    - This file is generated automatically at build time (see" >> $@
	echo "*    Makefile.am)" >> $@
	echo "*/" >> $@
	echo "" >> $@
	echo "#include \"prm_par.h\"" >> $@
	echo "#include \"cgeneric.h\"" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_DOUBLE_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_FLOAT_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_INT_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_WORD_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_UWORD_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_BYTE_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_UBYTE_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@
	echo "#define CGEN_CODE_TYPE CGEN_INT64_TYPE" >> $@
	echo "#include \"cgeneric_defs.h\"" >> $@
	echo "#include \"$(@:.c=.cgen)\"" >> $@
	echo "#undef CGEN_CODE_TYPE" >> $@
	echo "" >> $@

# Need to distribute the generic files
EXTRA_DIST = $(CGENERIC_ROUTINES) aryk_err.msg aryk1_cgen.h


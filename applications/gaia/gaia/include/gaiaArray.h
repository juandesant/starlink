#ifndef _GAIAARRAY_INCLUDED_
#define _GAIAARRAY_INCLUDED_
 
/*
 *  External prototypes and definitions for gaiaArray.c.
 */

/* Enumeration for the support data types, these are supposed to match the HDS
   ones. */
enum { HDS_UNKNOWN = -1, 
       HDS_UBYTE, HDS_BYTE, HDS_UWORD, HDS_WORD, HDS_INTEGER, HDS_REAL,
       HDS_DOUBLE };

/**
 * An array structure. One of these stores various useful information about an
 * array, such as its memory address, number of elements, data type and
 * some information about FITS derived data. FITS derived data may not be in
 * the native byte ordering and may have a bad value that will differ from HDS.
 */
struct ARRAYinfo {
    void *ptr;           /* Pointer to array data */
    int type;            /* The local type, HDS_xxx */
    long el;             /* The number of type elements available */
    int isfits;          /* If FITS data */
    int haveblank;       /* Have a FITS blank value */
    int blank;           /* The FITS blank value, if applicable */
};
typedef struct ARRAYinfo ARRAYinfo;


#ifdef __cplusplus
extern "C" {
#endif

    /* Create and initialise an ARRAYinfo structure */
    ARRAYinfo *gaiaArrayCreateInfo( void *ptr, int type, long el, int isfits,
                                    int haveblank, int blank );
    
    /* Free an ARRAYinfo structure. */
    void gaiaArrayFreeInfo( ARRAYinfo *info );

    /* Convert an HDS type into one of the enums */
    int gaiaArrayHDSType( char *typePtr );

    /* Convert FITS bitpix into one of the enums */
    int gaiaArrayFITSType( int bitpix );

    /* Convert local type into HDS type string */
    char const *gaiaArrayTypeToHDS( int type );

    /* Convert an array of typed data into simple double precision
       representation. */
    void gaiaArrayToDouble( ARRAYinfo *info, double badValue, double *outPtr );

    /* Get a simple image section from a cube */
    void gaiaArrayImageFromCube( ARRAYinfo *cubeinfo, int dims[3], int axis,
                                 int index, ARRAYinfo **imageinfo, 
                                 int cnfmalloc );
        
    /* Get a spectrum (line of data) from a cube */
    void gaiaArraySpectrumFromCube( ARRAYinfo *info, int dims[3], int axis, 
                                    int arange[2], int index1, int index2, 
                                    int cnfmalloc, void **outPtr, int *nel );

    /* Get strides for indexing an ND array */ 
    void gaiaArrayGetStrides( int ndims, int dims[], int strides[] );

    /* Free simple memory we've allocated */
    void gaiaArrayFree( ARRAYinfo *info, int cnfmalloc );

    /* Normalise an array from FITS to HDS-like format */
    void gaiaArrayNormalise( ARRAYinfo *info );

#ifdef __cplusplus
}
#endif

#endif

/*********************************************************************
 *   Copyright 1993, UCAR/Unidata
 *   See netcdf/COPYRIGHT file for copying and redistribution conditions.
 *********************************************************************/
#include "config.h"
#include "dapincludes.h"

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

struct Value {
    long long llval;
    double dval;
};

/*Forward*/
static int cvtnumconst(const char* s, struct Value* val);
static int rangecheck(nc_type srctype, nc_type dsttype, struct Value* val);

NCerror
dapconvert(nc_type srctype, nc_type dsttype, char* memory0, char* value0, size_t count)
{
    NCerror ncstat = NC_NOERR;
    size_t i;
    char* memory = memory0;
    char* value = value0;

    /* In order to deal with the DAP upgrade problem,
	try to preserve the bit patterns
    */

    /* Provide space and pointer casts for intermediate results */
    signed char ncbyte;
    signed char* ncbytep;
    char ncchar;
    char* nccharp;
    short ncshort;
    short* ncshortp;
    int ncint;
    int* ncintp;
    float ncfloat;
    float* ncfloatp;
    double ncdouble;
    double* ncdoublep;
    unsigned char ncubyte;
    unsigned char* ncubytep;
    unsigned short ncushort;
    unsigned short* ncushortp;
    unsigned int ncuint;
    unsigned int* ncuintp;
    long long ncint64;
    long long* ncint64p;
    unsigned long long ncuint64;
    unsigned long long* ncuint64p;


#define CASE(nc1,nc2) (nc1*256+nc2)
#define CUT8(e) ((unsigned char)((e) & 0xff))
#define CUT16(e) ((unsigned short)((e) & 0xffff))
#define CUT32(e) ((unsigned int)((e) & 0xffffffff))
#define ARM(vs,ncs,ts,vd,ncd,td) \
case CASE(ncs,ncd):\
    vs##p = (ts *)value;\
    vs = *vs##p;\
    vd##p = (td *)memory;\
    *vd##p = (td)vs;\
    break;

    for(i=0;i<count;i++) {

        switch (CASE(srctype,dsttype)) {
ARM(ncchar,NC_CHAR,char,ncchar,NC_CHAR,char)
ARM(ncchar,NC_CHAR,char,ncbyte,NC_BYTE,signed char)
ARM(ncchar,NC_CHAR,char,ncubyte,NC_UBYTE,unsigned char)
ARM(ncchar,NC_CHAR,char,ncshort,NC_SHORT,short)
ARM(ncchar,NC_CHAR,char,ncushort,NC_USHORT,unsigned short)
ARM(ncchar,NC_CHAR,char,ncint,NC_INT,int)
ARM(ncchar,NC_CHAR,char,ncuint,NC_UINT,unsigned int)
ARM(ncchar,NC_CHAR,char,ncint64,NC_INT64,long long)
ARM(ncchar,NC_CHAR,char,ncuint64,NC_UINT64,unsigned long long)
ARM(ncchar,NC_CHAR,char,ncfloat,NC_FLOAT,float)
ARM(ncchar,NC_CHAR,char,ncdouble,NC_DOUBLE,double)
ARM(ncbyte,NC_BYTE,signed char,ncchar,NC_CHAR,char)
ARM(ncbyte,NC_BYTE,signed char,ncbyte,NC_BYTE,signed char)
ARM(ncbyte,NC_BYTE,signed char,ncubyte,NC_UBYTE,unsigned char)
ARM(ncbyte,NC_BYTE,signed char,ncshort,NC_SHORT,short)
ARM(ncbyte,NC_BYTE,signed char,ncushort,NC_USHORT,unsigned short)
ARM(ncbyte,NC_BYTE,signed char,ncint,NC_INT,int)
ARM(ncbyte,NC_BYTE,signed char,ncuint,NC_UINT,unsigned int)
ARM(ncbyte,NC_BYTE,signed char,ncint64,NC_INT64,long long)
ARM(ncbyte,NC_BYTE,signed char,ncuint64,NC_UINT64,unsigned long long)
ARM(ncbyte,NC_BYTE,signed char,ncfloat,NC_FLOAT,float)
ARM(ncbyte,NC_BYTE,signed char,ncdouble,NC_DOUBLE,double)
ARM(ncubyte,NC_UBYTE,unsigned char,ncchar,NC_CHAR,char)
ARM(ncubyte,NC_UBYTE,unsigned char,ncbyte,NC_BYTE,signed char)
ARM(ncubyte,NC_UBYTE,unsigned char,ncubyte,NC_UBYTE,unsigned char)
ARM(ncubyte,NC_UBYTE,unsigned char,ncshort,NC_SHORT,short)
ARM(ncubyte,NC_UBYTE,unsigned char,ncushort,NC_USHORT,unsigned short)
ARM(ncubyte,NC_UBYTE,unsigned char,ncint,NC_INT,int)
ARM(ncubyte,NC_UBYTE,unsigned char,ncuint,NC_UINT,unsigned int)
ARM(ncubyte,NC_UBYTE,unsigned char,ncint64,NC_INT64,long long)
ARM(ncubyte,NC_UBYTE,unsigned char,ncuint64,NC_UINT64,unsigned long long)
ARM(ncubyte,NC_UBYTE,unsigned char,ncfloat,NC_FLOAT,float)
ARM(ncubyte,NC_UBYTE,unsigned char,ncdouble,NC_DOUBLE,double)
ARM(ncshort,NC_SHORT,short,ncchar,NC_CHAR,char)
ARM(ncshort,NC_SHORT,short,ncbyte,NC_BYTE,signed char)
ARM(ncshort,NC_SHORT,short,ncubyte,NC_UBYTE,unsigned char)
ARM(ncshort,NC_SHORT,short,ncshort,NC_SHORT,short)
ARM(ncshort,NC_SHORT,short,ncushort,NC_USHORT,unsigned short)
ARM(ncshort,NC_SHORT,short,ncint,NC_INT,int)
ARM(ncshort,NC_SHORT,short,ncuint,NC_UINT,unsigned int)
ARM(ncshort,NC_SHORT,short,ncint64,NC_INT64,long long)
ARM(ncshort,NC_SHORT,short,ncuint64,NC_UINT64,unsigned long long)
ARM(ncshort,NC_SHORT,short,ncfloat,NC_FLOAT,float)
ARM(ncshort,NC_SHORT,short,ncdouble,NC_DOUBLE,double)
ARM(ncushort,NC_USHORT,unsigned short,ncchar,NC_CHAR,char)
ARM(ncushort,NC_USHORT,unsigned short,ncbyte,NC_BYTE,signed char)
ARM(ncushort,NC_USHORT,unsigned short,ncubyte,NC_UBYTE,unsigned char)
ARM(ncushort,NC_USHORT,unsigned short,ncshort,NC_SHORT,short)
ARM(ncushort,NC_USHORT,unsigned short,ncushort,NC_USHORT,unsigned short)
ARM(ncushort,NC_USHORT,unsigned short,ncint,NC_INT,int)
ARM(ncushort,NC_USHORT,unsigned short,ncuint,NC_UINT,unsigned int)
ARM(ncushort,NC_USHORT,unsigned short,ncint64,NC_INT64,long long)
ARM(ncushort,NC_USHORT,unsigned short,ncuint64,NC_UINT64,unsigned long long)
ARM(ncushort,NC_USHORT,unsigned short,ncfloat,NC_FLOAT,float)
ARM(ncushort,NC_USHORT,unsigned short,ncdouble,NC_DOUBLE,double)
ARM(ncint,NC_INT,int,ncchar,NC_CHAR,char)
ARM(ncint,NC_INT,int,ncbyte,NC_BYTE,signed char)
ARM(ncint,NC_INT,int,ncubyte,NC_UBYTE,unsigned char)
ARM(ncint,NC_INT,int,ncshort,NC_SHORT,short)
ARM(ncint,NC_INT,int,ncushort,NC_USHORT,unsigned short)
ARM(ncint,NC_INT,int,ncint,NC_INT,int)
ARM(ncint,NC_INT,int,ncuint,NC_UINT,unsigned int)
ARM(ncint,NC_INT,int,ncint64,NC_INT64,long long)
ARM(ncint,NC_INT,int,ncuint64,NC_UINT64,unsigned long long)
ARM(ncint,NC_INT,int,ncfloat,NC_FLOAT,float)
ARM(ncint,NC_INT,int,ncdouble,NC_DOUBLE,double)
ARM(ncuint,NC_UINT,unsigned int,ncchar,NC_CHAR,char)
ARM(ncuint,NC_UINT,unsigned int,ncbyte,NC_BYTE,signed char)
ARM(ncuint,NC_UINT,unsigned int,ncubyte,NC_UBYTE,unsigned char)
ARM(ncuint,NC_UINT,unsigned int,ncshort,NC_SHORT,short)
ARM(ncuint,NC_UINT,unsigned int,ncushort,NC_USHORT,unsigned short)
ARM(ncuint,NC_UINT,unsigned int,ncint,NC_INT,int)
ARM(ncuint,NC_UINT,unsigned int,ncuint,NC_UINT,unsigned int)
ARM(ncuint,NC_UINT,unsigned int,ncint64,NC_INT64,long long)
ARM(ncuint,NC_UINT,unsigned int,ncuint64,NC_UINT64,unsigned long long)
ARM(ncuint,NC_UINT,unsigned int,ncfloat,NC_FLOAT,float)
ARM(ncuint,NC_UINT,unsigned int,ncdouble,NC_DOUBLE,double)
ARM(ncint64,NC_INT64,long long,ncchar,NC_CHAR,char)
ARM(ncint64,NC_INT64,long long,ncbyte,NC_BYTE,signed char)
ARM(ncint64,NC_INT64,long long,ncubyte,NC_UBYTE,unsigned char)
ARM(ncint64,NC_INT64,long long,ncshort,NC_SHORT,short)
ARM(ncint64,NC_INT64,long long,ncushort,NC_USHORT,unsigned short)
ARM(ncint64,NC_INT64,long long,ncint,NC_INT,int)
ARM(ncint64,NC_INT64,long long,ncuint,NC_UINT,unsigned int)
ARM(ncint64,NC_INT64,long long,ncint64,NC_INT64,long long)
ARM(ncint64,NC_INT64,long long,ncuint64,NC_UINT64,unsigned long long)
ARM(ncint64,NC_INT64,long long,ncfloat,NC_FLOAT,float)
ARM(ncint64,NC_INT64,long long,ncdouble,NC_DOUBLE,double)
ARM(ncuint64,NC_UINT64,unsigned long long,ncchar,NC_CHAR,char)
ARM(ncuint64,NC_UINT64,unsigned long long,ncbyte,NC_BYTE,signed char)
ARM(ncuint64,NC_UINT64,unsigned long long,ncubyte,NC_UBYTE,unsigned char)
ARM(ncuint64,NC_UINT64,unsigned long long,ncshort,NC_SHORT,short)
ARM(ncuint64,NC_UINT64,unsigned long long,ncushort,NC_USHORT,unsigned short)
ARM(ncuint64,NC_UINT64,unsigned long long,ncint,NC_INT,int)
ARM(ncuint64,NC_UINT64,unsigned long long,ncuint,NC_UINT,unsigned int)
ARM(ncuint64,NC_UINT64,unsigned long long,ncint64,NC_INT64,long long)
ARM(ncuint64,NC_UINT64,unsigned long long,ncuint64,NC_UINT64,unsigned long long)
ARM(ncuint64,NC_UINT64,unsigned long long,ncfloat,NC_FLOAT,float)
ARM(ncuint64,NC_UINT64,unsigned long long,ncdouble,NC_DOUBLE,double)
ARM(ncfloat,NC_FLOAT,float,ncchar,NC_CHAR,char)
ARM(ncfloat,NC_FLOAT,float,ncbyte,NC_BYTE,signed char)
ARM(ncfloat,NC_FLOAT,float,ncubyte,NC_UBYTE,unsigned char)
ARM(ncfloat,NC_FLOAT,float,ncshort,NC_SHORT,short)
ARM(ncfloat,NC_FLOAT,float,ncushort,NC_USHORT,unsigned short)
ARM(ncfloat,NC_FLOAT,float,ncint,NC_INT,int)
ARM(ncfloat,NC_FLOAT,float,ncuint,NC_UINT,unsigned int)
ARM(ncfloat,NC_FLOAT,float,ncint64,NC_INT64,long long)
ARM(ncfloat,NC_FLOAT,float,ncuint64,NC_UINT64,unsigned long long)
ARM(ncfloat,NC_FLOAT,float,ncfloat,NC_FLOAT,float)
ARM(ncfloat,NC_FLOAT,float,ncdouble,NC_DOUBLE,double)
ARM(ncdouble,NC_DOUBLE,double,ncchar,NC_CHAR,char)
ARM(ncdouble,NC_DOUBLE,double,ncbyte,NC_BYTE,signed char)
ARM(ncdouble,NC_DOUBLE,double,ncubyte,NC_UBYTE,unsigned char)
ARM(ncdouble,NC_DOUBLE,double,ncshort,NC_SHORT,short)
ARM(ncdouble,NC_DOUBLE,double,ncushort,NC_USHORT,unsigned short)
ARM(ncdouble,NC_DOUBLE,double,ncint,NC_INT,int)
ARM(ncdouble,NC_DOUBLE,double,ncuint,NC_UINT,unsigned int)
ARM(ncdouble,NC_DOUBLE,double,ncint64,NC_INT64,long long)
ARM(ncdouble,NC_DOUBLE,double,ncuint64,NC_UINT64,unsigned long long)
ARM(ncdouble,NC_DOUBLE,double,ncfloat,NC_FLOAT,float)
ARM(ncdouble,NC_DOUBLE,double,ncdouble,NC_DOUBLE,double)
	
        default: ncstat = NC_EINVAL; THROWCHK(ncstat); goto fail;
        }
        value += nctypesizeof(srctype);
        memory += nctypesizeof(dsttype);
    }

fail:
    return THROW(ncstat);
}

/**
Convert an attribute value string to a specific type.
If the conversion fails, then return NC_EBADTYPE.
If we need an int and the string value is out of range, return NC_ERANGE.
@param etype the target type
@param dst the memory into which to put the converted constants
@param src list of constants as strings
*/
NCerror
dapcvtattrval(nc_type etype, void* dst, NClist* src, NCattribute* att)
{
    int i;
    NCerror  ncstat = NC_NOERR;
    unsigned int memsize = nctypesizeof(etype);
    unsigned int nvalues = nclistlength(src);
    char* dstmem = (char*)dst;

    for(i=0;i<nvalues;i++) {
	/* Convert numeric looking constants to either double
           or signed long long */
	char* s;
	size_t slen;

	s = (char*)nclistget(src,i);
        slen = strlen(s);
	if(etype <= NC_DOUBLE && etype != NC_CHAR) {
	    struct Value val;
	    int stype;
	    /* Target types */
	    unsigned char* u8p;
	    short* i16p;
	    unsigned short* u16p;
	    int* i32p;
	    unsigned int* u32p;
	    float* fp;
	    double* dp;

	    /* Convert string to a number */
	    stype = cvtnumconst(s,&val);
	    if(stype == NC_NAT) {
	        nclog(NCLOGERR,"Unexpected attribute value: %s = %s",att->name,s);
	        ncstat = NC_EBADTYPE;
                goto done;
            }
	    if((ncstat = rangecheck(stype,etype,&val))) goto done;
	    if(ncstat == NC_ERANGE) {
		nclog(NCLOGERR,"Attribute value out of range: %s = %s",att->name,s);
		goto done;
	    }
	    switch (etype) {
	    case NC_BYTE:
		/* Note that in DAP2, this is unsigned 8-bit integer */
		u8p = (unsigned char*)dstmem;
	        *u8p = (unsigned char)(val.llval & 0xFF);
		break;	
	    case NC_SHORT:
		i16p = (short*)dstmem;
		*i16p = (short)(val.llval);
		break;	
	    case NC_USHORT:
		u16p = (unsigned short*)dstmem;
		*u16p = (unsigned short)(val.llval & 0xFFFF);
		break;	
	    case NC_INT:
		i32p = (int*)dstmem;
		*i32p = (int)(val.llval);
		break;	
	    case NC_UINT:
		u32p = (unsigned int*)dstmem;
		*u32p = (unsigned int)(val.llval & 0xFFFFFFFF);
		break;	
	    case NC_FLOAT:
		fp = (float*)dstmem;
		*fp = (float)(val.dval);
		break;	
	    case NC_DOUBLE:
		dp = (double*)dstmem;
		*dp = (double)(val.dval);
		break;	
	    default: return NC_EINTERNAL;
	    }
	} else if(etype == NC_CHAR) {
	    char* p = (char*)dstmem;
	    size_t count, nread;
	    count = sscanf(s,"%c%n",p,&nread);
	    if(count != 1 || nread != slen)
	        {ncstat = NC_ETRANSLATION; goto done;}
	} else if(etype == NC_STRING || etype == NC_URL) {
	    char** p = (char**)dstmem;
	    *p = nulldup(s);
	} else {
   	    PANIC1("unexpected nc_type: %d",(int)etype);
	}
	dstmem += memsize;
    }
done:
    if(ncstat == NC_EBADTYPE)
	nclog(NCLOGERR,"Unexpected attribute type: %s",att->name);
    return THROW(ncstat);
}

/**
@param val resulting converted numeric value
@return NC_INT || NC_DOUBLE || NC_NAT (failure)
*/
static int
cvtnumconst(const char* s, struct Value* val)
{
    size_t slen = strlen(s);
    size_t nread; /* # of chars read */
    size_t count; /* # of conversions */
    /* Try to convert to integer first */
    count = sscanf(s,"%lld%n",&val->llval,&nread);
    if(count == 1 && nread == slen)
	return NC_INT;
    /* Try to convert to float second */
#if defined(_WIN32)
    if (!_strnicmp(s, "NaN", 3)) {count = 1; nread = 3; val->llval = NAN;} else
#endif
	count = sscanf(s,"%lg%n",&val->dval,&nread);
    if(count == 1 && nread == slen)
	    return NC_DOUBLE;
    return NC_INT;
}

/**
@param srcttype
@param dsttype convert srcval to this integer type
@param srcval value to range check and convert
@param val store converted value
@return NC_NOERR | NC_ERANGE | NC_EBADTYPE
*/
static int
rangecheck(nc_type srctype, nc_type dsttype, struct Value* val)
{
    /* assert srctype == NC_INT || srctype == NC_DOUBLE */
    /* assert dsttype <= NC_DOUBLE && dsttype != NC_CHAR */

    /* Inter-convert */
    if(srctype == NC_DOUBLE) {
	/* Convert to the long long */
	val->llval = (long long)val->dval;
    } else { // (srctype == NC_INT)
	val->dval = (double)val->llval;
    }
    /* Do range checks depending on dsttype */
    switch (dsttype) {
    case NC_BYTE:
        if(val->llval < (long long)NC_MIN_BYTE || val->llval > (long long)NC_MAX_BYTE) return NC_ERANGE;
        break;
    case NC_UBYTE:
        if(val->llval < (long long)0 || val->llval > (long long)NC_MAX_UBYTE) return NC_ERANGE;
        break;
    case NC_SHORT:
        if(val->llval < (long long)NC_MIN_SHORT || val->llval > (long long)NC_MAX_SHORT) return NC_ERANGE;
        break;
    case NC_USHORT:
        if(val->llval < (long long)0 || val->llval > (long long)NC_MAX_USHORT) return NC_ERANGE;
        break;
    case NC_INT:
        if(val->llval < (long long)NC_MIN_INT || val->llval > (long long)NC_MAX_INT) return NC_ERANGE;
        break;
    case NC_UINT:
        if(val->llval < (long long)0 || val->llval > (long long)NC_MAX_UINT) return NC_ERANGE;
        break;
    case NC_FLOAT:
    case NC_DOUBLE:
	/* do nothing */
        break;
    default: return NC_EBADTYPE;
    }
    return NC_NOERR;
}

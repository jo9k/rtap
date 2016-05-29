#ifndef __JVX_DEBUGOUT_H__
#define __JVX_DEBUGOUT_H__

#if 1

#define DATAOUT(dlbout_var, dblout_size, dblout_name) \
  { \
    static FILE *f; \
    if(!f) f=fopen(dblout_name,"wb"); \
    fwrite(dlbout_var, sizeof(jvxData), dblout_size, f); \
  }
#define DATAIN(dblin_var, dblin_size, dblin_name) \
  { \
    static FILE *f; \
    if(!f) f=fopen(dblin_name,"rb"); \
    fread(dblin_var, sizeof(jvxData), dblin_size, f); \
  }
#define SRTOUT(srtout_var, srtout_size, srtout_name) \
  { \
    static FILE *f; \
    if(!f) f=fopen(srtout_name,"wb"); \
    fwrite(srtout_var, sizeof(short), srtout_size, f); \
  }
#define SRTIN(srtin_var, srtin_size, srtin_name) \
  { \
    static FILE *f; \
    if(!f) f=fopen(srtin_name,"rb"); \
    fread(srtin_var, sizeof(short), srtin_size, f); \
  }

#define INTOUT(intout_var, intout_size, intout_name) \
  { \
    static FILE *f; \
    if(!f) f=fopen(intout_name,"wb"); \
    fwrite(intout_var, sizeof(int), intout_size, f);    \
  }
#define INTOUTQ(intout_var, intout_size, intout_name, Q)  \
  { \
    static FILE *f; int q=Q;                            \
    if(!f) {f=fopen(intout_name,"wb");                  \
      fwrite(&q, sizeof(int), 1, f);}                   \
    fwrite(intout_var, sizeof(int), intout_size, f);    \
  }
#define INTIN(intin_var, intin_size, intin_name) \
  { \
    static FILE *f; \
    if(!f) f=fopen(intin_name,"rb"); \
    fread(intin_var, sizeof(int), intin_size, f); \
  }
#else

#define DATAOUT(dlbout_var, dblout_size, dblout_name)
#define DATAOUTF(dlbout_var, dblout_size, dblout_name)
#define DATAIN(dblin_var, dblin_size, dblin_name)
#define SRTOUT(srtout_var, srtout_size, srtout_name)
#define SRTIN(srtin_var, srtin_size, srtin_name)
#define INTOUT(intout_var, intout_size, intout_name)
#define INTOUTQ(intout_var, intout_size, intout_name, Q)
#define INTIN(intin_var, intin_size, intin_name)

#endif

#endif

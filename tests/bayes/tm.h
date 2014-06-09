#ifndef TM_H
#define TM_H 1

#include <stdlib.h>                   /* Defines size_t. */

#define TM_PURE                       __attribute__((transaction_pure))
#define TM_SAFE                       __attribute__((transaction_safe))

#define TM_SHARED_READ(var)           var
#define TM_SHARED_READ_P(var)         var
#define TM_SHARED_READ_F(var)         var

#define TM_SHARED_WRITE(var, val)     var = val
#define TM_SHARED_WRITE_P(var, val)   var = val
#define TM_SHARED_WRITE_F(var, val)   var = val

#define TM_LOCAL_WRITE(var, val)      var = val
#define TM_LOCAL_WRITE_P(var, val)    var = val
#define TM_LOCAL_WRITE_F(var, val)    var = val

/* Indirect function call management */
/* In STAMP applications, it is safe to use transaction_pure */
//#define TM_IFUNC_DECL                 __attribute__((transaction_pure))
#define TM_IFUNC_CALL1(r, f, a1)      r = f(a1)
#define TM_IFUNC_CALL2(r, f, a1, a2)  r = f((a1), (a2))

/* libitm.h stuff */
#ifdef __i386__
# define ITM_REGPARM __attribute__((regparm(2)))
#else
# define ITM_REGPARM
#endif

#endif /* TM_H */
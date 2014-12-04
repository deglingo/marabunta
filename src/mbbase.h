/* mbbase.h -
 */

#ifndef _MBBASE_H_
#define _MBBASE_H_

#include <los.h>
#include <clog.h>



#define ASSERT(expr) do {                           \
    if (!(expr)) {                                  \
      CL_ERROR("ASSERTION FAILED: `" #expr "'");    \
    }                                               \
  } while (0)



#endif

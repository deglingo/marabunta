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



#define MB_PRIORITY_GAME_TIMER 500

/* [FIXME] */
struct _MbGame;
struct _MbSector;
struct _MbColony;
struct _MbPopTree;
struct _MbPriority;
struct _MbTask;

#define MB_COUNT_MAX_CHARS 7

gchar *mb_count_print ( gint64 count,
                        gchar *dest );



#endif

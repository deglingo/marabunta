/* mbwatch.h -
 */

#ifndef _MBWATCH_H_
#define _MBWATCH_H_

#include "common/mbbase.h"



typedef struct _MBWatch MBWatch;
typedef void (* MBWatchFunc) ( GIOCondition condition,
                               gpointer data );

MBWatch *mb_watch_new ( gint fd,
                        MBWatchFunc func,
                        gpointer data,
                        GDestroyNotify destroy );
void mb_watch_add_condition ( MBWatch *watch,
                              GIOCondition condition );
void mb_watch_remove_condition ( MBWatch *watch,
                                 GIOCondition condition );



#endif

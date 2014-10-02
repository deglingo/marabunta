/* mbsapp.h -
 */

#ifndef _MBSAPP_H_
#define _MBSAPP_H_

#include "server/mbsbase.h"



/* [FIXME] */
typedef struct _MainData MbsApp;



void mbs_app_send ( MbsApp *app,
                    guint clid,
                    LObject *msg );



#endif

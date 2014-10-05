/* mbsapp.h -
 */

#ifndef _MBSAPP_H_
#define _MBSAPP_H_

#include "server/mbsbase.h"
#include "server/mbsserver.h"
#include "server/mbsgame.h"
#include "server/mbsapp-def.h"



/* MbsApp:
 */
struct _MbsApp
{
  MBS_APP_INSTANCE_HEADER;

  GMainLoop *loop;
  MbsServer *server;
  MbsGame *game;
};



/* MbsAppClass:
 */
struct _MbsAppClass
{
  MBS_APP_CLASS_HEADER;
};



MbsApp *mbs_app_new ( void );
gint mbs_app_main ( MbsApp *app,
                    gint argc,
                    gchar **argv );
void mbs_app_send ( MbsApp *app,
                    guint clid,
                    LObject *msg );



#endif

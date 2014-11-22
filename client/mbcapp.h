/* mbcapp.h -
 */

#ifndef _MBCAPP_H_
#define _MBCAPP_H_

#include "client/mbcbase.h"
#include "client/mbcapp-def.h"



/* MbcApp:
 */
struct _MbcApp
{
  MBC_APP_INSTANCE_HEADER;

  /* solo game (server side) */
  MbObject *game;
  MbObject *player;
};



/* MbcAppClass:
 */
struct _MbcAppClass
{
  MBC_APP_CLASS_HEADER;
};



void mbc_app_setup_solo_game ( MbcApp *app );



#endif

/* mbcapp.h -
 */

#ifndef _MBCAPP_H_
#define _MBCAPP_H_

#include "client/mbcbase.h"
#include "client/mbcclient.h"
#include "client/mbcgameproxy.h"
#include "client/mbcapp-def.h"



/* MbcApp:
 */
struct _MbcApp
{
  MBC_APP_INSTANCE_HEADER;

  MbcClient *client;
  /* LPacker *packer; */
  /* LUnpacker *unpacker; */

  /* solo game */
  MbsGame *game; /* [removeme] */
  MbObject *new_game;
  MbsPlayerID player;

  /* proxy datas */
  MbcGameProxy *game_proxy;
};



/* MbcAppClass:
 */
struct _MbcAppClass
{
  MBC_APP_CLASS_HEADER;

  /* [fixme] */
  void (* setup_proxy) ( MbcApp *app );
};



void mbc_app_setup_solo_game ( MbcApp *app );
void mbc_app_setup_proxy ( MbcApp *app );
void mbc_app_send_message ( MbcApp *app,
                            MbMessage *msg );



#endif

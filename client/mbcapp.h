/* mbcapp.h -
 */

#ifndef _MBCAPP_H_
#define _MBCAPP_H_

#include "client/mbcbase.h"
#include "client/mbcclient.h"
#include "client/mbcapp-def.h"



/* MbcApp:
 */
struct _MbcApp
{
  MBC_APP_INSTANCE_HEADER;

  MbcClient *client;
  LPacker *packer;
  LUnpacker *unpacker;
  LptTree *tree;
  LptClient *tclient_server;
};



/* MbcAppClass:
 */
struct _MbcAppClass
{
  MBC_APP_CLASS_HEADER;
};



void mbc_app_connect ( MbcApp *app );
void mbc_app_join_game ( MbcApp *app );



#endif

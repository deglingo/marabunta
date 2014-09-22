/* mbcclient.h -
 */

#ifndef _MBCCLIENT_H_
#define _MBCCLIENT_H_

#include "client/mbcbase.h"



typedef struct _MbcClient MbcClient;

typedef void (* MbcClientHandler) ( MbcClient *client,
                                    GIOCondition condition,
                                    LStream *stream,
                                    gpointer data );



/* MbcClient:
 */
struct _MbcClient
{
  gint sock;
  MBWatch *watch;
  MbcClientHandler handler;
  gpointer handler_data;
  LStream *stream;
};



MbcClient *mbc_client_new ( MbcClientHandler handler,
                            gpointer data );
gint mbc_client_connect ( MbcClient *cli,
						  const gchar *host,
						  guint16 port );
void mbc_client_send ( MbcClient *cli,
                       MbMessage *msg );
void mbc_client_add_watch ( MbcClient *client,
                            GIOCondition condition );
void mbc_client_remove_watch ( MbcClient *client,
                               GIOCondition condition );



#endif

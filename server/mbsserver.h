/* mbsserver.h -
 */

#ifndef _MBSSERVER_H_
#define _MBSSERVER_H_

#include "server/mbsbase.h"



typedef struct _MbsServer MbsServer;

typedef union _MbsServerEvent MbsServerEvent;
typedef void (* MbsServerHandler) ( MbsServerEvent *event,
									gpointer data );



/* MbsServer:
 */
struct _MbsServer
{
  MbsServerHandler handler;
  gpointer handler_data;
  guint16 port;
  gint listen_sock;
  GIOChannel *listen_chan;
};



MbsServer *mbs_server_new ( MbsServerHandler handler,
							gpointer handler_data );
void mbs_server_start ( MbsServer *server );



#endif

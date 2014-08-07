/* mbsserver.h -
 */

#ifndef _MBSSERVER_H_
#define _MBSSERVER_H_

#include "server/mbsbase.h"



typedef struct _MbsServer MbsServer;



/* MbsServer:
 */
struct _MbsServer
{
  guint16 port;
  gint listen_sock;
  GIOChannel *listen_chan;
};



MbsServer *mbs_server_new ( void );
void mbs_server_start ( MbsServer *server );



#endif

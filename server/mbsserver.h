/* mbsserver.h -
 */

#ifndef _MBSSERVER_H_
#define _MBSSERVER_H_

#include "server/mbsbase.h"



typedef struct _MbsServer MbsServer;

typedef enum _MbsServerEventType MbsServerEventType;
typedef struct _MbsServerEventAccept MbsServerEventAccept;
typedef struct _MbsServerEventMessage MbsServerEventMessage;
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
  guint client_counter;
  GHashTable *client_map;
};



#define _MBS_SERVER_EVENT_HEADER \
  MbsServerEventType type



/* MbsServerEventType:
 */
enum _MbsServerEventType
  {
	MBS_SERVER_EVENT_ACCEPT,
    MBS_SERVER_EVENT_MESSAGE,
  };



/* MbsServerEventAccept:
 */
struct _MbsServerEventAccept {
  _MBS_SERVER_EVENT_HEADER;
  guint clid;
};



/* MbsServerEventMessage:
 */
struct _MbsServerEventMessage
{
  _MBS_SERVER_EVENT_HEADER;
  guint clid;
  MbMessage *message;
};



/* MbsServerEvent:
 */
union _MbsServerEvent
{
  MbsServerEventType type;
  MbsServerEventAccept accept;
  MbsServerEventMessage message;
};



MbsServer *mbs_server_new ( MbsServerHandler handler,
							gpointer handler_data );
void mbs_server_start ( MbsServer *server );



#endif

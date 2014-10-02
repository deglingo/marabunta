/* mbsserver.h -
 */

#ifndef _MBSSERVER_H_
#define _MBSSERVER_H_

#include "server/mbsbase.h"



typedef struct _MbsServer MbsServer;

typedef enum _MbsServerEventType MbsServerEventType;
typedef struct _MbsServerEventAccept MbsServerEventAccept;
typedef struct _MbsServerEventMessage MbsServerEventMessage;
typedef struct _MbsServerEventReady MbsServerEventReady;
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
    MBS_SERVER_EVENT_READY,
  };



/* MbsServerEventAccept:
 */
struct _MbsServerEventAccept {
  _MBS_SERVER_EVENT_HEADER;
  guint clid;
  LStream *stream;
};



/* MbsServerEventMessage:
 */
struct _MbsServerEventMessage
{
  _MBS_SERVER_EVENT_HEADER;
  guint clid;
  MbMessage *message;
};



/* MbsServerEventReady:
 */
struct _MbsServerEventReady
{
  _MBS_SERVER_EVENT_HEADER;
  guint clid;
  GIOCondition condition;
};



/* MbsServerEvent:
 */
union _MbsServerEvent
{
  MbsServerEventType type;
  MbsServerEventAccept accept;
  MbsServerEventMessage message;
  MbsServerEventReady ready;
};



MbsServer *mbs_server_new ( MbsServerHandler handler,
							gpointer handler_data );
void mbs_server_start ( MbsServer *server );
void mbs_server_add_watch ( MbsServer *server,
                            guint clid,
                            GIOCondition condition );
void mbs_server_remove_watch ( MbsServer *server,
                               guint clid,
                               GIOCondition condition );



#endif

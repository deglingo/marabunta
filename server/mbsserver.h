/* mbsserver.h -
 */

#ifndef _MBSSERVER_H_
#define _MBSSERVER_H_

#include "server/mbsbase.h"



typedef struct _MbsServer MbsServer;
typedef gpointer MbsClientID;

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
  GList *clients;
};



#define _MBS_SERVER_EVENT_HEADER \
  MbsServerEventType type; \
  MbsClientID client



/* MbsServerEventType:
 */
enum _MbsServerEventType
  {
	MBS_SERVER_EVENT_ACCEPT,
    MBS_SERVER_EVENT_READY,
  };



/* MbsServerEventAccept:
 */
struct _MbsServerEventAccept {
  _MBS_SERVER_EVENT_HEADER;
  LStream *stream;
};



/* MbsServerEventReady:
 */
struct _MbsServerEventReady
{
  _MBS_SERVER_EVENT_HEADER;
  GIOCondition condition;
};



/* MbsServerEvent:
 */
union _MbsServerEvent
{
  MbsServerEventType type;
  MbsServerEventAccept accept;
  MbsServerEventReady ready;
};



MbsServer *mbs_server_new ( MbsServerHandler handler,
							gpointer handler_data );
void mbs_server_start ( MbsServer *server );

void mbs_client_set_data ( MbsClientID client,
                           gpointer data,
                           GDestroyNotify destroy );
gpointer mbs_client_get_data ( MbsClientID client );
void mbs_server_add_watch ( MbsServer *server,
                            MbsClientID client,
                            GIOCondition condition );
void mbs_server_remove_watch ( MbsServer *server,
                               MbsClientID client,
                               GIOCondition condition );



#endif

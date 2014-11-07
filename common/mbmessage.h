/* mbmessage.h -
 */

#ifndef _MBMESSAGE_H_
#define _MBMESSAGE_H_

#include "common/mbbase.h"
#include "common/mbmessage-def.h"



typedef enum _MbMessageKey MbMessageKey;



/* MbMessageKey:
 */
enum _MbMessageKey
  {
    MB_MESSAGE_KEY_JOIN,
    MB_MESSAGE_KEY_LPT_EVENT,
    MB_MESSAGE_KEY_GAME_SETUP,
    MB_MESSAGE_KEY_GAME_STATE,

    MB_MESSAGE_KEY_REQUEST_PRIORITY,
  };



/* MbMessage:
 */
struct _MbMessage
{
  MB_MESSAGE_INSTANCE_HEADER;

  MbMessageKey key;
  LObject *arg;
  /* [removeme] */
  guint frame;
};



/* MbMessageClass:
 */
struct _MbMessageClass
{
  MB_MESSAGE_CLASS_HEADER;
};



MbMessage *mb_message_new ( MbMessageKey key,
                            LObject *arg );



#endif

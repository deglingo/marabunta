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
  };



/* MbMessage:
 */
struct _MbMessage
{
  MB_MESSAGE_INSTANCE_HEADER;

  MbMessageKey key;
};



/* MbMessageClass:
 */
struct _MbMessageClass
{
  MB_MESSAGE_CLASS_HEADER;
};



MbMessage *mb_message_new ( MbMessageKey key,
                            const gchar *format,
                            ... )
G_GNUC_NULL_TERMINATED;



#endif

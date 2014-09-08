/* mbmessage.h -
 */

#ifndef _MBMESSAGE_H_
#define _MBMESSAGE_H_

#include "common/mbbase.h"



typedef struct _MbMessage MbMessage;
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
  MbMessageKey key;
};



MbMessage *mb_message_new ( MbMessageKey key,
                            const gchar *format,
                            ... )
G_GNUC_NULL_TERMINATED;
void mb_message_free ( MbMessage *msg );
gsize mb_message_pack_size ( MbMessage *msg );
void mb_message_pack ( MbMessage *msg,
                       gpointer dest );
MbMessage *mb_message_unpack ( gpointer pack,
                               gsize size );



#endif

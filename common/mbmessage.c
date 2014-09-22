/* mbmessage.c -
 */

#include "common/mbmessage.h"
#include "common/mbmessage.inl"

#include <string.h>



/* mb_message_new:
 */
MbMessage *mb_message_new ( MbMessageKey key,
                            const gchar *format,
                            ... )
{
  MbMessage *msg;
  msg = MB_MESSAGE(l_object_new(MB_CLASS_MESSAGE, NULL));
  msg->key = key;
  return msg;
}

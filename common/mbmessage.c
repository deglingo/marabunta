/* mbmessage.c -
 */

#include "common/mbmessage.h"



/* mb_message_new:
 */
MbMessage *mb_message_new ( MbMessageKey key,
                            const gchar *format,
                            ... )
{
  gsize size;
  gpointer block;
  size = sizeof(MbMessage);
  block = g_malloc0(size);
  ((MbMessage *) block)->key = key;
  return (MbMessage *) block;
}



/* mb_message_free:
 */
void mb_message_free ( MbMessage *msg )
{
  g_free(msg);
}



/* mb_message_pack:
 */
gpointer mb_message_pack ( MbMessage *msg,
                           gsize *size )
{
  *size = 0;
  return NULL;
}

/* mbmessage.c -
 */

#include "common/mbmessage.h"

#include <string.h>



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



/* mb_message_pack_size:
 */
gsize mb_message_pack_size ( MbMessage *msg )
{
  return sizeof(gint);
}



/* mb_message_pack:
 */
void mb_message_pack ( MbMessage *msg,
                       gpointer dest )
{
  gint nkey;
  nkey = g_htonl(msg->key);
  memcpy(dest, &nkey, sizeof(gint));
}



/* mb_message_unpack:
 */
MbMessage *mb_message_unpack ( gpointer pack,
                               gsize size )
{
  MbMessage *msg;
  gint nkey;
  /* ASSERT(size == sizeof(gint)); */
  msg = g_new0(MbMessage, 1);
  memcpy(&nkey, pack, sizeof(gint));
  msg->key = g_ntohl(nkey);
  return msg;
}

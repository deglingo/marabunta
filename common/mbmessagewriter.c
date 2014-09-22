/* mbmessagewriter.c -
 */

#include "common/mbmessagewriter.h"



/* MbMessageWriter:
 */
struct _MbMessageWriter
{
  int _dummy;
};



/* mb_message_writer_new:
 */
MbMessageWriter *mb_message_writer_new ( void )
{
  MbMessageWriter *wr;
  wr = g_new0(MbMessageWriter, 1);
  return wr;
}



/* mb_message_writer_push:
 */
void mb_message_writer_push ( MbMessageWriter *writer,
                              LObject *msg )
{
}



/* mb_message_writer_send:
 */
gboolean mb_message_writer_send ( MbMessageWriter *writer,
                                  LStream *stream,
                                  GError **error )
{
  return TRUE;
}

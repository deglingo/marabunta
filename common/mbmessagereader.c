/* mbmessagereader.c -
 */

#include "common/mbmessagereader.h"



/* MbMessageReader:
 */
struct _MbMessageReader
{
  int _dummy;
};



/* mb_message_reader_new:
 */
MbMessageReader *mb_message_reader_new ( LStream *stream )
{
  MbMessageReader *rd;
  rd = g_new0(MbMessageReader, 1);
  return rd;
}



/* mb_message_reader_recv:
 */
LObject *mb_message_reader_recv ( MbMessageReader *reader,
                                  GError **error )
{
  return L_OBJECT(l_string_new("MSG", -1));
}

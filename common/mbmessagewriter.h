/* mbmessagewriter.h -
 */

#ifndef _MBMESSAGEWRITER_H_
#define _MBMESSAGEWRITER_H_

#include "common/mbbase.h"



typedef struct _MbMessageWriter MbMessageWriter;



MbMessageWriter *mb_message_writer_new ( void );
void mb_message_writer_push ( MbMessageWriter *writer,
                              LObject *msg );
gboolean mb_message_writer_send ( MbMessageWriter *writer,
                                  LStream *stream,
                                  GError **error );



#endif

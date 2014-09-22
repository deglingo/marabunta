/* mbmessagereader.h -
 */

#ifndef _MBMESSAGEREADER_H_
#define _MBMESSAGEREADER_H_

#include "common/mbbase.h"



typedef struct _MbMessageReader MbMessageReader;



MbMessageReader *mb_message_reader_new ( void );
LObject *mb_message_reader_recv ( MbMessageReader *reader,
                                  LStream *stream,
                                  GError **error );



#endif

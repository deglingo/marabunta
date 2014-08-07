/* mbsserver.h -
 */

#ifndef _MBSSERVER_H_
#define _MBSSERVER_H_



typedef struct _MbsServer MbsServer;



MbsServer *mbs_server_new ( void );
void mbs_server_start ( MbsServer *server );



#endif

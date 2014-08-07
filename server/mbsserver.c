/* mbsserver.c -
 */

#include "server/mbsserver.h"



/* mbs_server_new:
 */
MbsServer *mbs_server_new ( void )
{
  MbsServer *s;
  s = g_new0(MbsServer, 1);
  return s;
}



/* mbs_server_start:
 */
void mbs_server_start ( MbsServer *server )
{
}

/* mbcclient.h -
 */

#ifndef _MBCCLIENT_H_
#define _MBCCLIENT_H_

#include "client/mbcbase.h"



typedef struct _MbcClient MbcClient;



/* MbcClient:
 */
struct _MbcClient
{
  gint sock;
};



MbcClient *mbc_client_new ( void );
gint mbc_client_connect ( MbcClient *cli,
						  const gchar *host,
						  guint16 port );



#endif

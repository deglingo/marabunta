/* mbalapp.h -
 */

#ifndef _MBALAPP_H_
#define _MBALAPP_H_

#include "client-al/mbalbase.h"
#include "client-al/mbalapp-def.h"



/* MbalApp:
 */
struct _MbalApp
{
  MBAL_APP_INSTANCE_HEADER;

  AltkDisplay *display;
  AltkWidget *dialog;
};



/* MbalAppClass:
 */
struct _MbalAppClass
{
  MBAL_APP_CLASS_HEADER;
};



MbApp *mbal_app_new ( void );



#endif

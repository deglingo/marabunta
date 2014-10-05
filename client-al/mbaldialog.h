/* mbaldialog.h -
 */

#ifndef _MBALDIALOG_H_
#define _MBALDIALOG_H_

#include "client-al/mbalbase.h"



AltkWidget *mbal_dialog_create ( AltkDisplay *display );
void mbal_dialog_setup_game ( AltkWidget *dialog,
                              LptTree *tree );



#endif

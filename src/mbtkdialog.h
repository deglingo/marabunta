/* mbtkdialog.h -
 */

#ifndef _MBTKDIALOG_H_
#define _MBTKDIALOG_H_

#include "mbtkbase.h"
#include "mbtkdialog-def.h"

G_BEGIN_DECLS



/* MbtkDialog:
 */
struct _MbtkDialog
{
  MBTK_DIALOG_INSTANCE_HEADER;
};



/* MbtkDialogClass:
 */
struct _MbtkDialogClass
{
  MBTK_DIALOG_CLASS_HEADER;
};



AltkWidget *mbtk_dialog_new ( AltkDisplay *display );



G_END_DECLS

#endif /* ifndef _MBTKDIALOG_H_ */

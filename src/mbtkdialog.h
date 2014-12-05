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

  gpointer private;
};



/* MbtkDialogClass:
 */
struct _MbtkDialogClass
{
  MBTK_DIALOG_CLASS_HEADER;
};



AltkWidget *mbtk_dialog_new ( AltkDisplay *display );
void mbtk_dialog_set_game ( MbtkDialog *dialog,
                            struct _MbGame *game );
void mbtk_dialog_set_sector ( MbtkDialog *dialog,
                              struct _MbSector *sector );



G_END_DECLS

#endif /* ifndef _MBTKDIALOG_H_ */

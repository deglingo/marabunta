/* mbtkinfopanel.h -
 */

#ifndef _MBTKINFOPANEL_H_
#define _MBTKINFOPANEL_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtkinfopanel-def.h"

G_BEGIN_DECLS



/* MbtkInfoPanel:
 */
struct _MbtkInfoPanel
{
  MBTK_INFO_PANEL_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkInfoPanelClass:
 */
struct _MbtkInfoPanelClass
{
  MBTK_INFO_PANEL_CLASS_HEADER;
};



AltkWidget *mbtk_info_panel_new ( void );
void mbtk_info_panel_set_colony ( MbtkInfoPanel *panel,
                                  MbcProxy *colony );



G_END_DECLS

#endif /* ifndef _MBTKINFOPANEL_H_ */

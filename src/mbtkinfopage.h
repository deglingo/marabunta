/* mbtkinfopage.h -
 */

#ifndef _MBTKINFOPAGE_H_
#define _MBTKINFOPAGE_H_

#include "mbtkbase.h"
#include "mbtkinfopage-def.h"

G_BEGIN_DECLS



/* MbtkInfoPage:
 */
struct _MbtkInfoPage
{
  MBTK_INFO_PAGE_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkInfoPageClass:
 */
struct _MbtkInfoPageClass
{
  MBTK_INFO_PAGE_CLASS_HEADER;
};



void mbtk_info_page_set_sector ( MbtkInfoPage *page,
                                 MbSector *sector );



G_END_DECLS

#endif /* ifndef _MBTKINFOPAGE_H_ */

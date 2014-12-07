/* mbtkroomspage.h -
 */

#ifndef _MBTKROOMSPAGE_H_
#define _MBTKROOMSPAGE_H_

#include "mbtkinfopage.h"
#include "mbtkroomspage-def.h"

G_BEGIN_DECLS



/* MbtkRoomsPage:
 */
struct _MbtkRoomsPage
{
  MBTK_ROOMS_PAGE_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkRoomsPageClass:
 */
struct _MbtkRoomsPageClass
{
  MBTK_ROOMS_PAGE_CLASS_HEADER;
};



AltkWidget *mbtk_rooms_page_new ( void );



G_END_DECLS

#endif /* ifndef _MBTKROOMSPAGE_H_ */

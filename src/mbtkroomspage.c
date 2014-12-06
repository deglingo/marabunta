/* mbtkroomspage.c -
 */

#include "mbtkroomspage.h"
#include "mbtkroomspage.inl"



/* mbtk_rooms_page_new:
 */
AltkWidget *mbtk_rooms_page_new ( void )
{
  AltkWidget *page;
  page = ALTK_WIDGET(l_object_new(MBTK_CLASS_ROOMS_PAGE, NULL));
  return page;
}

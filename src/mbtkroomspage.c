/* mbtkroomspage.c -
 */

#include "mbtkroomspage.h"
#include "mbtktaskview.h"
#include "mbtkroomspage.inl"



static void _setup ( MbtkInfoPage *page );
static void _cleanup ( MbtkInfoPage *page );



/* mbtk_rooms_page_class_init:
 */
static void mbtk_rooms_page_class_init ( LObjectClass *cls )
{
  MBTK_INFO_PAGE_CLASS(cls)->setup = _setup;
  MBTK_INFO_PAGE_CLASS(cls)->cleanup = _cleanup;
}



/* mbtk_rooms_page_new:
 */
AltkWidget *mbtk_rooms_page_new ( void )
{
  AltkWidget *page;
  page = ALTK_WIDGET(l_object_new_give(MBTK_CLASS_ROOMS_PAGE,
                                       "title", l_string_new("Rooms"),
                                       NULL));
  return page;
}



/* _setup:
 */
static void _setup ( MbtkInfoPage *page )
{
  MbSector *sector;
  AltkWidget *body, *box, *task_view;
  MBTK_INFO_PAGE_CLASS(parent_class)->setup(page);
  sector = mbtk_info_page_get_sector(page);
  CL_TRACE("%p, %p", page, sector);
  if (!(sector && sector->colony))
    return;
  CL_DEBUG("SETUP");
  body = mbtk_info_page_get_body(page);
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(body, box);
  /* top build task */
  task_view = L_TRASH_OBJECT
    (mbtk_task_view_new(sector->colony->t_build));
  ALTK_BOX_ADD(box, task_view, 0);
  altk_widget_show_all(body);
}



/* _cleanup:
 */
static void _cleanup ( MbtkInfoPage *page )
{
  MBTK_INFO_PAGE_CLASS(parent_class)->cleanup(page);
}

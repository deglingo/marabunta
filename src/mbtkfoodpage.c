/* mbtkfoodpage.c -
 */

#include "mbtkfoodpage.h"
#include "mbtktaskview.h"
#include "mbtkfoodpage.inl"



/* Private:
 */
typedef struct _Private
{
  int dummy;
}
  Private;

#define PRIVATE(page) ((Private *)(MBTK_FOOD_PAGE(page)->private))



static void _setup ( MbtkInfoPage *page );
static void _cleanup ( MbtkInfoPage *page );



/* mbtk_food_page_class_init:
 */
static void mbtk_food_page_class_init ( LObjectClass *cls )
{
  MBTK_INFO_PAGE_CLASS(cls)->setup = _setup;
  MBTK_INFO_PAGE_CLASS(cls)->cleanup = _cleanup;
}



/* mbtk_food_page_init:
 */
static void mbtk_food_page_init ( LObject *obj )
{
  MBTK_FOOD_PAGE(obj)->private = g_new0(Private, 1);
}



/* mbtk_food_page_new:
 */
AltkWidget *mbtk_food_page_new ( void )
{
  return ALTK_WIDGET(l_object_new_give(MBTK_CLASS_FOOD_PAGE,
                                       "title", l_string_new("Food"),
                                       NULL));
}



/* _setup:
 */
static void _setup ( MbtkInfoPage *page )
{
  AltkWidget *body, *box, *view;
  MbSector *sector;
  MBTK_INFO_PAGE_CLASS(parent_class)->setup(page);
  body = mbtk_info_page_get_body(page);
  sector = mbtk_info_page_get_sector(page);
  if (!(sector && sector->colony))
    return;
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(body, box);
  view = L_TRASH_OBJECT
    (mbtk_task_view_new(sector->colony->t_farm));
  ALTK_BOX_ADD(box, view, 0);
  view = L_TRASH_OBJECT
    (mbtk_task_view_new(sector->colony->t_food));
  ALTK_BOX_ADD(box, view, 0);
}



/* _cleanup:
 */
static void _cleanup ( MbtkInfoPage *page )
{
  MBTK_INFO_PAGE_CLASS(parent_class)->setup(page);
}

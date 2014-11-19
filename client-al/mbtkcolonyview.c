/* mbtkcolonyview.c -
 */

#include "client-al/mbtkcolonyview.h"
#include "client-al/mbtkcolonyview.inl"



/* Private:
 */
typedef struct _Private
{
  int dummy;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_COLONY_VIEW(view)->private))



static void size_request ( AltkWidget *widget,
                           AltkRequisition *req );



/* mbtk_colony_view_class_init:
 */
static void mbtk_colony_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = size_request;
}



/* mbtk_colony_view_init:
 */
static void mbtk_colony_view_init ( LObject *obj )
{
  MBTK_COLONY_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
}



/* mbtk_colony_view_new:
 */
AltkWidget *mbtk_colony_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_COLONY_VIEW, NULL));
  return view;
}



/* size_request:
 */
static void size_request ( AltkWidget *widget,
                           AltkRequisition *req )
{
  req->width = 8;
  req->height = 8;
}

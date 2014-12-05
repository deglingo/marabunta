/* mbtksectorview.c -
 */

#include "mbtksectorview.h"
#include "mbtksectorview.inl"



/* Private:
 */
typedef struct _Private
{
  MbSector *sector;
}
  Private;

#define PRIVATE(view) ((Private *)(MBTK_SECTOR_VIEW(view)->private))



static void _size_request ( AltkWidget *widget,
                            AltkRequisition *req );
static void _expose_event ( AltkWidget *widget,
                            AltkEvent *event );



/* mbtk_sector_view_class_init:
 */
static void mbtk_sector_view_class_init ( LObjectClass *cls )
{
  ALTK_WIDGET_CLASS(cls)->size_request = _size_request;
  ALTK_WIDGET_CLASS(cls)->expose_event = _expose_event;
}



/* mbtk_sector_view_init:
 */
static void mbtk_sector_view_init ( LObject *obj )
{
  MBTK_SECTOR_VIEW(obj)->private = g_new0(Private, 1);
  ALTK_WIDGET(obj)->flags |= ALTK_WIDGET_FLAG_NOWINDOW;
  altk_widget_set_event_mask(ALTK_WIDGET(obj), ALTK_EVENT_MASK_EXPOSE);
}



/* mbtk_sector_view_new:
 */
AltkWidget *mbtk_sector_view_new ( void )
{
  AltkWidget *view;
  view = ALTK_WIDGET(l_object_new(MBTK_CLASS_SECTOR_VIEW, NULL));
  return view;
}



/* mbtk_sector_view_set_sector:
 */
void mbtk_sector_view_set_sector ( MbtkSectorView *view,
                                   MbSector *sector )
{
  Private *priv = PRIVATE(view);
  if (sector == priv->sector)
    return;
  ASSERT(!priv->sector); /* [TODO] */
  priv->sector = l_object_ref(sector);
}



/* _size_request:
 */
static void _size_request ( AltkWidget *widget,
                            AltkRequisition *req )
{
  req->width = req->height = 8;
}



/* _expose_event:
 */
static void _expose_event ( AltkWidget *widget,
                            AltkEvent *event )
{
  altk_gc_set_color_hrgb(event->expose.gc, 0x00FF00);
  altk_gc_clear_region(event->expose.gc, event->expose.area);
}

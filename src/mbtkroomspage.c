/* mbtkroomspage.c -
 */

#include "mbtkroomspage.h"
#include "mbtktaskview.h"
#include "mbtkroomspage.inl"



/* RoomData:
 */
typedef struct _RoomData
{
  MbRoom *room;
  AltkWidget *task_view;
}
  RoomData;



/* Private:
 */
typedef struct _Private
{
  RoomData *rooms;
  gint n_rooms;
  gint max_rooms;
}
  Private;

#define PRIVATE(page) ((Private *)(MBTK_ROOMS_PAGE(page)->private))



static void _setup ( MbtkInfoPage *page );
static void _cleanup ( MbtkInfoPage *page );



/* mbtk_rooms_page_class_init:
 */
static void mbtk_rooms_page_class_init ( LObjectClass *cls )
{
  MBTK_INFO_PAGE_CLASS(cls)->setup = _setup;
  MBTK_INFO_PAGE_CLASS(cls)->cleanup = _cleanup;
}



/* mbtk_rooms_page_init:
 */
static void mbtk_rooms_page_init ( LObject *obj )
{
  MBTK_ROOMS_PAGE(obj)->private = g_new0(Private, 1);
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



/* _setup_rooms:
 */
static void _setup_rooms ( MbtkInfoPage *page,
                           MbSector *sector,
                           AltkWidget *box )
{
  Private *priv = PRIVATE(page);
  gint tp, max_tp;
  MbGame *game = sector->world->game;
  max_tp = mb_game_max_room_type(game);
  for (tp = 1; tp <= max_tp; tp++)
    {
      RoomData *data;
      AltkWidget *room_box, *name, *build_but;
      const MbRoomTypeInfo *info;
      ASSERT(priv->n_rooms <= priv->max_rooms);
      info = mb_game_room_type_info(game, tp);
      if (priv->n_rooms == priv->max_rooms)
        {
          priv->max_rooms += 8;
          priv->rooms = g_realloc(priv->rooms, sizeof(RoomData) * priv->max_rooms);
        }
      data = &priv->rooms[priv->n_rooms++];
      data->room = mb_colony_get_room(sector->colony, tp);
      room_box = L_TRASH_OBJECT
        (altk_box_new(ALTK_HORIZONTAL));
      ALTK_BOX_ADD(box, room_box, 0);
      name = L_TRASH_OBJECT
        (altk_label_new(info->name));
      ALTK_BOX_ADD(room_box, name, 0);
      if (data->room)
        {
          MbTask *t_build = data->room->t_build;
          l_object_ref(data->room);
          data->task_view = L_TRASH_OBJECT
            (mbtk_task_view_new(t_build));
          ALTK_BOX_ADD(room_box, data->task_view, 0);
        }
      else
        {
        }
      /* build button */
      build_but = L_TRASH_OBJECT
        (altk_button_new_with_label("+"));
      ALTK_BOX_ADD(room_box, build_but, 0);
    }
}



/* _setup:
 */
static void _setup ( MbtkInfoPage *page )
{
  /* Private *priv = PRIVATE(page); */
  MbSector *sector;
  AltkWidget *body, *box, *task_view;
  MBTK_INFO_PAGE_CLASS(parent_class)->setup(page);
  sector = mbtk_info_page_get_sector(page);
  CL_TRACE("%p, %p", page, sector);
  if (!(sector && sector->colony))
    return;
  body = mbtk_info_page_get_body(page);
  box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(body, box);
  /* top build task */
  task_view = L_TRASH_OBJECT
    (mbtk_task_view_new(sector->colony->t_build));
  ALTK_BOX_ADD(box, task_view, 0);
  /* room build tasks */
  _setup_rooms(page, sector, box);
  /* */
  altk_widget_show_all(body);
}



/* _cleanup:
 */
static void _cleanup ( MbtkInfoPage *page )
{
  Private *priv = PRIVATE(page);
  gint n;
  for (n = 0; n < priv->n_rooms; n++)
    {
      RoomData *data = &priv->rooms[n];
      if (data->room)
        L_OBJECT_CLEAR(data->room);
    }
  priv->n_rooms = 0;
  MBTK_INFO_PAGE_CLASS(parent_class)->cleanup(page);
}

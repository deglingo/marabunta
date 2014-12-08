/* mbtkroomspage.c -
 */

#include "mbtkroomspage.h"
#include "mbtktaskview.h"
#include "mbtkroomspage.inl"



/* RoomData:
 */
typedef struct _RoomData
{
  AltkWidget *page;
  MbRoomType room_type;
  MbRoom *room;
  AltkWidget *task_view;
}
  RoomData;



/* Private:
 */
typedef struct _Private
{
  LSignalHandlerGroup *sig_group;
  RoomData *rooms;
  gint n_rooms;
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
  PRIVATE(obj)->sig_group = l_signal_handler_group_new();
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



/* _on_build_button_clicked:
 */
static void _on_build_button_clicked ( AltkWidget *button,
                                       RoomData *data )
{
  MbSector *sector = mbtk_info_page_get_sector(MBTK_INFO_PAGE(data->page));
  if (sector && sector->colony)
    {
      MbGame *game = sector->world->game;
      mb_game_request_build_room(game, sector->colony, data->room_type);
    }
}



/* _get_room_data:
 */
static RoomData *_get_room_data ( MbtkRoomsPage *page,
                                  MbRoomType type )
{
  Private *priv = PRIVATE(page);
  if (type > 0 && type < priv->n_rooms)
    {
      RoomData *data = &priv->rooms[type];
      ASSERT(data->room_type == 0 || data->room_type == type);
      if (data->room_type)
        return data;
    }
  return NULL;
}



/* _room_data_set_room:
 */
static void _room_data_set_room ( RoomData *data,
                                  MbRoom *room )
{
  MbTask *t_build;
  ASSERT(!data->room); /* [todo] ? */
  if (room)
    {
      data->room = l_object_ref(room);
      t_build = data->room->t_build;
      mbtk_task_view_set_task(MBTK_TASK_VIEW(data->task_view), t_build);
      altk_widget_set_enable_show_all(data->task_view, TRUE);
      altk_widget_show(data->task_view);
    }
  else
    {
      altk_widget_set_enable_show_all(data->task_view, FALSE);
      altk_widget_hide(data->task_view);
    }
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
  priv->n_rooms = max_tp + 1;
  priv->rooms = g_malloc0(sizeof(RoomData) * priv->n_rooms);
  for (tp = 1; tp <= max_tp; tp++)
    {
      RoomData *data;
      MbRoom *room;
      AltkWidget *room_box, *name, *build_but;
      const MbRoomTypeInfo *info;
      info = mb_game_room_type_info(game, tp);
      data = &priv->rooms[tp];
      data->page = ALTK_WIDGET(page);
      data->room_type = tp;
      room = mb_colony_get_room(sector->colony, tp);
      room_box = L_TRASH_OBJECT
        (altk_box_new(ALTK_HORIZONTAL));
      ALTK_BOX_ADD(box, room_box, 0);
      name = L_TRASH_OBJECT
        (altk_label_new(info->name));
      ALTK_BOX_ADD(room_box, name, 0);
      data->task_view = L_TRASH_OBJECT
        (mbtk_task_view_new(NULL));
      ALTK_BOX_ADD(room_box, data->task_view, 0);
      /* build button */
      build_but = L_TRASH_OBJECT
        (altk_button_new_with_label("+"));
      ALTK_BOX_ADD(room_box, build_but, 0);
      l_signal_handler_group_add
        (priv->sig_group,
         l_signal_connect(L_OBJECT(build_but),
                          "clicked",
                          (LSignalHandler) _on_build_button_clicked,
                          data,
                          NULL));
      _room_data_set_room(data, room);
    }
}



/* _on_room_added:
 */
static void _on_room_added ( MbColony *colony,
                             MbRoom *room,
                             AltkWidget *page )
{
  RoomData *data;
  CL_DEBUG("room added: %d", room->type);
  data = _get_room_data(MBTK_ROOMS_PAGE(page), room->type);
  ASSERT(data);
  ASSERT(!data->room);
  _room_data_set_room(data, room);
}



/* _setup:
 */
static void _setup ( MbtkInfoPage *page )
{
  Private *priv = PRIVATE(page);
  MbSector *sector;
  AltkWidget *body, *box, *task_view;
  MBTK_INFO_PAGE_CLASS(parent_class)->setup(page);
  sector = mbtk_info_page_get_sector(page);
  CL_TRACE("%p, %p", page, sector);
  if (!(sector && sector->colony))
    return;
  body = mbtk_info_page_get_body(page);
  l_signal_handler_group_add
    (priv->sig_group,
     l_signal_connect(L_OBJECT(sector->colony),
                      "add_room",
                      (LSignalHandler) _on_room_added,
                      page,
                      NULL));
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
  l_signal_handler_group_remove_all(priv->sig_group);
  for (n = 0; n < priv->n_rooms; n++)
    {
      RoomData *data = &priv->rooms[n];
      if (data->room)
        L_OBJECT_CLEAR(data->room);
    }
  priv->n_rooms = 0;
  g_free(priv->rooms);
  MBTK_INFO_PAGE_CLASS(parent_class)->cleanup(page);
}

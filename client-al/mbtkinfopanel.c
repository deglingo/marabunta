/* mbtkinfopanel.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbtkinfopanel.h"
#include "client-al/mbtktaskview.h"
#include "client-al/mbtkinfopanel.inl"



/* Page:
 */
typedef struct _Page Page;
typedef void (* PageCreateFunc) ( Page *page );
typedef void (* PageInitFunc) ( Page *page );
typedef void (* PageCleanupFunc) ( Page *page );

struct _Page
{
  gchar *name;
  PageCreateFunc create;
  PageInitFunc init;
  PageCleanupFunc cleanup;
  guint created : 1;
  guint initialized : 1;
  AltkWidget *panel;
  AltkWidget *top;
  AltkWidget *body;
};

#define PAGE(p) ((Page *)(p))



/* FoodPage:
 */
typedef struct _FoodPage
{
  Page page;
  AltkWidget *top_box;
  GSList *widgets;
}
  FoodPage;

#define FOOD_PAGE(p) ((FoodPage *)(p))



/* MinePage:
 */
typedef struct _MinePage
{
  Page page;
  AltkWidget *top_box;
  GSList *widgets;
}
  MinePage;

#define MINE_PAGE(p) ((MinePage *)(p))



/* RoomData:
 */
typedef struct _RoomData
{
  Page *page;
  gint type;
  MbObject *room;
}
  RoomData;



/* RoomsPage:
 */
typedef struct _RoomsPage
{
  Page page;
  /* AltkWidget *top_box; */
  /* GSList *widgets; */
  /* RoomData room_data[MB_ROOM_TYPE_COUNT]; */
}
  RoomsPage;

#define ROOMS_PAGE(p) ((RoomsPage *)(p))



/* Private:
 */
typedef struct _Private
{
  MbObject *colony;
  GList *pages;
  Page *current_page;
  AltkWidget *button_box;
  AltkWidget *pages_box;
}
  Private;

#define PRIVATE(panel) ((Private *)(MBTK_INFO_PANEL(panel)->private))



/* mbtk_info_panel_init:
 */
static void mbtk_info_panel_init ( LObject *obj )
{
  MBTK_INFO_PANEL(obj)->private = g_new0(Private, 1);
}



/* page_create:
 */
static void page_create ( Page *page )
{
  if (!page->created)
    {
      page->created = 1;
      l_trash_push();
      page->create(page);
      l_trash_pop();
    }
}



/* page_init:
 */
static void page_init ( Page *page )
{
  page_create(page);
  if (!page->initialized)
    {
      page->initialized = 1;
      l_trash_push();
      page->init(page);
      l_trash_pop();
    }
}



/* page_cleanup:
 */
static void page_cleanup ( Page *page )
{
  if (page->initialized)
    {
      page->initialized = 0;
      l_trash_push();
      page->cleanup(page);
      l_trash_pop();
    }
}



/* _set_page:
 */
static void _set_page ( MbtkInfoPanel *panel,
                        Page *page )
{
  Private *priv = PRIVATE(panel);
  if (priv->current_page == page)
    return;
  if (priv->current_page)
    altk_widget_hide(priv->current_page->top);
  if ((priv->current_page = page))
    {
      page_init(page);
      altk_widget_show(page->top);
    }
}



/* _on_page_button_clicked:
 */
static void _on_page_button_clicked ( AltkWidget *button,
                                      Page *page )
{
  _set_page(MBTK_INFO_PANEL(page->panel), page);
}



/* _create_page:
 */
static Page *_create_page ( AltkWidget *panel,
                            gint struct_size,
                            const gchar *name,
                            const gchar *title,
                            const gchar *but_label,
                            PageCreateFunc create,
                            PageInitFunc init,
                            PageCleanupFunc cleanup )
{
  Private *priv = PRIVATE(panel);
  ASSERT(struct_size >= sizeof(Page));
  Page *page = g_malloc0(struct_size);
  AltkWidget *but, *top_box, *title_label;
  priv->pages = g_list_append(priv->pages, page);
  page->panel = panel;
  page->name = g_strdup(name);
  page->create = create;
  page->init = init;
  page->cleanup = cleanup;
  /* button */
  but = L_TRASH_OBJECT
    (altk_button_new_with_label(but_label));
  ALTK_BOX_ADD(priv->button_box, but, 0);
  l_signal_connect(L_OBJECT(but),
                   "clicked",
                   (LSignalHandler) _on_page_button_clicked,
                   page,
                   NULL);
  /* top frame */
  page->top = L_TRASH_OBJECT
    (altk_frame_new(""));
  altk_widget_set_enable_show_all(page->top, FALSE);
  ALTK_BOX_ADD(priv->pages_box, page->top, ALTK_PACK_EXPAND_FILL);
  /* top box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page->top, top_box);
  /* title */
  title_label = L_TRASH_OBJECT
    (altk_label_new(title));
  ALTK_BOX_ADD(top_box, title_label, 0);
  /* body */
  page->body = L_TRASH_OBJECT
    (altk_frame_new(""));
  ALTK_BOX_ADD(top_box, page->body, ALTK_PACK_EXPAND_FILL);
  /* ok */
  return page;
}



/* _rooms_page_create:
 */
static void _rooms_page_create ( Page *page )
{
  /* ROOMS_PAGE(page)->top_box = L_TRASH_OBJECT */
  /*   (altk_box_new(ALTK_VERTICAL)); */
  /* ALTK_CONTAINER_ADD(page->body, ROOMS_PAGE(page)->top_box); */
}



/* static void _on_build_button_clicked ( AltkWidget *button, */
/*                                        RoomData *data ) */
/* { */
/*   CL_DEBUG("[TODO] build room type %d", data->type); */
/* } */



/* /\* _create_room_dialog: */
/*  *\/ */
/* static AltkWidget *_create_room_dialog ( RoomData *data ) */
/* { */
/*   gchar text[64]; */
/*   AltkWidget *box, *label, *build_but; */
/*   sprintf(text, "TP %d", data->type); */
/*   box = altk_box_new(ALTK_HORIZONTAL); */
/*   label = L_TRASH_OBJECT */
/*     (altk_label_new(text)); */
/*   ALTK_BOX_ADD(box, label, 0); */
/*   build_but = L_TRASH_OBJECT */
/*     (altk_button_new_with_label("+")); */
/*   ALTK_BOX_ADD(box, build_but, 0); */
/*   l_signal_connect(L_OBJECT(build_but), */
/*                    "clicked", */
/*                    (LSignalHandler) _on_build_button_clicked, */
/*                    data, */
/*                    NULL); */
/*   return box; */
/* } */



/* _rooms_page_init:
 */
static void _rooms_page_init ( Page *page )
{
  /* Private *priv = PRIVATE(page->panel); */
  /* if (priv->colony) */
  /*   { */
  /*     gint tp; */
  /*     for (tp = 0; tp < MB_ROOM_TYPE_COUNT; tp++) */
  /*       { */
  /*         MbObject *room = MB_COLONY_ROOM(priv->colony, tp); */
  /*         RoomData *data = &ROOMS_PAGE(page)->room_data[tp]; */
  /*         data->page = page; */
  /*         data->type = tp; */
  /*         data->room = room; /\* [fixme] ref ? *\/ */
  /*         AltkWidget *wid = L_TRASH_OBJECT */
  /*           (_create_room_dialog(data)); */
  /*         ALTK_BOX_ADD(ROOMS_PAGE(page)->top_box, wid, ALTK_PACK_HEXPAND_FILL); */
  /*         ROOMS_PAGE(page)->widgets = g_slist_prepend(ROOMS_PAGE(page)->widgets, wid); */
  /*       } */
  /*   } */
  /* altk_widget_show_all(ROOMS_PAGE(page)->top_box); */
}



/* _rooms_page_cleanup:
 */
static void _rooms_page_cleanup ( Page *page )
{
  /* g_slist_free_full(ROOMS_PAGE(page)->widgets, (GDestroyNotify) altk_widget_destroy); */
  /* ROOMS_PAGE(page)->widgets = NULL; */
}



/* _food_page_create:
 */
static void _food_page_create ( Page *page )
{
  FOOD_PAGE(page)->top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page->body, FOOD_PAGE(page)->top_box);
}



/* _food_page_init:
 */
static void _food_page_init ( Page *page )
{
  Private *priv = PRIVATE(page->panel);
  MbObject *task;
  AltkWidget *tview;
  GList *l;
  if (!priv->colony)
    return;
  /* top task */
  task = mb_task_find(MB_TASK(MB_COLONY_TOP_TASK(priv->colony)), "work/farm");
  ASSERT(task);
  tview = L_TRASH_OBJECT
    (mbtk_task_view_new(ALTK_HORIZONTAL, task));
  ALTK_BOX_ADD(FOOD_PAGE(page)->top_box, tview, 0);
  FOOD_PAGE(page)->widgets = g_slist_prepend(FOOD_PAGE(page)->widgets, tview);
  /* sub tasks */
  for (l = MB_TASK_CHILDREN(task); l; l = l->next)
    {
      task = l->data;
      tview = L_TRASH_OBJECT
        (mbtk_task_view_new(ALTK_HORIZONTAL, task));
      ALTK_BOX_ADD(FOOD_PAGE(page)->top_box, tview, 0);
      FOOD_PAGE(page)->widgets = g_slist_prepend(FOOD_PAGE(page)->widgets, tview);
    }
  altk_widget_show_all(page->body);
}



/* _food_page_cleanup:
 */
static void _food_page_cleanup ( Page *page )
{
  GSList *l;
  for (l = FOOD_PAGE(page)->widgets; l; l = l->next)
    altk_widget_destroy(l->data);
  g_slist_free(FOOD_PAGE(page)->widgets);
  FOOD_PAGE(page)->widgets = NULL;
}



/* _mine_page_create:
 */
static void _mine_page_create ( Page *page )
{
  MINE_PAGE(page)->top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page->body, MINE_PAGE(page)->top_box);
}



/* _mine_page_init:
 */
static void _mine_page_init ( Page *page )
{
  Private *priv = PRIVATE(page->panel);
  MbObject *task;
  AltkWidget *tview;
  GList *l;
  if (!priv->colony)
    return;
  /* top task */
  task = mb_task_find(MB_TASK(MB_COLONY_TOP_TASK(priv->colony)), "work/mine");
  ASSERT(task);
  tview = L_TRASH_OBJECT
    (mbtk_task_view_new(ALTK_HORIZONTAL, task));
  ALTK_BOX_ADD(MINE_PAGE(page)->top_box, tview, 0);
  MINE_PAGE(page)->widgets = g_slist_prepend(MINE_PAGE(page)->widgets, tview);
  /* sub tasks */
  for (l = MB_TASK_CHILDREN(task); l; l = l->next)
    {
      task = l->data;
      tview = L_TRASH_OBJECT
        (mbtk_task_view_new(ALTK_HORIZONTAL, task));
      ALTK_BOX_ADD(MINE_PAGE(page)->top_box, tview, 0);
      MINE_PAGE(page)->widgets = g_slist_prepend(MINE_PAGE(page)->widgets, tview);
    }
  altk_widget_show_all(page->body);
}



/* _mine_page_cleanup:
 */
static void _mine_page_cleanup ( Page *page )
{
  GSList *l;
  for (l = MINE_PAGE(page)->widgets; l; l = l->next)
    altk_widget_destroy(l->data);
  g_slist_free(MINE_PAGE(page)->widgets);
  MINE_PAGE(page)->widgets = NULL;
}



/* _create_panel:
 */
static void _create_panel ( AltkWidget *panel )
{
  Private *priv = PRIVATE(panel);
  AltkWidget *top_box;
  /* top_box */
  top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_CONTAINER_ADD(panel, top_box);
  /* page box */
  priv->pages_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_HORIZONTAL));
  ALTK_BOX_ADD(top_box, priv->pages_box, ALTK_PACK_EXPAND_FILL);
  /* button_box */
  priv->button_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_BOX_ADD(top_box, priv->button_box, ALTK_PACK_ANCHOR_TOP);
  /* create the pages */
  _create_page(panel,
               sizeof(RoomsPage),
               "rooms",
               "Rooms",
               "R",
               _rooms_page_create,
               _rooms_page_init,
               _rooms_page_cleanup);
  _create_page(panel,
               sizeof(FoodPage),
               "food",
               "Food",
               "F",
               _food_page_create,
               _food_page_init,
               _food_page_cleanup);
  _create_page(panel,
               sizeof(MinePage),
               "mine",
               "Mine",
               "M",
               _mine_page_create,
               _mine_page_init,
               _mine_page_cleanup);
  /* open the first page */
  _set_page(MBTK_INFO_PANEL(panel), priv->pages->data);
}



/* mbtk_info_panel_new:
 */
AltkWidget *mbtk_info_panel_new ( void )
{
  AltkWidget *panel;
  panel = ALTK_WIDGET(l_object_new(MBTK_CLASS_INFO_PANEL, NULL));
  l_trash_push();
  _create_panel(panel);
  l_trash_pop();
  return panel;
}



/* mbtk_info_panel_set_colony:
 */
void mbtk_info_panel_set_colony ( MbtkInfoPanel *panel,
                                  MbObject *colony )
{
  Private *priv = PRIVATE(panel);
  GList *l;
  if (colony == priv->colony)
    return;
  for (l = priv->pages; l; l = l->next)
    page_cleanup(l->data);
  if (priv->colony)
    L_OBJECT_CLEAR(priv->colony);
  if (colony)
    priv->colony = l_object_ref(colony);
  if (priv->current_page)
    page_init(priv->current_page);
}
/* mbtkinfopage.c -
 */

#include "mbtkinfopage.h"
#include "mbtkinfopage.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_TITLE,
    N_PROPS,
  };

static LParamSpec *pspecs[N_PROPS];



/* Private:
 */
typedef struct _Private
{
  gchar *title;
  MbSector *sector;
  guint setup : 1;
  AltkWidget *top_box;
  AltkWidget *body;
}
  Private;

#define PRIVATE(p) ((Private *)(MBTK_INFO_PAGE(p)->private))



static void _set_property ( LObject *obj,
                            LParamSpec *pspec,
                            LObject *value );
static LObject *_get_property ( LObject *obj,
                                LParamSpec *pspec );
static void _setup ( MbtkInfoPage *page );
static void _cleanup ( MbtkInfoPage *page );



/* mbtk_info_page_class_init:
 */
static void mbtk_info_page_class_init ( LObjectClass *cls )
{
  cls->set_property = _set_property;
  cls->get_property = _get_property;
  MBTK_INFO_PAGE_CLASS(cls)->setup = _setup;
  MBTK_INFO_PAGE_CLASS(cls)->cleanup = _cleanup;

  pspecs[PROP_TITLE] =
    l_param_spec_string("title",
                        "");

  l_object_class_install_properties(cls, N_PROPS, pspecs);
}



/* mbtk_info_page_init:
 */
static void mbtk_info_page_init ( LObject *obj )
{
  MBTK_INFO_PAGE(obj)->private = g_new0(Private, 1);
  PRIVATE(obj)->title = g_strdup("");
}



/* _set_property:
 */
static void _set_property ( LObject *obj,
                            LParamSpec *pspec,
                            LObject *value )
{
  switch (pspec->param_id)
    {
    case PROP_TITLE:
      g_free(PRIVATE(obj)->title);
      PRIVATE(obj)->title = g_strdup(L_STRING(value)->str);
      break;
    default:
      L_OBJECT_SET_PROPERTY_ERROR(obj, pspec);
    }
}



/* _get_property:
 */
static LObject *_get_property ( LObject *obj,
                                LParamSpec *pspec )
{
  L_OBJECT_GET_PROPERTY_ERROR(obj, pspec);
}



/* mbtk_info_page_set_sector:
 */
void mbtk_info_page_set_sector ( MbtkInfoPage *page,
                                 MbSector *sector )
{
  Private *priv = PRIVATE(page);
  /* CL_TRACE("%p, %p, %p", page, sector, (sector ? sector->colony : NULL)); */
  if (sector == priv->sector)
    return;
  ASSERT(!priv->sector); /* [todo] */
  mbtk_info_page_cleanup(page);
  if ((priv->sector = sector))
    l_object_ref(priv->sector);
}



/* mbtk_info_page_get_sector:
 */
MbSector *mbtk_info_page_get_sector ( MbtkInfoPage *page )
{
  return PRIVATE(page)->sector;
}



/* mbtk_info_page_get_body:
 */
AltkWidget *mbtk_info_page_get_body ( MbtkInfoPage *page )
{
  return PRIVATE(page)->body;
}



/* mbtk_info_page_setup:
 */
void mbtk_info_page_setup ( MbtkInfoPage *page )
{
  Private *priv = PRIVATE(page);
  if (!priv->setup)
    {
      priv->setup = 1;
      l_trash_push();
      MBTK_INFO_PAGE_GET_CLASS(page)->setup(page);
      l_trash_pop();
      altk_widget_show_all(priv->top_box);
    }
}



/* mbtk_info_page_cleanup:
 */
void mbtk_info_page_cleanup ( MbtkInfoPage *page )
{
  Private *priv = PRIVATE(page);
  if (priv->setup)
    {
      priv->setup = 0;
      l_trash_push();
      MBTK_INFO_PAGE_GET_CLASS(page)->cleanup(page);
      l_trash_pop();
    }
}



/* _setup:
 */
static void _setup ( MbtkInfoPage *page )
{
  Private *priv = PRIVATE(page);
  AltkWidget *title;
  ASSERT(!priv->top_box);
  priv->top_box = L_TRASH_OBJECT
    (altk_box_new(ALTK_VERTICAL));
  ALTK_CONTAINER_ADD(page, priv->top_box);
  title = L_TRASH_OBJECT
    (altk_label_new(priv->title));
  ALTK_BOX_ADD(priv->top_box, title, 0);
  priv->body = L_TRASH_OBJECT
    (altk_frame_new(""));
  ALTK_BOX_ADD(priv->top_box, priv->body, ALTK_PACK_EXPAND_FILL);
}



/* _cleanup:
 */
static void _cleanup ( MbtkInfoPage *page )
{
  Private *priv = PRIVATE(page);
  ASSERT(priv->top_box);
  altk_widget_destroy(priv->top_box);
  priv->top_box = NULL;
}

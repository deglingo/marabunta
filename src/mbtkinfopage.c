/* mbtkinfopage.c -
 */

#include "mbtkinfopage.h"
#include "mbtkinfopage.inl"



/* Private:
 */
typedef struct _Private
{
  MbSector *sector;
  guint setup : 1;
  AltkWidget *top_box;
  AltkWidget *body;
}
  Private;

#define PRIVATE(p) ((Private *)(MBTK_INFO_PAGE(p)->private))



static void _setup ( MbtkInfoPage *page );
static void _cleanup ( MbtkInfoPage *page );



/* mbtk_info_page_class_init:
 */
static void mbtk_info_page_class_init ( LObjectClass *cls )
{
  MBTK_INFO_PAGE_CLASS(cls)->setup = _setup;
  MBTK_INFO_PAGE_CLASS(cls)->cleanup = _cleanup;
}



/* mbtk_info_page_init:
 */
static void mbtk_info_page_init ( LObject *obj )
{
  MBTK_INFO_PAGE(obj)->private = g_new0(Private, 1);
}



/* mbtk_info_page_set_sector:
 */
void mbtk_info_page_set_sector ( MbtkInfoPage *page,
                                 MbSector *sector )
{
  Private *priv = PRIVATE(page);
  if (sector == priv->sector)
    return;
  ASSERT(!priv->sector); /* [todo] */
  if ((priv->sector = sector))
    l_object_ref(priv->sector);
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
    (altk_label_new("TITLE"));
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
  L_OBJECT_CLEAR(priv->top_box);
}

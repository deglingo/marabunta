/* mbtkinfopage.c -
 */

#include "mbtkinfopage.h"
#include "mbtkinfopage.inl"



/* Private:
 */
typedef struct _Private
{
  MbSector *sector;
}
  Private;

#define PRIVATE(p) ((Private *)(MBTK_INFO_PAGE(p)->private))



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

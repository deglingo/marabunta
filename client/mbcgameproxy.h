/* mbcgameproxy.h -
 */

#ifndef _MBCGAMEPROXY_H_
#define _MBCGAMEPROXY_H_

#include "client/mbcproxy.h"
#include "client/mbcgameproxy-def.h"

G_BEGIN_DECLS



/* MbcGameProxy:
 */
struct _MbcGameProxy
{
  MBC_GAME_PROXY_INSTANCE_HEADER;

  guint sim_time;
  GHashTable *proxy_map;
  /* world proxy */
  MbcProxy *world;
};



/* MbcGameProxyClass:
 */
struct _MbcGameProxyClass
{
  MBC_GAME_PROXY_CLASS_HEADER;
};



MbcGameProxy *mbc_game_proxy_new ( guint id );
MbcProxy *mbc_game_proxy_create_object ( MbcGameProxy *game,
                                         LObjectClass *cls,
                                         guint id );
MbcProxy *mbc_game_proxy_lookup_object ( MbcGameProxy *proxy,
                                         guint id );
void mbc_game_proxy_create_world ( MbcGameProxy *game,
                                   guint id,
                                   guint width,
                                   guint height );
void mbc_game_proxy_create_sector ( MbcGameProxy *game,
                                    guint id,
                                    guint x,
                                    guint y );
void mbc_game_proxy_create_colony ( MbcGameProxy *game,
                                    guint id,
                                    guint sector_id,
                                    guint owner );
void mbc_game_proxy_create_task ( MbcGameProxy *game,
                                  guint task_id,
                                  guint colony_id,
                                  guint parent_id,
                                  gboolean group,
                                  const gchar *name,
                                  gint64 workers );
void mbc_game_proxy_reset ( MbcGameProxy *proxy );
void mbc_game_proxy_set_sim_time ( MbcGameProxy *proxy,
                                   guint sim_time );
/* void mbc_game_proxy_started ( MbcGameProxy *proxy ); */



G_END_DECLS

#endif /* ifndef _MBCGAMEPROXY_H_ */

/* mbcgameproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgameproxy.h"
#include "client/mbcworldproxy.h"
#include "client/mbccolonyproxy.h"
#include "client/mbcgameproxy.inl"



/* properties:
 */
enum
  {
    PROP_0,
    PROP_SIM_TIME,
    PROP_COUNT,
  };

static LParamSpec *pspecs[PROP_COUNT] = { NULL, };



/* /\* Signals: */
/*  *\/ */
/* enum */
/*   { */
/*     SIG_STARTED, */
/*     SIG_COUNT, */
/*   }; */

/* static LSignalID signals[SIG_COUNT] = { 0, }; */



/* mbc_game_proxy_class_init:
 */
static void mbc_game_proxy_class_init ( LObjectClass *cls )
{
  pspecs[PROP_SIM_TIME] =
    l_param_spec_int("sim_time", 0);

  l_object_class_install_properties(cls, PROP_COUNT, pspecs);

  /* signals[SIG_STARTED] = */
  /*   l_signal_new(cls, */
  /*                "started"); */
}



/* mbc_game_proxy_new:
 */
MbcGameProxy *mbc_game_proxy_new ( guint id )
{
  MbcGameProxy *gp;
  gp = MBC_GAME_PROXY(mbc_proxy_new(MBC_CLASS_GAME_PROXY, id));
  gp->proxy_map = g_hash_table_new_full(NULL, NULL, NULL,
                                        (GDestroyNotify) l_object_unref);
  g_hash_table_insert(gp->proxy_map,
                      GUINT_TO_POINTER(id),
                      l_object_ref(gp)); /* [fixme] ref ? */
  return gp;
}



/* mbc_game_proxy_create_object:
 */
MbcProxy *mbc_game_proxy_create_object ( MbcGameProxy *game,
                                         LObjectClass *cls,
                                         guint id )
{
  MbcProxy *object;
  /* [TODO] ASSERT(MBC_IS_PROXY_CLASS(cls)); */
  ASSERT(id > 0);
  object = mbc_proxy_new(cls, id);
  object->game = MBC_PROXY(game);
  ASSERT(!mbc_game_proxy_lookup_object(game, object->id));
  g_hash_table_insert(game->proxy_map,
                      GUINT_TO_POINTER(object->id),
                      object);
  return object;
}



/* mbc_game_proxy_lookup_object:
 */
MbcProxy *mbc_game_proxy_lookup_object ( MbcGameProxy *proxy,
                                         guint id )
{
  return g_hash_table_lookup(proxy->proxy_map,
                             GUINT_TO_POINTER(id));
}



/* mbc_game_proxy_create_world:
 */
void mbc_game_proxy_create_world ( MbcGameProxy *game,
                                   guint id,
                                   guint width,
                                   guint height )
{
  ASSERT(!game->world);
  game->world = mbc_game_proxy_create_object(game, MBC_CLASS_WORLD_PROXY, id);
  mbc_world_proxy_set_size(MBC_WORLD_PROXY(game->world), width, height);
}



/* mbc_game_proxy_create_sector:
 */
void mbc_game_proxy_create_sector ( MbcGameProxy *game,
                                    guint id,
                                    guint x,
                                    guint y )
{
  MbcProxy *sector;
  ASSERT(game->world);
  sector = mbc_sector_proxy_new(MBC_PROXY(game), id, x, y);
  mbc_world_proxy_add_sector(MBC_WORLD_PROXY(game->world), sector);
}



/* mbc_game_proxy_create_colony:
 */
void mbc_game_proxy_create_colony ( MbcGameProxy *game,
                                    guint id,
                                    guint sector_id,
                                    guint owner )
{
  MbcProxy *sector, *colony;
  sector = mbc_game_proxy_lookup_object(game, sector_id);
  ASSERT(sector);
  ASSERT(MBC_IS_SECTOR_PROXY(sector));
  colony = mbc_colony_proxy_new(MBC_PROXY(game), id, owner);
  mbc_sector_proxy_add_colony(MBC_SECTOR_PROXY(sector), colony);
}



/* mbc_game_proxy_reset:
 */
void mbc_game_proxy_reset ( MbcGameProxy *proxy )
{
  /* CL_DEBUG("[TODO] reset"); */
  mbc_game_proxy_set_sim_time(proxy, 0);
}



/* mbc_game_proxy_set_sim_time:
 */
void mbc_game_proxy_set_sim_time ( MbcGameProxy *proxy,
                                   guint sim_time )
{
  proxy->sim_time = sim_time;
  l_object_notify(L_OBJECT(proxy), pspecs[PROP_SIM_TIME]);
}



/* /\* mbc_game_proxy_started: */
/*  *\/ */
/* void mbc_game_proxy_started ( MbcGameProxy *proxy ) */
/* { */
/*   l_signal_emit(L_OBJECT(proxy), signals[SIG_STARTED], 0); */
/* } */

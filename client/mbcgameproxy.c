/* mbcgameproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcgameproxy.h"
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
  gp->world = mbc_world_proxy_new();
  return gp;
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

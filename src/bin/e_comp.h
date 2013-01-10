#ifdef E_TYPEDEFS
typedef struct _E_Comp      E_Comp;
typedef struct _E_Comp_Win  E_Comp_Win;
typedef struct _E_Comp_Zone E_Comp_Zone;

#define E_COMP_ENGINE_SW 1
#define E_COMP_ENGINE_GL 2

#else
#ifndef E_MOD_COMP_H
#define E_MOD_COMP_H

EINTERN Eina_Bool e_comp_init(void);
EINTERN int      e_comp_shutdown(void);

EAPI int e_comp_internal_save(void);
EAPI E_Comp_Config *e_comp_config_get(void);
EAPI void e_comp_shadow_set(void);

#endif
#endif

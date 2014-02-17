#include "e.h"

static void _e_client_cb_border_menu_end(void *data, E_Menu *m);
static void _e_client_menu_cb_locks(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_remember(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_borderless(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_border(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_redirect_set(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi EINA_UNUSED);
static void _e_client_menu_cb_close(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_iconify(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_kill(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_move(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_resize(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_maximize_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_maximize(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_maximize_vertically(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_maximize_horizontally(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_maximize_left(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_maximize_right(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_unmaximize(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_shade(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_resistance(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_icon_edit(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_application_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_window_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi);
static void _e_client_menu_cb_prop(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_stick(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_stacking_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_on_top(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_normal(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_below(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_fullscreen(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_skip_winlist(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_skip_pager(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_skip_taskbar(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_sendto_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_sendto(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_pin(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_unpin(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_raise(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_lower(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_skip_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_fav_add(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_kbdshrtct_add(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_ibar_add_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_ibar_add(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_border_pre(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_iconpref_e(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_iconpref_netwm(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_iconpref_user(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_default_icon(void *data, E_Menu *m, E_Menu_Item *mi);
static void _e_client_menu_cb_netwm_icon(void *data, E_Menu *m, E_Menu_Item *mi);

static Eina_List *menu_hooks = NULL;

EAPI E_Client_Menu_Hook *
e_int_client_menu_hook_add(E_Client_Menu_Hook_Cb cb, const void *data)
{
   E_Client_Menu_Hook *h;

   if (!cb) return NULL;
   h = E_NEW(E_Client_Menu_Hook, 1);
   if (!h) return NULL;

   h->cb = cb;
   h->data = (void *)data;
   menu_hooks = eina_list_append(menu_hooks, h);
   return h;
}

EAPI void
e_int_client_menu_hook_del(E_Client_Menu_Hook *hook)
{
   E_Client_Menu_Hook *h;
   Eina_List *l;

   if (!hook) return;

   EINA_LIST_FOREACH(menu_hooks, l, h)
     if (h == hook)
       {
          menu_hooks = eina_list_remove_list(menu_hooks, l);
          free(h);
          return;
       }
}

EAPI void
e_int_client_menu_hooks_clear(void)
{
   E_Client_Menu_Hook *h;
   EINA_LIST_FREE(menu_hooks, h)
     free(h);
}

EAPI void
e_int_client_menu_create(E_Client *ec)
{
   E_Menu *m;
   E_Menu_Item *mi;
   Eina_List *l;
   E_Client_Menu_Hook *h;
   char buf[128];
   Eina_Bool borderless;

   if (ec->border_menu) return;

   m = e_menu_new();
   e_menu_category_set(m, "border");
   e_menu_category_data_set("border", ec);
   e_object_data_set(E_OBJECT(m), ec);
   ec->border_menu = m;
   e_menu_post_deactivate_callback_set(m, _e_client_cb_border_menu_end, NULL);

   if (!ec->internal)
     {
        if (ec->icccm.class)
          snprintf(buf, sizeof(buf), "%s", ec->icccm.class);
        else
          snprintf(buf, sizeof(buf), _("Application"));
        mi = e_menu_item_new(m);
        e_menu_item_label_set(mi, buf);
        e_menu_item_submenu_pre_callback_set(mi, _e_client_menu_cb_application_pre, ec);
        if (ec->desktop)
          e_util_desktop_menu_item_icon_add(ec->desktop, 16, mi);
     }
   borderless = e_client_util_borderless(ec);

   mi = e_menu_item_new(m);
   e_menu_item_label_set(mi, _("Window"));
   e_menu_item_submenu_pre_callback_set(mi, _e_client_menu_cb_window_pre, ec);
   e_menu_item_icon_edje_set(mi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/window"),
                             "e/widgets/border/default/window");

   mi = e_menu_item_new(m);
   e_menu_item_separator_set(mi, 1);

   if ((!ec->sticky) && ((ec->zone->desk_x_count > 1) || (ec->zone->desk_y_count > 1)))
     {
        mi = e_menu_item_new(m);
        e_menu_item_label_set(mi, _("Move to"));
        e_menu_item_submenu_pre_callback_set(mi, _e_client_menu_cb_sendto_pre, ec);
        e_menu_item_icon_edje_set(mi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/sendto"),
                                  "e/widgets/border/default/sendto");
     }

   mi = e_menu_item_new(m);
   e_menu_item_label_set(mi, _("Always on Top"));
   e_menu_item_check_set(mi, 1);
   e_menu_item_toggle_set(mi, (ec->layer == 150 ? 1 : 0));
   if (ec->layer == 150)
     e_menu_item_callback_set(mi, _e_client_menu_cb_normal, ec);
   else
     e_menu_item_callback_set(mi, _e_client_menu_cb_on_top, ec);
   e_menu_item_icon_edje_set(mi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/stack_on_top"),
                             "e/widgets/border/default/stack_on_top");

   if (!ec->lock_user_sticky)
     {
        mi = e_menu_item_new(m);
        e_menu_item_label_set(mi, _("Sticky"));
        e_menu_item_check_set(mi, 1);
        e_menu_item_toggle_set(mi, (ec->sticky ? 1 : 0));
        e_menu_item_callback_set(mi, _e_client_menu_cb_stick, ec);
        e_menu_item_icon_edje_set(mi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/stick"),
                                  "e/widgets/border/default/stick");
     }

   if ((!ec->lock_user_shade) && (!ec->fullscreen) && (!ec->maximized) &&
       ((!ec->border.name) || (!borderless)))
     {
        mi = e_menu_item_new(m);
        e_menu_item_label_set(mi, _("Shade"));
        e_menu_item_check_set(mi, 1);
        e_menu_item_toggle_set(mi, (ec->shaded ? 1 : 0));
        e_menu_item_callback_set(mi, _e_client_menu_cb_shade, ec);
        e_menu_item_icon_edje_set(mi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/shade"),
                                  "e/widgets/border/default/shade");
     }

   if ((!ec->fullscreen) && (!ec->lock_border) && (!ec->shading) && (!ec->shaded))
     {
        mi = e_menu_item_new(m);
        e_menu_item_label_set(mi, _("Borderless"));
        e_menu_item_check_set(mi, 1);
        e_menu_item_toggle_set(mi, borderless);
        e_menu_item_callback_set(mi, _e_client_menu_cb_borderless, ec);
        e_menu_item_icon_edje_set(mi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/borderless"),
                                  "e/widgets/border/default/borderless");
     }

   if (e_comp_config_get()->enable_advanced_features && (e_pixmap_type_get(ec->pixmap) == E_PIXMAP_TYPE_X))
     {
        mi = e_menu_item_new(m);
        e_menu_item_check_set(mi, 1);
        e_menu_item_label_set(mi, _("Unredirected"));
        e_menu_item_toggle_set(mi, !ec->redirected);
        e_menu_item_callback_set(mi, _e_client_menu_cb_redirect_set, ec);
     }

   if (!ec->lock_close)
     {
        mi = e_menu_item_new(m);
        e_menu_item_separator_set(mi, 1);

        mi = e_menu_item_new(m);
        e_menu_item_label_set(mi, _("Close"));
        e_menu_item_callback_set(mi, _e_client_menu_cb_close, ec);
        e_menu_item_icon_edje_set(mi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/close"),
                                  "e/widgets/border/default/close");
     }
   EINA_LIST_FOREACH(menu_hooks, l, h)
     h->cb(h->data, ec);
}

EAPI void
e_int_client_menu_show(E_Client *ec, Evas_Coord x, Evas_Coord y, int key, unsigned int timestamp)
{
   e_int_client_menu_create(ec);
   if (key)
     e_menu_activate_key(ec->border_menu, ec->zone, x, y, 1, 1,
                         E_MENU_POP_DIRECTION_DOWN);
   else
     e_menu_activate_mouse(ec->border_menu, ec->zone, x, y, 1, 1,
                           E_MENU_POP_DIRECTION_DOWN, timestamp);
}

EAPI void
e_int_client_menu_del(E_Client *ec)
{
   if (!ec->border_menu) return;
   e_menu_post_deactivate_callback_set(ec->border_menu, NULL, NULL);
   E_FREE_FUNC(ec->border_menu, e_object_del);
}

static void
_e_client_cb_border_menu_end(void *data __UNUSED__, E_Menu *m)
{
   E_Client *ec;

   ec = e_object_data_get(E_OBJECT(m));
   if (ec)
     {
        /* If the client exists, delete all associated menus */
        e_int_client_menu_del(ec);
     }
   else
     {
        /* Just delete this menu */
        e_object_del(E_OBJECT(m));
     }
}

static void
_e_client_menu_cb_locks(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (ec->border_locks_dialog)
     {
        e_client_desk_set(ec->border_locks_dialog->dia->win->client, ec->desk);
        e_win_raise(ec->border_locks_dialog->dia->win);
        evas_object_focus_set(ec->border_locks_dialog->dia->win->client->frame, 1);
        return;
     }
   e_int_client_locks(ec);
}

static void
_e_client_menu_cb_remember(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (ec->border_remember_dialog)
     {
        e_client_desk_set(ec->border_remember_dialog->dia->win->client, ec->desk);
        e_win_raise(ec->border_remember_dialog->dia->win);
        evas_object_focus_set(ec->border_remember_dialog->dia->win->client->frame, 1);
        return;
     }
   e_int_client_remember(ec);
}

static void
_e_client_menu_cb_border(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;
   char buf[256];

   ec = data;
   if (ec->border_border_dialog)
     {
        e_client_desk_set(ec->border_border_dialog->dia->win->client, ec->desk);
        e_win_raise(ec->border_border_dialog->dia->win);
        evas_object_focus_set(ec->border_border_dialog->dia->win->client->frame, 1);
        return;
     }
   snprintf(buf, sizeof(buf), "%p", ec);
   e_configure_registry_call("internal/borders_border", ec->zone->comp, buf);
}

static void
_e_client_menu_cb_borderless(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Client *ec = data;

   EC_CHANGED(ec);
   ec->border.changed = 1;
   ec->borderless = mi->toggle;
}


static void
_e_client_menu_cb_redirect_set(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi EINA_UNUSED)
{
   e_comp_client_redirect_toggle(data);
}

static void
_e_client_menu_cb_close(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_close) e_client_act_close_begin(ec);
}

static void
_e_client_menu_cb_iconify(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_iconify)
     {
        if (ec->iconic)
          e_client_uniconify(ec);
        else
          e_client_iconify(ec);
     }
}

static void
_e_client_menu_cb_kill(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Action *a;
   E_Client *ec;

   ec = data;
   if ((ec->lock_close) || (ec->internal)) return;

   a = e_action_find("window_kill");
   if ((a) && (a->func.go)) a->func.go(E_OBJECT(ec), NULL);
}

static void
_e_client_menu_cb_move(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;

   if (!ec->lock_user_location)
     e_client_act_move_keyboard(ec);
}

static void
_e_client_menu_cb_resize(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;

   if (!ec->lock_user_size)
     e_client_act_resize_keyboard(ec);
}

static void
_e_client_menu_cb_maximize_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *subm;
   E_Menu_Item *submi;
   E_Client *ec;

   if (!(ec = data)) return;

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   if ((!ec->lock_user_fullscreen) && (!ec->shaded))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Fullscreen"));
        e_menu_item_check_set(submi, 1);
        e_menu_item_toggle_set(submi, ec->fullscreen);
        e_menu_item_callback_set(submi, _e_client_menu_cb_fullscreen, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/fullscreen"),
                                  "e/widgets/border/default/fullscreen");
     }

   if (!ec->fullscreen)
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Maximize"));
        e_menu_item_radio_set(submi, 1);
        e_menu_item_radio_group_set(submi, 3);
        e_menu_item_toggle_set(submi, (ec->maximized & E_MAXIMIZE_DIRECTION) == E_MAXIMIZE_BOTH);
        e_menu_item_callback_set(submi, _e_client_menu_cb_maximize, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/maximize"),
                                  "e/widgets/border/default/maximize");

        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Maximize Vertically"));
        e_menu_item_radio_set(submi, 1);
        e_menu_item_radio_group_set(submi, 3);
        e_menu_item_toggle_set(submi, (ec->maximized & E_MAXIMIZE_DIRECTION) == E_MAXIMIZE_VERTICAL);
        e_menu_item_callback_set(submi, _e_client_menu_cb_maximize_vertically, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/maximize"),
                                  "e/widgets/border/default/maximize");

        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Maximize Horizontally"));
        e_menu_item_radio_set(submi, 1);
        e_menu_item_radio_group_set(submi, 3);
        e_menu_item_toggle_set(submi, (ec->maximized & E_MAXIMIZE_DIRECTION) == E_MAXIMIZE_HORIZONTAL);
        e_menu_item_callback_set(submi, _e_client_menu_cb_maximize_horizontally, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/maximize"),
                                  "e/widgets/border/default/maximize");

        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Maximize Left"));
        e_menu_item_radio_set(submi, 1);
        e_menu_item_radio_group_set(submi, 3);
        e_menu_item_toggle_set(submi, (ec->maximized & E_MAXIMIZE_DIRECTION) == E_MAXIMIZE_LEFT);
        e_menu_item_callback_set(submi, _e_client_menu_cb_maximize_left, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/maximize"),
                                  "e/widgets/border/default/maximize");

        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Maximize Right"));
        e_menu_item_radio_set(submi, 1);
        e_menu_item_radio_group_set(submi, 3);
        e_menu_item_toggle_set(submi, (ec->maximized & E_MAXIMIZE_DIRECTION) == E_MAXIMIZE_RIGHT);
        e_menu_item_callback_set(submi, _e_client_menu_cb_maximize_right, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/maximize"),
                                  "e/widgets/border/default/maximize");

        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Unmaximize"));
        e_menu_item_radio_set(submi, 1);
        e_menu_item_radio_group_set(submi, 3);
        e_menu_item_toggle_set(submi, (ec->maximized & E_MAXIMIZE_DIRECTION) == E_MAXIMIZE_NONE);
        e_menu_item_callback_set(submi, _e_client_menu_cb_unmaximize, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/maximize"),
                                  "e/widgets/border/default/maximize");
     }
}

static void
_e_client_menu_cb_maximize(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_maximize)
     e_client_maximize(ec, (e_config->maximize_policy & E_MAXIMIZE_TYPE) |
                       E_MAXIMIZE_BOTH);
}

static void
_e_client_menu_cb_maximize_vertically(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_maximize)
     {
        if ((ec->maximized & E_MAXIMIZE_HORIZONTAL))
          e_client_unmaximize(ec, E_MAXIMIZE_HORIZONTAL);
        e_client_maximize(ec, (e_config->maximize_policy & E_MAXIMIZE_TYPE) |
                          E_MAXIMIZE_VERTICAL);
     }
}

static void
_e_client_menu_cb_maximize_horizontally(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_maximize)
     {
        if ((ec->maximized & E_MAXIMIZE_VERTICAL))
          e_client_unmaximize(ec, E_MAXIMIZE_VERTICAL);
        e_client_maximize(ec, (e_config->maximize_policy & E_MAXIMIZE_TYPE) |
                          E_MAXIMIZE_HORIZONTAL);
     }
}

static void
_e_client_menu_cb_maximize_left(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_maximize)
     {
        if ((ec->maximized & E_MAXIMIZE_DIRECTION))
          e_client_unmaximize(ec, ec->maximized & E_MAXIMIZE_DIRECTION);
        e_client_maximize(ec, (e_config->maximize_policy & E_MAXIMIZE_TYPE) |
                          E_MAXIMIZE_LEFT);
     }
}

static void
_e_client_menu_cb_maximize_right(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_maximize)
     {
        if ((ec->maximized & E_MAXIMIZE_DIRECTION))
          e_client_unmaximize(ec, ec->maximized & E_MAXIMIZE_DIRECTION);
        e_client_maximize(ec, (e_config->maximize_policy & E_MAXIMIZE_TYPE) |
                          E_MAXIMIZE_RIGHT);
     }
}

static void
_e_client_menu_cb_unmaximize(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   e_client_unmaximize(ec, E_MAXIMIZE_BOTH);
}

static void
_e_client_menu_cb_shade(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_shade)
     {
        if (ec->shaded)
          e_client_unshade(ec, ec->shade_dir);
        else
          e_client_shade(ec, E_DIRECTION_UP);
     }
}

static void
_e_client_menu_cb_resistance(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   ec->offer_resistance = !ec->offer_resistance;
}

static void
_e_client_menu_cb_icon_edit(void *data, E_Menu *m, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   e_desktop_border_edit(m->zone->comp, ec);
}

static void
_e_client_menu_cb_application_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *subm;
   E_Menu_Item *submi;
   E_Client *ec;

   if (!(ec = data)) return;

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   if (ec->desktop)
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Edit Icon"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_icon_edit, ec);
        e_util_desktop_menu_item_icon_add(ec->desktop, 16, submi);
     }
   else if (ec->icccm.class)
     {
        /* icons with no class useless to borders */
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Create Icon"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_icon_edit, ec);
     }

   submi = e_menu_item_new(subm);
   e_menu_item_separator_set(submi, 1);

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Add to Favorites Menu"));
   e_menu_item_callback_set(submi, _e_client_menu_cb_fav_add, ec);
   e_util_menu_item_theme_icon_set(submi, "user-bookmarks");

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Add to IBar"));
   e_menu_item_submenu_pre_callback_set(submi,
                                        _e_client_menu_cb_ibar_add_pre, ec);
   e_util_menu_item_theme_icon_set(submi, "preferences-applications-ibar");

   if (e_configure_registry_exists("keyboard_and_mouse/key_bindings"))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Create Keyboard Shortcut"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_kbdshrtct_add, ec);
        e_util_menu_item_theme_icon_set(submi, "preferences-desktop-keyboard");
     }
}

static void
_e_client_menu_cb_window_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *subm;
   E_Menu_Item *submi;
   E_Client *ec;
   Eina_Bool resize = EINA_FALSE;

   if (!(ec = data)) return;

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   /* internal dialog which is resizable */
   if (ec->internal && (ec->netwm.type == E_WINDOW_TYPE_DIALOG))
     resize = (ec->icccm.max_w != ec->icccm.min_w);
   if (resize || (ec->netwm.type == E_WINDOW_TYPE_NORMAL) ||
       (ec->netwm.type == E_WINDOW_TYPE_UNKNOWN))
     {
        if (!(((ec->icccm.min_w == ec->icccm.max_w) &&
               (ec->icccm.min_h == ec->icccm.max_h)) ||
              (ec->lock_user_maximize)))
          {
             if ((!ec->lock_user_maximize) && (!ec->shaded))
               {
                  submi = e_menu_item_new(subm);
                  e_menu_item_label_set(submi, _("Maximize"));
                  e_menu_item_submenu_pre_callback_set(submi, _e_client_menu_cb_maximize_pre, ec);
                  e_menu_item_icon_edje_set(submi,
                                            e_theme_edje_file_get("base/theme/borders",
                                                                  "e/widgets/border/default/maximize"),
                                            "e/widgets/border/default/maximize");
               }
          }

        if ((!ec->lock_user_iconify) && (!ec->fullscreen))
          {
             submi = e_menu_item_new(subm);
             e_menu_item_label_set(submi, _("Iconify"));
             e_menu_item_callback_set(submi, _e_client_menu_cb_iconify, ec);
             e_menu_item_icon_edje_set(submi,
                                       e_theme_edje_file_get("base/theme/borders",
                                                             "e/widgets/border/default/minimize"),
                                       "e/widgets/border/default/minimize");
          }
     }

   if ((!ec->lock_user_location) && (!ec->fullscreen) &&
       (((ec->maximized & E_MAXIMIZE_DIRECTION) != E_MAXIMIZE_BOTH) || e_config->allow_manip))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Move"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_move, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/move_icon"),
                                  "e/widgets/border/default/move_icon");
     }

   if (((!ec->lock_user_size) && (!ec->fullscreen) &&
        (((ec->maximized & E_MAXIMIZE_DIRECTION) != E_MAXIMIZE_BOTH) || e_config->allow_manip)) &&
       ((ec->netwm.type == E_WINDOW_TYPE_NORMAL) ||
        (ec->netwm.type == E_WINDOW_TYPE_UNKNOWN)))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Resize"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_resize, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/resize_icon"),
                                  "e/widgets/border/default/resize_icon");
     }

   submi = e_menu_item_new(subm);
   e_menu_item_separator_set(submi, 1);

   if ((!ec->lock_user_stacking) && (!ec->fullscreen))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Stacking"));
        e_menu_item_submenu_pre_callback_set(submi, _e_client_menu_cb_stacking_pre, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/stacking"),
                                  "e/widgets/border/default/stacking");
     }

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Skip"));
   e_menu_item_submenu_pre_callback_set(submi, _e_client_menu_cb_skip_pre, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/skip"),
                             "e/widgets/border/default/skip");

   if (!ec->lock_border)
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Border"));
        e_menu_item_submenu_pre_callback_set(submi, _e_client_menu_cb_border_pre, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/borderless"),
                                  "e/widgets/border/default/borderless");
     }

   submi = e_menu_item_new(subm);
   e_menu_item_separator_set(submi, 1);

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Locks"));
   e_menu_item_callback_set(submi, _e_client_menu_cb_locks, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/locks"),
                             "e/widgets/border/default/locks");

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Remember"));
   e_menu_item_callback_set(submi, _e_client_menu_cb_remember, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/remember"),
                             "e/widgets/border/default/remember");

   submi = e_menu_item_new(subm);
   e_menu_item_separator_set(submi, 1);

   if ((!ec->internal) && (!ec->lock_close))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Kill"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_kill, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/kill"),
                                  "e/widgets/border/default/kill");
     }

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("ICCCM/NetWM"));
   e_menu_item_callback_set(submi, _e_client_menu_cb_prop, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/properties"),
                             "e/widgets/border/default/properties");
}

static void
_e_client_menu_cb_prop(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   e_int_client_prop(ec);
}

static void
_e_client_menu_cb_stick(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (!ec->lock_user_sticky)
     {
        if (ec->sticky)
          e_client_unstick(ec);
        else
          e_client_stick(ec);
     }
}

static void
_e_client_menu_cb_on_top(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (ec->layer != E_LAYER_CLIENT_ABOVE)
     evas_object_layer_set(ec->frame, E_LAYER_CLIENT_ABOVE);
}

static void
_e_client_menu_cb_below(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (ec->layer != E_LAYER_CLIENT_BELOW)
     evas_object_layer_set(ec->frame, E_LAYER_CLIENT_BELOW);
}

static void
_e_client_menu_cb_normal(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = data;
   if (ec->layer != E_LAYER_CLIENT_NORMAL)
     evas_object_layer_set(ec->frame, E_LAYER_CLIENT_NORMAL);
}

static void
_e_client_menu_cb_fullscreen(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Client *ec;
   int toggle;

   if (!(ec = data)) return;

   if (!ec->lock_user_fullscreen)
     {
        toggle = e_menu_item_toggle_get(mi);
        if (toggle)
          e_client_fullscreen(ec, e_config->fullscreen_policy);
        else
          e_client_unfullscreen(ec);
     }
}

static void
_e_client_menu_cb_skip_winlist(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Client *ec;

   if (!(ec = data)) return;

   if (((ec->icccm.accepts_focus) || (ec->icccm.take_focus)) &&
       (!ec->netwm.state.skip_taskbar))
     ec->user_skip_winlist = e_menu_item_toggle_get(mi);
   else
     ec->user_skip_winlist = 0;
   ec->changed = 1;
   e_remember_update(ec);
}

static void
_e_client_menu_cb_skip_pager(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Client *ec;

   if (!(ec = data)) return;

   if ((ec->icccm.accepts_focus) || (ec->icccm.take_focus))
     ec->netwm.state.skip_pager = e_menu_item_toggle_get(mi);
   else
     ec->netwm.state.skip_pager = 0;
   ec->changed = 1;
   e_remember_update(ec);
}

static void
_e_client_menu_cb_skip_taskbar(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Client *ec;

   if (!(ec = data)) return;

   if ((ec->icccm.accepts_focus) || (ec->icccm.take_focus))
     ec->netwm.state.skip_taskbar = e_menu_item_toggle_get(mi);
   else
     ec->netwm.state.skip_taskbar = 0;
   ec->changed = 1;
   e_remember_update(ec);
}

#ifndef DESKMIRROR_TEST
 static void
_e_client_menu_cb_sendto_icon_pre(void *data, E_Menu *m, E_Menu_Item *mi)
{
   E_Desk *desk = NULL;
   Evas_Object *o = NULL;
   const char *bgfile = NULL;
   int tw = 0, th = 0;

   desk = data;
   E_OBJECT_CHECK(desk);

   tw = 50;
   th = (tw * desk->zone->h) / desk->zone->w;
   bgfile = e_bg_file_get(desk->zone->comp->num, desk->zone->num,
                          desk->x, desk->y);
   o = e_thumb_icon_add(m->evas);
   e_thumb_icon_file_set(o, bgfile, "e/desktop/background");
   eina_stringshare_del(bgfile);
   e_thumb_icon_size_set(o, tw, th);
   e_thumb_icon_begin(o);
   mi->icon_object = o;
}
#endif

static void
_e_client_menu_cb_sendto_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *subm;
   E_Menu_Item *submi;
   E_Client *ec;
   E_Zone *zone;
   Eina_List *l = NULL;
   char buf[128];
   int zones, i;

   ec = data;
   zones = eina_list_count(ec->zone->comp->zones);

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   EINA_LIST_FOREACH(ec->comp->zones, l, zone)
     {
        if (zones > 1)
          {
             snprintf(buf, sizeof(buf), _("Screen %d"), zone->num);
             submi = e_menu_item_new(subm);
             e_menu_item_label_set(submi, buf);
             e_menu_item_disabled_set(submi, EINA_TRUE);
          }

        for (i = 0; i < zone->desk_x_count * zone->desk_y_count; i++)
          {
             E_Desk *desk;
#ifdef DESKMIRROR_TEST
             int tw = 50, th;
#endif
             desk = zone->desks[i];
#ifdef DESKMIRROR_TEST
             th = (tw * desk->zone->h) / desk->zone->w;
#endif
             submi = e_menu_item_new(subm);
             e_menu_item_label_set(submi, desk->name);
             e_menu_item_radio_set(submi, 1);
             e_menu_item_radio_group_set(submi, 2);
#ifdef DESKMIRROR_TEST
             e_menu_item_icon_file_set(submi, "sup");
#endif
             if ((ec->zone == zone) && (!ec->iconic) && (ec->desk == desk))
               e_menu_item_toggle_set(submi, 1);
             else
               e_menu_item_callback_set(submi, _e_client_menu_cb_sendto, desk);
#ifdef DESKMIRROR_TEST
             submi->icon_object = e_deskmirror_add(desk, 0, 0);
             edje_extern_object_min_size_set(submi->icon_object, tw, th);
             evas_object_show(submi->icon_object);
#else
             e_menu_item_realize_callback_set(submi, _e_client_menu_cb_sendto_icon_pre,
                                              desk);
#endif
          }
     }
}

static void
_e_client_menu_cb_sendto(void *data, E_Menu *m, E_Menu_Item *mi __UNUSED__)
{
   E_Desk *desk;
   E_Client *ec;

   desk = data;
   ec = e_object_data_get(E_OBJECT(m));
   if ((ec) && (desk))
     e_client_desk_set(ec, desk);
}

static void
_e_client_menu_cb_pin(void *data __UNUSED__, E_Menu *m, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = e_object_data_get(E_OBJECT(m));
   if (ec) e_client_pinned_set(ec, 1);
}

static void
_e_client_menu_cb_unpin(void *data __UNUSED__, E_Menu *m, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   ec = e_object_data_get(E_OBJECT(m));
   if (ec) e_client_pinned_set(ec, 0);
}

static void
_e_client_menu_cb_stacking_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *subm;
   E_Menu_Item *submi;
   E_Client *ec;

   if (!(ec = data)) return;

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   /* Only allow to change layer for windows in "normal" layers */
   e_menu_category_set(subm, "border/stacking");
   e_menu_category_data_set("border/stacking", ec);

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Always on Top"));
   e_menu_item_radio_set(submi, 1);
   e_menu_item_radio_group_set(submi, 2);
   e_menu_item_toggle_set(submi, (ec->layer == 150 ? 1 : 0));
   e_menu_item_callback_set(submi, _e_client_menu_cb_on_top, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/stack_on_top"),
                             "e/widgets/border/default/stack_on_top");

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Normal"));
   e_menu_item_radio_set(submi, 1);
   e_menu_item_radio_group_set(submi, 2);
   e_menu_item_toggle_set(submi, (ec->layer == 100 ? 1 : 0));
   e_menu_item_callback_set(submi, _e_client_menu_cb_normal, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/stack_normal"),
                             "e/widgets/border/default/stack_normal");

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Always Below"));
   e_menu_item_radio_set(submi, 1);
   e_menu_item_radio_group_set(submi, 2);
   e_menu_item_toggle_set(submi, (ec->layer == 50 ? 1 : 0));
   e_menu_item_callback_set(submi, _e_client_menu_cb_below, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/stack_below"),
                             "e/widgets/border/default/stack_below");

   submi = e_menu_item_new(subm);
   e_menu_item_separator_set(submi, 1);

   // Only allow to change layer for windows in "normal" layers
   if ((!ec->lock_user_stacking) &&
       ((ec->layer == 50) || (ec->layer == 100) || (ec->layer == 150)))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Raise"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_raise, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/stack_on_top"),
                                  "e/widgets/border/default/stack_on_top");

        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Lower"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_lower, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/stack_below"),
                                  "e/widgets/border/default/stack_below");
     }

   submi = e_menu_item_new(subm);
   e_menu_item_separator_set(submi, 1);

   if ((ec->netwm.type == E_WINDOW_TYPE_NORMAL) ||
       (ec->netwm.type == E_WINDOW_TYPE_UNKNOWN))
     {
        if ((ec->netwm.state.stacking != E_STACKING_BELOW) ||
            (!ec->user_skip_winlist) || (!ec->borderless))
          {
             submi = e_menu_item_new(subm);
             e_menu_item_label_set(submi, _("Pin to Desktop"));
             e_menu_item_callback_set(submi, _e_client_menu_cb_pin, ec);
             e_menu_item_icon_edje_set(submi,
                                       e_theme_edje_file_get("base/theme/borders",
                                                             "e/widgets/border/default/stick"),
                                       "e/widgets/border/default/stick");
          }
        if ((ec->netwm.state.stacking == E_STACKING_BELOW) &&
            (ec->user_skip_winlist) && (ec->borderless))
          {
             submi = e_menu_item_new(subm);
             e_menu_item_label_set(submi, _("Unpin from Desktop"));
             e_menu_item_callback_set(submi, _e_client_menu_cb_unpin, ec);
             e_menu_item_icon_edje_set(submi,
                                       e_theme_edje_file_get("base/theme/borders",
                                                             "e/widgets/border/default/stick"),
                                       "e/widgets/border/default/stick");
          }
     }
}

static void
_e_client_menu_cb_raise(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec = data;

   if ((!ec->lock_user_stacking) && (!ec->internal) &&
       ((ec->layer >= E_LAYER_CLIENT_DESKTOP) && (ec->layer <= E_LAYER_CLIENT_NORMAL)))
     {
        evas_object_raise(ec->frame);
     }
}

static void
_e_client_menu_cb_lower(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec = data;

   if ((!ec->lock_user_stacking) && (!ec->internal) &&
       ((ec->layer >= E_LAYER_CLIENT_DESKTOP) && (ec->layer <= E_LAYER_CLIENT_NORMAL)))
     {
        evas_object_lower(ec->frame);
     }
}

static void
_e_client_menu_cb_default_icon(void *data, E_Menu *m, E_Menu_Item *mi)
{
   E_Client *ec;
   Evas_Object *o;
   unsigned char prev_icon_pref;

   ec = data;
   E_OBJECT_CHECK(ec);

   prev_icon_pref = ec->icon_preference;
   ec->icon_preference = E_ICON_PREF_E_DEFAULT;
   o = e_client_icon_add(ec, m->evas);
   ec->icon_preference = prev_icon_pref;
   mi->icon_object = o;
}

static void
_e_client_menu_cb_netwm_icon(void *data, E_Menu *m, E_Menu_Item *mi)
{
   E_Client *ec;
   Evas_Object *o;

   ec = data;
   E_OBJECT_CHECK(ec);

   if (ec->netwm.icons)
     {
        o = e_icon_add(m->evas);
        e_icon_data_set(o, ec->netwm.icons[0].data,
                        ec->netwm.icons[0].width,
                        ec->netwm.icons[0].height);
        e_icon_alpha_set(o, 1);
        mi->icon_object = o;
     }
}

static void
_e_client_menu_cb_border_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *subm;
   E_Menu_Item *submi;
   E_Client *ec;

   if (!(ec = data)) return;

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   if (e_configure_registry_exists("internal/borders_border"))
     {
        submi = e_menu_item_new(subm);
        e_menu_item_label_set(submi, _("Select Border Style"));
        e_menu_item_callback_set(submi, _e_client_menu_cb_border, ec);
        e_menu_item_icon_edje_set(submi,
                                  e_theme_edje_file_get("base/theme/borders",
                                                        "e/widgets/border/default/borderless"),
                                  "e/widgets/border/default/borderless");

        submi = e_menu_item_new(subm);
        e_menu_item_separator_set(submi, 1);
     }

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Use Enlightenment Default Icon Preference"));
   e_menu_item_radio_set(submi, 1);
   e_menu_item_radio_group_set(submi, 2);
   e_menu_item_toggle_set(submi, (ec->icon_preference == E_ICON_PREF_E_DEFAULT ? 1 : 0));
   e_menu_item_realize_callback_set(submi, _e_client_menu_cb_default_icon, ec);
   e_menu_item_callback_set(submi, _e_client_menu_cb_iconpref_e, ec);

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Use Application Provided Icon"));
   e_menu_item_radio_set(submi, 1);
   e_menu_item_radio_group_set(submi, 2);
   e_menu_item_toggle_set(submi, (ec->icon_preference == E_ICON_PREF_NETWM ? 1 : 0));
   e_menu_item_realize_callback_set(submi, _e_client_menu_cb_netwm_icon, ec);
   e_menu_item_callback_set(submi, _e_client_menu_cb_iconpref_netwm, ec);

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Use User Defined Icon"));
   e_menu_item_radio_set(submi, 1);
   e_menu_item_radio_group_set(submi, 2);
   e_menu_item_toggle_set(submi, (ec->icon_preference == E_ICON_PREF_USER ? 1 : 0));
   e_util_desktop_menu_item_icon_add(ec->desktop, 16, submi);
   e_menu_item_callback_set(submi, _e_client_menu_cb_iconpref_user, ec);
   e_menu_item_separator_set(submi, 1);

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Offer Resistance"));
   e_menu_item_check_set(submi, 1);
   e_menu_item_toggle_set(submi, (ec->offer_resistance ? 1 : 0));
   e_menu_item_callback_set(submi, _e_client_menu_cb_resistance, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/borderless"),
                             "e/widgets/border/default/borderless");
}

static void
_e_client_menu_cb_iconpref_e(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   if (!(ec = data)) return;

   ec->icon_preference = E_ICON_PREF_E_DEFAULT;
   ec->changes.icon = 1;
   ec->changed = 1;
}

static void
_e_client_menu_cb_iconpref_user(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   if (!(ec = data)) return;

   ec->icon_preference = E_ICON_PREF_USER;
   ec->changes.icon = 1;
   ec->changed = 1;
}

static void
_e_client_menu_cb_iconpref_netwm(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;

   if (!(ec = data)) return;

   ec->icon_preference = E_ICON_PREF_NETWM;
   ec->changes.icon = 1;
   ec->changed = 1;
}

static void
_e_client_menu_cb_skip_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Client *ec;
   E_Menu *subm;
   E_Menu_Item *submi;

   if (!(ec = data)) return;

   subm = e_menu_new();
   e_object_data_set(E_OBJECT(subm), ec);
   e_menu_item_submenu_set(mi, subm);
   e_object_unref(E_OBJECT(subm));

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Window List"));
   e_menu_item_check_set(submi, 1);
   e_menu_item_toggle_set(submi, ec->user_skip_winlist);
   e_menu_item_callback_set(submi, _e_client_menu_cb_skip_winlist, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/skip_winlist"),
                             "e/widgets/border/default/skip_winlist");

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Pager"));
   e_menu_item_check_set(submi, 1);
   e_menu_item_toggle_set(submi, ec->netwm.state.skip_pager);
   e_menu_item_callback_set(submi, _e_client_menu_cb_skip_pager, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/skip_pager"),
                             "e/widgets/border/default/skip_pager");

   submi = e_menu_item_new(subm);
   e_menu_item_label_set(submi, _("Taskbar"));
   e_menu_item_check_set(submi, 1);
   e_menu_item_toggle_set(submi, ec->netwm.state.skip_taskbar);
   e_menu_item_callback_set(submi, _e_client_menu_cb_skip_taskbar, ec);
   e_menu_item_icon_edje_set(submi,
                             e_theme_edje_file_get("base/theme/borders",
                                                   "e/widgets/border/default/skip_taskbar"),
                             "e/widgets/border/default/skip_taskbar");
}

static void
_e_client_menu_cb_fav_add(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;
   Efreet_Menu *menu;
   char buf[PATH_MAX];

   if (!(ec = data)) return;
   e_user_dir_concat_static(buf, "applications/menu/favorite.menu");
   menu = efreet_menu_parse(buf);
   if (!menu)
     menu = efreet_menu_new("Favorites");
   if (!menu) return;
   efreet_menu_desktop_insert(menu, ec->desktop, -1);
   efreet_menu_save(menu, buf);
   efreet_menu_free(menu);
}

static void
_e_client_menu_cb_kbdshrtct_add(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi __UNUSED__)
{
   E_Client *ec;
   E_Zone *zone;

   if (!(ec = data)) return;
   zone = e_util_zone_current_get(e_manager_current_get());
   if (!zone) return;
   e_configure_registry_call("keyboard_and_mouse/key_bindings",
                             zone->comp, ec->desktop->exec);
}

static void
_e_client_menu_cb_ibar_add_pre(void *data, E_Menu *m __UNUSED__, E_Menu_Item *mi)
{
   E_Menu *sm;
   E_Client *ec;
   Eina_List *dirs;
   Eina_List *l;
   char buf[PATH_MAX], *file;
   size_t len;

   if (!(ec = data)) return;
   len = e_user_dir_concat_static(buf, "applications/bar");
   if (len + 1 >= sizeof(buf)) return;
   dirs = ecore_file_ls(buf);
   if (!dirs) return;

   buf[len] = '/';
   len++;

   sm = e_menu_new();
   EINA_LIST_FOREACH(dirs, l, file)
     {
        E_Menu_Item *smi;

        if (file[0] == '.') continue;

        eina_strlcpy(buf + len, file, sizeof(buf) - len);
        if (ecore_file_is_dir(buf))
          {
             smi = e_menu_item_new(sm);
             e_menu_item_label_set(smi, file);
             e_menu_item_callback_set(smi, _e_client_menu_cb_ibar_add, file);
          }
     }
   e_object_data_set(E_OBJECT(sm), ec);
   e_menu_item_submenu_set(mi, sm);
   e_object_unref(E_OBJECT(sm));
}

static void
_e_client_menu_cb_ibar_add(void *data, E_Menu *m, E_Menu_Item *mi __UNUSED__)
{
   E_Order *od;
   E_Client *ec;
   char buf[PATH_MAX];

   ec = e_object_data_get(E_OBJECT(m));
   if ((!ec) || (!ec->desktop)) return;

   e_user_dir_snprintf(buf, sizeof(buf), "applications/bar/%s/.order",
                       (const char *)data);
   od = e_order_new(buf);
   if (!od) return;
   e_order_append(od, ec->desktop);
   e_object_del(E_OBJECT(od));
}

/*vim:ts=8 sw=3 sts=3 expandtab cino=>5n-3f0^-2{2(0W1st0*/

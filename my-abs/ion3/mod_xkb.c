/*
 * ion-scripts-3/misc/mod_xkb.c
 *
 * Copyright (c) Sergey Redin 2006. 
 *
 * Ion is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 */

#include <ioncore/global.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKB.h>

#include "exports.h"


/*{{{ Module information */


#include "../version.h"

char mod_xkb_ion_api_version[]=ION_API_VERSION;

/*}}}*/

/*{{{ Exports */

/*EXTL_DOC
 * Set the current XKB group. See \code{XkbLockGroup}(3) manual page
 * for details. See xkbion.lua for example use.
 */
EXTL_EXPORT
int mod_xkb_lock_group(int state)
{
	return XkbLockGroup(ioncore_g.dpy, XkbUseCoreKbd, state);
}

/*EXTL_DOC
 * Latch modifiers. See \code{XkbLatchModifiers}(3) manual page
 * for details.
 */
EXTL_EXPORT
int mod_xkb_lock_modifiers(int affect, int values)
{
	return XkbLockModifiers(ioncore_g.dpy, XkbUseCoreKbd, affect, values);
}

/*}}}*/



/*{{{ Init & deinit */


void mod_xkb_deinit()
{
    mod_xkb_unregister_exports();
}


   

bool mod_xkb_init()
{
    if(!mod_xkb_register_exports())
        return FALSE;

/*    extl_read_config("cfg_xkb", NULL, FALSE); */
    
    return TRUE;
}


/*}}}*/


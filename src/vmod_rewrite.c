#include <stdlib.h>
#include "vcl.h"
#include "vrt.h"
#include "bin/varnishd/cache.h"

#include "vcc_if.h"
vcl_func_f *vmod_redirect_Hook_vcl_error;

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	return (0);
}

static int vmod_Hook_vcl_error(struct sess *sp){
	int status=VRT_r_obj_status(sp);
	if (
		status == 301 ||
		status == 302 ||
		status == 303
	){
		char * location = VRT_GetHdr(sp, HDR_REQ, "\030X-VMODREDIRECT-Location:");
		if(location != 0){
			VRT_SetHdr(sp, HDR_OBJ, "\011Location:" , location , vrt_magic_string_end);
			VRT_done(sp, VCL_RET_DELIVER);
		}
	}

	return(vmod_redirect_Hook_vcl_error(sp));

}


int 
vmod_location(struct sess *sp, struct vmod_priv *priv, int status, const char*p,...)
{
	if (
		status == 301 ||
		status == 302 ||
		status == 303
	){

		if (priv->priv == NULL) {
			priv->priv = malloc(1);
			priv->free = free;
			vmod_redirect_Hook_vcl_error = sp->vcl->error_func;
			sp->vcl->error_func = vmod_Hook_vcl_error;
		}

		//build location string
		va_list ap;
		char *location;
		va_start(ap, p);
		location = VRT_String(sp->wrk->ws, NULL, p, ap);
		va_end(ap); 
		
		//set location header
		VRT_SetHdr(sp, HDR_REQ, "\030X-VMODREDIRECT-Location:",
			location,
			vrt_magic_string_end
		);
	}
	return (status);
}

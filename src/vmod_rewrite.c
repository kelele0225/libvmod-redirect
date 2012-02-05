#include <stdlib.h>
#include "vcl.h"
#include "vrt.h"
#include "bin/varnishd/cache.h"

#include "vcc_if.h"
static vcl_func_f 			*vmod_redirect_Hook_vcl_error = NULL;
static pthread_mutex_t		tmutex = PTHREAD_MUTEX_INITIALIZER;

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
vmod_location(struct sess *sp, int status, const char*p,...)
{
	if (
		status == 301 ||
		status == 302 ||
		status == 303
	){

		if(vmod_redirect_Hook_vcl_error == NULL)
		{
			AZ(pthread_mutex_lock(&tmutex));
			if(vmod_redirect_Hook_vcl_error == NULL)
			{
				vmod_redirect_Hook_vcl_error = sp->vcl->error_func;
				sp->vcl->error_func = vmod_Hook_vcl_error;
			}
			AZ(pthread_mutex_unlock(&tmutex));
		}
		/*
		if (priv->priv == NULL) {
			priv->priv = malloc(1);
			priv->free = free;
			vmod_redirect_Hook_vcl_error = sp->vcl->error_func;
			sp->vcl->error_func = vmod_Hook_vcl_error;
		}
		*/

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

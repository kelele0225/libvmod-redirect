============
vmod_rewrite
============

----------------------
Varnish Rewrite Module
----------------------

:Author: Syohei Tanaka(@xcir)
:Date: 2012-02-05
:Version: 0.2
:Manual section: 3

SYNOPSIS
========

import rewrite;

DESCRIPTION
===========

Varnish redirect operation to easy.

before(via: https://www.varnish-cache.org/trac/wiki/VCLExampleRedirectInVCL)
------------------------------------------------------------------------------------
::
  
  sub vcl_recv {
    if (req.http.user-agent ~ "iP(hone|od)") {
      error 750 "Moved Temporarily";
    }
  }

  sub vcl_error {
    if (obj.status == 750) {
      set obj.http.Location = "http://www.example.com/iphoneversion/";
      set obj.status = 302;
      return(deliver);
    }
  }

after
--------------
::
  
  import rewrite;
  sub vcl_recv {
     if (req.http.user-agent ~ "iP(hone|od)") {
        error(rewrite.location(302,"http://www.example.com/iphoneversion/") , "Moved Temporarily");
     }
  }

FUNCTIONS
=========

location
---------

Prototype
        ::

                location(INT status , STRING location)
Return value
	INT status
Description
	Performing redirects be used in conjunction with error function.
Example
        ::

                //no reason
                error(rewrite.location(302,"http://xcir.net/"));

                //use reason
                error(rewrite.location(302,"http://xcir.net/") , "Moved Temporarily");

INSTALLATION
============

Installation requires Varnish source tree.

Usage::

 ./autogen.sh
 ./configure VARNISHSRC=DIR [VMODDIR=DIR]

`VARNISHSRC` is the directory of the Varnish source tree for which to
compile your vmod. Both the `VARNISHSRC` and `VARNISHSRC/include`
will be added to the include search paths for your module.

Optionally you can also set the vmod install directory by adding
`VMODDIR=DIR` (defaults to the pkg-config discovered directory from your
Varnish installation).

Make targets:

* make - builds the vmod
* make install - installs your vmod in `VMODDIR`
* make check - runs the unit tests in ``src/tests/*.vtc``

In your VCL you could then use this vmod along the following lines::
        
        import rewrite;

        sub vcl_recv {
                //redirect to my hp
                error(rewrite.location(302,"http://xcir.net/"));
        }

HISTORY
=======

Version 0.2: Bug fix.
 
Version 0.1: Initial version.

COPYRIGHT
=========

This document is licensed under the same license as the
libvmod-rewrite project. See LICENSE for details.

* Copyright (c) 2012 Syohei Tanaka(@xcir)

File layout and configuration based on libvmod-example

* Copyright (c) 2011 Varnish Software AS

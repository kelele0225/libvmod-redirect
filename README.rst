============
vmod_rewrite
============

----------------------
Varnish Rewrite Module
----------------------

:Author: Syohei Tanaka(@xcir)
:Date: 2012-02-05
:Version: 0.1
:Manual section: 3

SYNOPSIS
========

import rewrite;

DESCRIPTION
===========

Varnish redirect operation to easy.

before(via: https://www.varnish-cache.org/trac/wiki/VCLExampleRedirectInVCL)
-----
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
-----
  ::
    import rewrite;
    sub vcl_recv {
       if (req.http.user-agent ~ "iP(hone|od)") {
          error(rewrite.location(302,"http://www.example.com/iphoneversion/"),"Moved Temporarily");
       }
    }

FUNCTIONS
=========

location
-----

Prototype
        ::

                location(INT status , STRING location)
Return value
	INT status
Description
	send location header(Location: XXXXXX)
Example
        ::

                error(rewrite.location(302,"http://xcir.net/"));

INSTALLATION
============

This is an example skeleton for developing out-of-tree Varnish
vmods. It implements the "Hello, World!" as a vmod callback. Not
particularly useful in good hello world tradition, but demonstrates how
to get the glue around a vmod working.

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the varnishtest tool.

Usage::

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
                //
                error(rewrite.location(302,"http://xcir.net/"));
        }

HISTORY
=======

This manual page was released as part of the libvmod-example package,
demonstrating how to create an out-of-tree Varnish vmod.

File layout and configuration based on libvmod-example.

COPYRIGHT
=========

This document is licensed under the same license as the
libvmod-example project. See LICENSE for details.

* Copyright (c) 2012 Syohei Tanaka(@xcir)

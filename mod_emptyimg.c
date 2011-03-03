/**
 * Module emptyimg inspired by nginx module
 * Could improve perfomance by reducing disk operations on 
 * reading empty gif images.
 * Use with caution.
 * USAGE:
 * apxs -i -c mod_emptyimg.c will compile and install module for you
 * then edit your apache config file to add following lines
 * LoadModule uptags_module mod_emptyimg.so
 *
 * <IfModule mod_emptyimg.c>
 *		<Location /images/empty.gif>
 *			AddHandler emptyimg-handler
 *		</Location>
 * </IfModule>
 * @author Evgeny Soynov <saboteur@saboteur.mp>
 */

#include "httpd.h"
#include "http_config.h"

/* Borrowed from nginx */
static const u_char empty_gif[] = {

    'G', 'I', 'F', '8', '9', 'a',  /* header                                 */

                                   /* logical screen descriptor              */
    0x01, 0x00,                    /* logical screen width                   */
    0x01, 0x00,                    /* logical screen height                  */
    0x80,                          /* global 1-bit color table               */
    0x01,                          /* background color #1                    */
    0x00,                          /* no aspect ratio                        */

                                   /* global color table                     */
    0x00, 0x00, 0x00,              /* #0: black                              */
    0xff, 0xff, 0xff,              /* #1: white                              */

                                   /* graphic control extension              */
    0x21,                          /* extension introducer                   */
    0xf9,                          /* graphic control label                  */
    0x04,                          /* block size                             */
    0x01,                          /* transparent color is given,            */
                                   /*     no disposal specified,             */
                                   /*     user input is not expected         */
    0x00, 0x00,                    /* delay time                             */
    0x01,                          /* transparent color #1                   */
    0x00,                          /* block terminator                       */

                                   /* image descriptor                       */
    0x2c,                          /* image separator                        */
    0x00, 0x00,                    /* image left position                    */
    0x00, 0x00,                    /* image top position                     */
    0x01, 0x00,                    /* image width                            */
    0x01, 0x00,                    /* image height                           */
    0x00,                          /* no local color table, no interlaced    */

    0x01,                          /* transparent color is given,            */
                                   /*     no disposal specified,             */
                                   /*     user input is not expected         */
    0x00, 0x00,                    /* delay time                             */
    0x01,                          /* transparent color #1                   */
    0x00,                          /* block terminator                       */

                                   /* image descriptor                       */
    0x2c,                          /* image separator                        */
    0x00, 0x00,                    /* image left position                    */
    0x00, 0x00,                    /* image top position                     */
    0x01, 0x00,                    /* image width                            */
    0x01, 0x00,                    /* image height                           */
    0x00,                          /* no local color table, no interlaced    */

                                   /* table based image data                 */
    0x02,                          /* LZW minimum code size,                 */
                                   /*     must be at least 2-bit             */
    0x02,                          /* block size                             */
    0x4c, 0x01,                    /* compressed bytes 01_001_100, 0000000_1 */
                                   /* 100: clear code                        */
                                   /* 001: 1                                 */
                                   /* 101: end of information code           */
    0x00,                          /* block terminator                       */

    0x3B                           /* trailer                                */
};


module AP_MODULE_DECLARE_DATA emptyimg_module;

static command_rec emptyimg_directives[] = {
    AP_INIT_FLAG(
        "EmptyImg",
        ap_set_flag_slot,
        NULL,
        OR_OPTIONS,
        "empty image module"
    ),

    {NULL}
};

static int emptyimg_handler( request_rec *r) {
    if (strcmp( r->handler, "emptyimg-handler")) {
        return DECLINED;
    }   

 
    r->content_type = "image/gif";

    if (r->header_only) {
        return OK;
    }
    
    ap_rwrite(empty_gif, 49, r);
     
    return OK;
}

static void emptyimg_register_hooks( apr_pool_t *p) {
    ap_hook_handler( emptyimg_handler, NULL, NULL, APR_HOOK_FIRST);
}

module AP_MODULE_DECLARE_DATA emptyimg_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,                          
    NULL,
    NULL,                          
    NULL,
    emptyimg_register_hooks
};

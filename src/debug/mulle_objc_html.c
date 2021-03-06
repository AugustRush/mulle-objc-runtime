//
//  common.inc
//  mulle-objc
//
//  Created by Nat! on 10.05.16.
//  Copyright (c) 2016 Nat! - Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#include "mulle_objc_html.h"

#include "mulle_objc.h"
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>


// have my own, because asprintf is a gnu extension, whereas
// vsnprintf should be C99. Use malloc because that's what vsprintf does
//
static int   mulle_vasprintf( char **output, char *format, va_list args)
{
   int      size;
   va_list  tmp;
   
   va_copy( tmp, args);
   size = vsnprintf( NULL, 0, format, tmp);
   va_end( tmp);
   
   if( size < 0)
      return( size);
   
   *output = (char *) malloc( size + 1);
   if( ! *output)
      return( -1);
   
   return( vsprintf( *output, format, args));
}


static int   mulle_asprintf( char **output, char *format, ...)
{
   va_list  args;
   int      size;
   
   va_start( args, format);
   size = mulle_vasprintf( output, format, args);
   va_end(args);
   
   return size;
}


#define asprintf  mulle_asprintf



//
// stuff shared by graphviz and html, don't want to expose the
// symbols or dick around with linker options
//

static char  *inheritance_description( unsigned int inheritance)
{
   char     *tmp;
   size_t   len;
   
   tmp = malloc( 128);
   if( ! tmp)
      return( tmp);
   
   tmp[ 0] = 0;
   if( ! (inheritance & MULLE_OBJC_CLASS_DONT_INHERIT_SUPERCLASS))
      strcat( tmp, "superclass ");
   if( ! (inheritance & MULLE_OBJC_CLASS_DONT_INHERIT_CATEGORIES))
      strcat( tmp, "categories ");
   if( ! (inheritance & MULLE_OBJC_CLASS_DONT_INHERIT_PROTOCOLS))
      strcat( tmp, "protocols ");
   if( ! (inheritance & MULLE_OBJC_CLASS_DONT_INHERIT_PROTOCOL_CATEGORIES))
      strcat( tmp, "protocol_categories ");
   
   len = strlen( tmp);
   if( len)
      tmp[ len - 1] = 0;
   return( tmp);
}


char  *mulle_objc_runtime_html_description( struct _mulle_objc_runtime *runtime)
{
   size_t         len;
   char           *s;
   char           *tmp[ 3];
   unsigned int   i;
   
   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
            "<TABLE>\n<TR><TD COLSPAN=\"2\" BGCOLOR=\"red\"><FONT COLOR=\"white\">runtime</FONT></TD></TR>\n");
   asprintf( &tmp[ 1],
            "<TR><TD>version</TD><TD>0x%x</TD></TR>\n",
               runtime->version);
   asprintf( &tmp[ 2],
            "</TABLE>");
   
   len = 0;
   for( i = 0; i < 3; i++)
      len += strlen( tmp[ i]);
   
   // concatenate all strings
   
   s = malloc( len + 1);
   s[ 0] = 0;
   
   for( i = 0; i < 3; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }
   
   return( s);
}


char  *mulle_objc_staticstring_html_description( struct _mulle_objc_staticstring *string)
{
   size_t         len;
   char           *s;
   char           *tmp[ 4];
   unsigned int   i;
   
   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
            "<TABLE>\n<TR><TD COLSPAN=\"2\" BGCOLOR=\"green\"><FONT COLOR=\"white\">string</FONT></TD></TR>\n");
   asprintf( &tmp[ 1],
            "<TR><TD>_s</TD><TD>%s</TD></TR>\n",
               string->_s ? string->_s : "*null*");
   asprintf( &tmp[ 2],
            "<TR><TD>_len</TD><TD>%d</TD></TR>\n",
               string->_len);
   asprintf( &tmp[ 3],
            "</TABLE>");
   
   len = 0;
   for( i = 0; i < 4; i++)
      len += strlen( tmp[ i]);
   
   // concatenate all strings
   
   s = malloc( len + 1);
   s[ 0] = 0;
   
   for( i = 0; i < 4; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }
   
   return( s);
}


char   *mulle_objc_class_short_html_description( struct _mulle_objc_class *cls)
{
   char   *s;
   char   *prefix;

   prefix = _mulle_objc_class_is_metaclass( cls) ? "+" : "";
   asprintf( &s, "<a href=\"%s%s.html\">%s (%08x)</a>\n", prefix, cls->name, cls->name, cls->classid);
   return( s);
}


char  *mulle_objc_class_html_description( struct _mulle_objc_class *cls, char *color)
{
   size_t         len;
   char           *s;
   char           *tmp[ 7];
   unsigned int   i;

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
            "<TABLE>\n<TR><TD COLSPAN=\"2\" BGCOLOR=\"%s\"><FONT COLOR=\"white\">%s</FONT></TD></TR>\n",
            color, cls->name);
   asprintf( &tmp[ 1],
            "<TR><TD>instance_and_header_size</TD><TD>%lu</TD></TR>\n",
            cls->instance_and_header_size);

   s = inheritance_description( cls->inheritance);
   asprintf( &tmp[ 2],
            "<TR><TD>inheritance</TD><TD>%s</TD></TR>\n",
            s);
   free( s);

   asprintf( &tmp[ 3],
            "<TR><TD>state</TD><TD>0x%lx</TD></TR>\n",
            (long) _mulle_atomic_pointer_nonatomic_read( &cls->state));
   asprintf( &tmp[ 4],
            "<TR><TD>ivarhash</TD><TD>0x%lx</TD></TR>\n",
            (long) cls->ivarhash);
   
   asprintf( &tmp[ 5],
            "<TR><TD>preloads</TD><TD>0x%x</TD></TR>\n",
            cls->preloads);
   asprintf( &tmp[ 6],
            "</TABLE>");

   len = 0;
   for( i = 0; i < sizeof( tmp) / sizeof( char *); i++)
      len += strlen( tmp[ i]);

   // concatenate all strings

   s = malloc( len + 1);
   s[ 0] = 0;
   
   for( i = 0; i < sizeof( tmp) / sizeof( char *); i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   return( s);
}


char  *mulle_objc_ivarlist_html_description( struct _mulle_objc_ivarlist *list)
{
   size_t         len;
   char           **tmp;
   char           *s;
   unsigned int   i;
   unsigned int   n;

   n   = list->n_ivars + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return(NULL);

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
             "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>n_ivars</TD><TD>%u</TD></TR>\n",
             list->n_ivars);
   len = strlen( tmp[ 0]);

   for( i = 0; i < list->n_ivars; i++)
   {
      asprintf( &tmp[ i + 1], "<TR><TD>%s</TD>"
               "<TD><TABLE>"
               "<TR><TD>signature</TD><TD>%s</TD></TR>"
               "<TR><TD>methodid</TD><TD>0x%lx</TD></TR>"
               "<TR><TD>offset</TD><TD>%d</TD></TR>"
               "</TABLE></TD>"
               "</TR>\n",
               list->ivars[ i].descriptor.name,
               list->ivars[ i].descriptor.signature,
               (long) list->ivars[ i].descriptor.ivarid,
               list->ivars[ i].offset);
      len += strlen( tmp[ i + 1]);
   }

   asprintf( &tmp[ i + 1], "</TABLE>");
   len += strlen( tmp[ i + 1]);


   // concatenate all strings

   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   free( tmp);

   return( s);
}


char  *mulle_objc_ivarlist_html_hor_description( struct _mulle_objc_ivarlist *list)
{
   size_t         len;
   char           **tmp;
   char           *s;
   unsigned int   i;
   unsigned int   n;

   n   = list->n_ivars + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return(NULL);

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
             "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>n_ivars</TD><TD>%u</TD></TR>\n",
             list->n_ivars);
   len = strlen( tmp[ 0]);

   for( i = 0; i < list->n_ivars; i++)
   {
      asprintf( &tmp[ i + 1], "<TR><TD>%s</TD>"
               "<TD>%s</TD>"
               "<TD>%08x</TD>"
               "<TD>%d</TD>"
               "</TR>\n",
               list->ivars[ i].descriptor.name,
               list->ivars[ i].descriptor.signature,
               list->ivars[ i].descriptor.ivarid,
               list->ivars[ i].offset);
      len += strlen( tmp[ i + 1]);
   }

   asprintf( &tmp[ i + 1], "</TABLE>");
   len += strlen( tmp[ i + 1]);


   // concatenate all strings

   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   free( tmp);

   return( s);
}


char   *mulle_objc_methoddescriptor_html_description( struct _mulle_objc_methoddescriptor *desc)
{
   char   *s;

   asprintf( &s,
            "<TABLE>"
            "<TR><TD>name</TD><TD>%s</TD></TR>"
            "<TR><TD>signature</TD><TD>%s</TD></TR>"
            "<TR><TD>methodid</TD><TD>0x%lx</TD></TR>"
            "<TR><TD>bits</TD><TD>0x%x</TD></TR>"
            "</TABLE>",
            desc->name,
            desc->signature,
            (long) desc->methodid,
            desc->bits);
   
   return( s);
}


char   *mulle_objc_methoddescriptor_html_hor_description( struct _mulle_objc_methoddescriptor *desc)
{
   char   *s;

   asprintf( &s,
            "<TR>"
            "<TD>%s</TD>"
            "<TD>%s</TD>"
            "<TD>%08x</TD>"
            "<TD>0x%x</TD>"
            "</TR>",
            desc->name,
            desc->signature,
            desc->methodid,
            desc->bits);
   
   return( s);
}




char  *mulle_objc_propertylist_html_description( struct _mulle_objc_propertylist *list)
{
   size_t         len;
   char           **tmp;
   char           *s;
   unsigned int   i;
   unsigned int   n;

   n   = list->n_properties + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return( NULL);

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
             "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>n_properties</TD><TD>%u</TD></TR>\n",
             list->n_properties);
   len = strlen( tmp[ 0]);

   for( i = 0; i < list->n_properties; i++)
   {
      asprintf( &tmp[ i + 1], "<TR><TD>%s</TD>"
               "<TD><TABLE>"
               "<TR><TD>signature</TD><TD>%s</TD></TR>"
               "<TR><TD>propertyid</TD><TD>%08x</TD></TR>"
               "<TR><TD>getter</TD><TD>%08x</TD></TR>"
               "<TR><TD>setter</TD><TD>%08x</TD></TR>"
               "<TR><TD>clearer</TD><TD>%08x</TD></TR>"
               "</TABLE></TD>"
               "</TR>\n",
               list->properties[ i].name,
               list->properties[ i].signature,
               list->properties[ i].propertyid,
               list->properties[ i].getter,
               list->properties[ i].setter,
               list->properties[ i].clearer);
               
      len += strlen( tmp[ i + 1]);
   }

   asprintf( &tmp[ i + 1], "</TABLE>");
   len += strlen( tmp[ i + 1]);

   // concatenate all strings

   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   free( tmp);

   return( s);
}



char  *mulle_objc_cache_html_description( struct _mulle_objc_cache *cache)
{
   size_t          len;
   char            **tmp;
   char            *s;
   unsigned long   i;
   unsigned long   n;

   n   = cache->size + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return(NULL);

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
             "<TABLE>\n<TR BGCOLOR=\"gray\"><TD COLSPAN=\"2\">size</TD><TD>%lu</TD></TR>\n",
             (long) cache->size);
   len = strlen( tmp[ 0]);

   for( i = 0; i < cache->size; i++)
   {
      asprintf( &tmp[ i + 1],
               "<TR><TD>#%ld</TD><TD>0x%lx</TD><TD>%p</TD></TR>\n",
               i,
               (long) cache->entries[ i].key.uniqueid,
               cache->entries[ i].value.functionpointer);
      len += strlen( tmp[ i + 1]);
   }

   asprintf( &tmp[ i + 1], "</TABLE>");
   len += strlen( tmp[ i + 1]);

   // concatenate all strings

   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   free( tmp);

   return( s);
}


char  *mulle_objc_methodlist_html_description( struct _mulle_objc_methodlist *list)
{
   size_t         len;
   char           **tmp;
   char           *s;
   unsigned int   i;
   unsigned int   n;

   n   = list->n_methods + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return(NULL);

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
             "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>n_methods</TD><TD>%u</TD></TR>\n",
             list->n_methods);
   len = strlen( tmp[ 0]);

   for( i = 0; i < list->n_methods; i++)
   {
      asprintf( &tmp[ i + 1], "<TR><TD>%s</TD>"
               "<TD><TABLE>"
               "<TR><TD>signature</TD><TD>%s</TD></TR>"
               "<TR><TD>methodid</TD><TD>0x%lx</TD></TR>"
               "<TR><TD>bits</TD><TD>0x%x</TD></TR>"
               "<TR><TD>implementation</TD><TD>%p</TD></TR>"
               "</TABLE></TD>"
               "</TR>\n",
               list->methods[ i].descriptor.name,
               list->methods[ i].descriptor.signature,
               (long) list->methods[ i].descriptor.methodid,
               list->methods[ i].descriptor.bits,
               list->methods[ i].implementation);
      len += strlen( tmp[ i + 1]);
   }

   asprintf( &tmp[ i + 1], "</TABLE>");
   len += strlen( tmp[ i + 1]);

   // concatenate all strings

   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   free( tmp);

   return( s);
}


char  *mulle_objc_methodlist_html_hor_description( struct _mulle_objc_methodlist *list)
{
   size_t         len;
   char           **tmp;
   char           *s;
   unsigned int   i;
   unsigned int   n;

   n   = list->n_methods + 3;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return(NULL);

   // create single lines for each method and two for head/tail
   asprintf( &tmp[ 0],
             "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>n_methods</TD><TD>%u</TD></TR>\n",
             list->n_methods);
   len = strlen( tmp[ 0]);

   asprintf( &tmp[ 1], "<TR><TH>name</TH>"
               "<TH>signature</TH>"
               "<TH>methodid</TH>"
               "<TH>bits</TH>"
               "<TH>implementation</TH>"
               "</TR>\n");
   len += strlen( tmp[ 1]);

   for( i = 0; i < list->n_methods; i++)
   {
      asprintf( &tmp[ i + 2], "<TR><TD>%s</TD>"
               "<TD>%s</TD>"
               "<TD>%08x</TD>"
               "<TD>0x%x</TD>"
               "<TD>%p</TD>"
               "</TR>\n",
               list->methods[ i].descriptor.name,
               list->methods[ i].descriptor.signature,
               list->methods[ i].descriptor.methodid,
               list->methods[ i].descriptor.bits,
               list->methods[ i].implementation);
      len += strlen( tmp[ i + 2]);
   }

   asprintf( &tmp[ i + 2], "</TABLE>");
   len += strlen( tmp[ i + 2]);

   // concatenate all strings

   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }

   free( tmp);

   return( s);
}


char  *mulle_concurrent_pointerarray_html_description( struct mulle_concurrent_pointerarray *list)
{
   struct mulle_concurrent_pointerarrayenumerator   rover;
   size_t                  count;
   size_t                  len;
   char                    **tmp;
   char                    *s;
   unsigned int            i;
   unsigned int            n;
   void                    *value;
   
   count = mulle_concurrent_pointerarray_get_count( list);

   n   = (unsigned int) count + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return( NULL);
   
   asprintf( &tmp[ 0],
            "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>count</TD><TD>%lu</TD></TR>\n",
            count);
   len = strlen( tmp[ 0]);
   
   i = 1;
   rover = mulle_concurrent_pointerarray_enumerate( list);
   while( value = _mulle_concurrent_pointerarrayenumerator_next( &rover))
   {
      asprintf( &tmp[ i],"<TR><TD COLSPAN=\"2\">%p</TD></TR>\n", value);
      len += strlen( tmp[ i]);
      ++i;
   }
   
   asprintf( &tmp[ i], "</TABLE>");
   len += strlen( tmp[ i]);
   
   // concatenate all strings
   
   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }
   
   free( tmp);
   
   return( s);
}


char  *mulle_concurrent_hashmap_html_description( struct mulle_concurrent_hashmap *map,
                                                  char *(value_description)( void *))
{
   struct mulle_concurrent_hashmapenumerator   rover;
   char                    *null_description;
   intptr_t                uniqueid;
   size_t                  count;
   size_t                  len;
   char                    **tmp;
   char                    *s;
   unsigned int            i;
   unsigned int            n;
   void                    *value;
   
   count = mulle_concurrent_hashmap_count( map);

   n   = (unsigned int) count + 2;
   tmp = calloc( n, sizeof( char *));
   if( ! tmp)
      return(NULL);
   
   
   asprintf( &tmp[ 0],
            "<TABLE>\n<TR BGCOLOR=\"gray\"><TD>n_entries</TD><TD>%lu</TD></TR>\n",
            count);
   
   len = strlen( tmp[ 0]);
   
   null_description = "*null*";
   
   i = 1;
   rover = mulle_concurrent_hashmap_enumerate( map);
   while( _mulle_concurrent_hashmapenumerator_next( &rover, &uniqueid, &value))
   {
      s = value ? value_description( value) : null_description;
      asprintf( &tmp[ i],"<TR><TD COLSPAN=\"2\">%s</TD></TR>\n", s);
      if( s != null_description)
         free( s);
      
      len += strlen( tmp[ i]);
      ++i;
   }
   mulle_concurrent_hashmapenumerator_done( &rover);
   
   asprintf( &tmp[ i], "</TABLE>");
   len += strlen( tmp[ i]);
   
   // concatenate all strings
   
   s = realloc( tmp[ 0], len + 1);
   for( i = 1; i < n; i++)
   {
      strcat( s, tmp[ i]);
      free( tmp[ i]);
   }
   
   free( tmp);
   
   return( s);
}


//
//  mulle_objc_load.h
//  mulle-objc
//
//  Created by Nat! on 19.11.14.
//  Copyright (c) 2014 Nat! - Mulle kybernetiK.
//  Copyright (c) 2014 Codeon GmbH.
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
//

#ifndef mulle_objc_load_h__
#define mulle_objc_load_h__

#include <stddef.h>
#include <stdint.h>

#include "mulle_objc_uniqueid.h"


struct _mulle_objc_category;
struct _mulle_objc_callqueue;
struct _mulle_objc_class;
struct _mulle_objc_ivarlist;
struct _mulle_objc_methodlist;
struct _mulle_objc_propertylist;


struct _mulle_objc_loadclass
{
   mulle_objc_classid_t              classid;
   char                              *class_name;
   mulle_objc_hash_t                 class_ivarhash;

   mulle_objc_classid_t              superclassid;
   char                              *superclass_name;
   mulle_objc_hash_t                 superclass_ivarhash;

   int                               fastclassindex;
   int                               instance_size;
   
   struct _mulle_objc_ivarlist       *instance_variables;
   
   struct _mulle_objc_methodlist     *class_methods;
   struct _mulle_objc_methodlist     *instance_methods;
   struct _mulle_objc_propertylist   *properties;
   
   mulle_objc_protocolid_t           *protocol_uniqueids;
};


struct _mulle_objc_loadcategory
{
   char                              *category_name;

   mulle_objc_classid_t              classid;
   char                              *class_name;         // useful ??
   mulle_objc_hash_t                 class_ivarhash;
   
   struct _mulle_objc_methodlist     *class_methods;
   struct _mulle_objc_methodlist     *instance_methods;
   struct _mulle_objc_propertylist   *properties;

   mulle_objc_protocolid_t           *protocol_uniqueids;
};


struct _mulle_objc_loadclasslist
{
   unsigned int                    n_loadclasses;
   struct _mulle_objc_loadclass    *loadclasses[ 1];
};


static inline size_t  mulle_objc_size_of_loadclasslist( unsigned int n_loadclasses)
{
   return( sizeof( struct _mulle_objc_loadclasslist) + (n_loadclasses - 1) * sizeof( struct _mulle_objc_loadclass *));
}


struct _mulle_objc_loadcategorylist
{
   unsigned int                      n_loadcategories;
   struct _mulle_objc_loadcategory   *loadcategories[ 1];
};


static inline size_t  mulle_objc_size_of_loadcategorylist( unsigned int n_load_categories)
{
   return( sizeof( struct _mulle_objc_loadcategorylist) + (n_load_categories - 1) * sizeof( struct _mulle_objc_loadcategory *));
}


struct _mulle_objc_loadstringlist
{
   unsigned int                n_loadstrings;
   struct _mulle_objc_object   *loadstrings[ 1];
};


struct _mulle_objc_loadhashedstring
{
   mulle_objc_uniqueid_t   uniqueid;
   char                    *string;
};


struct _mulle_objc_loadhashedstringlist
{
   unsigned int                          n_loadentries;
   struct _mulle_objc_loadhashedstring   loadentries[ 1];
};


int   _mulle_objc_loadhashedstring_compare( struct _mulle_objc_loadhashedstring *a,
                                            struct _mulle_objc_loadhashedstring *b);

void   mulle_objc_loadhashedstring_sort( struct _mulle_objc_loadhashedstring *methods,
                                         unsigned int n);


char   *_mulle_objc_loadhashedstring_bsearch( struct _mulle_objc_loadhashedstring *buf,
                                              unsigned int n,
                                              mulle_objc_ivarid_t search);

static inline char   *mulle_objc_loadhashedstringlist_bsearch( struct _mulle_objc_loadhashedstringlist *map,
                                                          mulle_objc_ivarid_t search)
{
   if( map)
      return( _mulle_objc_loadhashedstring_bsearch( map->loadentries, map->n_loadentries, search));
   return( NULL);
}


static inline void   mulle_objc_loadhashedstringlist_sort( struct _mulle_objc_loadhashedstringlist *map)
{
   if( map)
      mulle_objc_loadhashedstring_sort( map->loadentries, map->n_loadentries);
}


//
// this adds version info to the loaded classes and the
// categories. .
//
enum _mulle_objc_loadinfobits
{
   _mulle_objc_loadinfo_unsorted     = 0x1,
   _mulle_objc_loadinfo_aaomode      = 0x2,
   _mulle_objc_loadinfo_notaggedptrs = 0x4,
   _mulle_objc_loadinfo_optlevel_0   = (0 << 16),
   _mulle_objc_loadinfo_optlevel_1   = (1 << 16),
   _mulle_objc_loadinfo_optlevel_2   = (2 << 16),
   _mulle_objc_loadinfo_optlevel_3   = (3 << 16),
   _mulle_objc_loadinfo_optlevel_s   = (7 << 16)
};


//
// The objc_version is the MULLE_OBJC_RUNTIME_VERSION built into the compiler.
// The foundation version is necessary for the fastcalls. Since the foundation
// defines the fastcalls and fastclasses, the foundation must match.
// Whenever the fastcall/fastclasses change you need to update the foundation
// number.
// There is no minor/major scheme with respect to foundation.
// it's not underscored, coz it's public by design

struct mulle_objc_loadversion
{
   uint32_t   runtime;
   uint32_t   foundation;
   uint32_t   user;
   uint32_t   bits;
};


struct _mulle_objc_loadinfo
{
   struct mulle_objc_loadversion             version;
   
   struct _mulle_objc_loadclasslist          *loadclasslist;
   struct _mulle_objc_loadcategorylist       *loadcategorylist;
   struct _mulle_objc_loadstringlist         *loadstringlist;
   struct _mulle_objc_loadhashedstringlist   *loadhashedstringlist;  // optional for debugging
};



# pragma mark  -
# pragma mark master calls

//
// use this if the compiler was able to sort all protocol_uniqueids
// all method lists referenced by load_categories and loadclasses by their methodid
//
void   mulle_objc_loadinfo_unfailing_enqueue( struct _mulle_objc_loadinfo *info);


# pragma mark  -
# pragma mark class

void   mulle_objc_loadclass_unfailing_enqueue( struct _mulle_objc_loadclass *info);


# pragma mark  -
# pragma mark category

int    mulle_objc_loadcategory_enqueue( struct _mulle_objc_loadcategory *info, struct _mulle_objc_callqueue *loads);
void   mulle_objc_loadcategory_unfailing_enqueue( struct _mulle_objc_loadcategory *info, struct _mulle_objc_callqueue *loads);

#endif

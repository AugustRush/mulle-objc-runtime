//
//  mulle_objc_fast_call.h
//  mulle-objc
//
//  Created by Nat! on 26/10/15.
//  Copyright (c) 2015 Nat! - Mulle kybernetiK.
//  Copyright (c) 2015 Codeon GmbH.
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
#ifndef mulle_objc_fastmethodtable_h__
#define mulle_objc_fastmethodtable_h__

#include "mulle_objc_method.h"

#include <mulle_thread/mulle_thread.h>
#include <mulle_aba/mulle_aba.h>
#include <mulle_c11/mulle_c11.h>


#define MULLE_OBJC_S_FASTMETHODS    24

//
// Keep this table small, as this struct will be embedded in each class
// also you need to change quite a bit of code...
//
union _mulle_objc_atomicmethodpointer_t
{
   struct _mulle_objc_method    *method;      // dont read, except when debugging
   mulle_atomic_pointer_t       pointer;
};


struct _mulle_objc_fastmethodtable
{
   union _mulle_objc_atomicmethodpointer_t   methods[ MULLE_OBJC_S_FASTMETHODS];
};


// setup fault handlers for the method table
void _mulle_objc_fastmethodtable_init( struct _mulle_objc_fastmethodtable *table);


MULLE_C_ALWAYS_INLINE
static inline void   *_mulle_objc_fastmethodtable_invoke( void *obj,
                                                          mulle_objc_methodid_t methodid,
                                                          void *param,
                                                          struct _mulle_objc_fastmethodtable *table,
                                                          unsigned int index)
{
   mulle_objc_methodimplementation_t   imp;
   
   imp = (mulle_objc_methodimplementation_t) _mulle_atomic_pointer_read( &table->methods[ index].pointer);
   return( (*imp)( obj, methodid, param));
}
   

//
// 8 basic functions are predefined, it's not that these need
// to be very speedy, but being fastmethods, they also reduce the generated
// code-size a lot for the inlineable code and the compiler generated code.
//
MULLE_C_CONST_RETURN
static inline int   mulle_objc_get_fastmethodtable_index( mulle_objc_methodid_t methodid)
{
   switch( methodid)
   {
   case MULLE_OBJC_ALLOC_METHODID       : return( 0);
   case MULLE_OBJC_INIT_METHODID        : return( 1);  // makes new faster
   case MULLE_OBJC_FINALIZE_METHODID    : return( 2); 
   case MULLE_OBJC_DEALLOC_METHODID     : return( 3);  // in AAO mode noone can call dealloc
   case MULLE_OBJC_INSTANTIATE_METHODID : return( 4);  // alloc + autorelease
   case MULLE_OBJC_AUTORELEASE_METHODID : return( 5);  // for compiler
   case MULLE_OBJC_RETAIN_METHODID      : return( 6);  // not used in -O2 and up
   case MULLE_OBJC_RELEASE_METHODID     : return( 7);  // not used in -O2 and up
         
#ifdef MULLE_OBJC_FASTMETHODHASH_8
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_8 ) : return( 8);
#endif
#ifdef MULLE_OBJC_FASTMETHODHASH_9
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_9 ) : return( 9);
#endif
#ifdef MULLE_OBJC_FASTMETHODHASH_10
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_10) : return( 10);
#endif
#ifdef MULLE_OBJC_FASTMETHODHASH_11
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_11) : return( 11);
#endif
         
#ifdef MULLE_OBJC_FASTMETHODHASH_12
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_12) : return( 12);
#endif
#ifdef MULLE_OBJC_FASTMETHODHASH_13
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_13) : return( 13);
#endif
#ifdef MULLE_OBJC_FASTMETHODHASH_14
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_14) : return( 14);
#endif
#ifdef MULLE_OBJC_FASTMETHODHASH_15
   case MULLE_OBJC_METHODID( MULLE_OBJC_FASTMETHODHASH_15) : return( 15);
#endif
   }
   return( -1);
}


#endif /* mulle_objc_fastmethodtable_h */

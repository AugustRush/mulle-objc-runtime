//
//  mulle_objc_uniqueid.h
//  mulle-objc
//
//  Created by Nat! on 15.09.15.
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

//

#ifndef mulle_objc_uniqueid_h__
#define mulle_objc_uniqueid_h__

#include <stdint.h>


// it's signed, because it sorts easier

typedef uint32_t   mulle_objc_uniqueid_t;


#define MULLE_OBJC_NO_UNIQUEID         ((mulle_objc_uniqueid_t) 0)
#define MULLE_OBJC_INVALID_UNIQUEID    ((mulle_objc_uniqueid_t) -1)
#define MULLE_OBJC_MIN_UNIQUEID        ((mulle_objc_uniqueid_t) 1)


// everything is 32 bit since fnv1
#define MULLE_OBJC_UNIQUEID( x)  ((mulle_objc_uniqueid_t) (x))

#if 0
# ifdef __LP64__
#  define MULLE_OBJC_UNIQUEID( x)  ((mulle_objc_uniqueid_t) ((x) >> 32))
# else
#  define MULLE_OBJC_UNIQUEID( x)  ((mulle_objc_uniqueid_t) ((x) >> 32))
# endif
#endif

mulle_objc_uniqueid_t  mulle_objc_uniqueid_from_string( char *s);

// for compiler purposes it's better to typedef the various id types here
// (include cycles..)


#pragma mark -
#pragma mark mulle_objc_classid_t

typedef mulle_objc_uniqueid_t   mulle_objc_classid_t;

#define MULLE_OBJC_CLASSID( x)           MULLE_OBJC_UNIQUEID( x)

#define MULLE_OBJC_NO_CLASSID            MULLE_OBJC_NO_UNIQUEID
#define MULLE_OBJC_INVALID_CLASSID       MULLE_OBJC_INVALID_UNIQUEID


#pragma mark -
#pragma mark mulle_objc_ivarid_t

typedef mulle_objc_uniqueid_t   mulle_objc_ivarid_t;

#define MULLE_OBJC_IVARID( x)       MULLE_OBJC_UNIQUEID(x)

#define MULLE_OBJC_NO_IVARID        MULLE_OBJC_NO_UNIQUEID
#define MULLE_OBJC_INVALID_IVARID   MULLE_OBJC_INVALID_UNIQUEID


#pragma mark -
#pragma mark mulle_objc_methodid_t

typedef mulle_objc_uniqueid_t      mulle_objc_methodid_t;

#define MULLE_OBJC_METHODID( x)       MULLE_OBJC_UNIQUEID(x)

#define MULLE_OBJC_NO_METHODID        MULLE_OBJC_NO_UNIQUEID
#define MULLE_OBJC_INVALID_METHODID   MULLE_OBJC_INVALID_UNIQUEID


#pragma mark -
#pragma mark mulle_objc_propertyid_t

typedef mulle_objc_uniqueid_t      mulle_objc_propertyid_t;

#define MULLE_OBJC_PROPERTYID( x)       MULLE_OBJC_UNIQUEID( x)

#define MULLE_OBJC_NO_PROPERTYID        MULLE_OBJC_NO_UNIQUEID
#define MULLE_OBJC_INVALID_PROPERTYID   MULLE_OBJC_INVALID_UNIQUEID


#pragma mark -
#pragma mark mulle_objc_protocolid_t

typedef mulle_objc_uniqueid_t      mulle_objc_protocolid_t;

#define MULLE_OBJC_PROTOCOLID( x)       MULLE_OBJC_UNIQUEID( x)

#define MULLE_OBJC_NO_PROTOCOLID        MULLE_OBJC_NO_UNIQUEID
#define MULLE_OBJC_INVALID_PROTOCOLID   MULLE_OBJC_INVALID_UNIQUEID



#pragma mark -
#pragma mark mulle_objc_hash_t

typedef mulle_objc_uniqueid_t   mulle_objc_hash_t;


# pragma mark -
# pragma mark API

static inline mulle_objc_classid_t   mulle_objc_classid_from_string( char *s)
{
   return( mulle_objc_uniqueid_from_string( s));
}


static inline mulle_objc_ivarid_t   mulle_objc_ivarid_from_string( char *s)
{
   return( mulle_objc_uniqueid_from_string( s));
}


static inline mulle_objc_methodid_t   mulle_objc_methodid_from_string( char *s)
{
   return( mulle_objc_uniqueid_from_string( s));
}


static inline mulle_objc_propertyid_t   mulle_objc_propertyid_from_string( char *s)
{
   return( mulle_objc_uniqueid_from_string( s));
}


static inline mulle_objc_protocolid_t   mulle_objc_protocolid_from_string( char *s)
{
   return( mulle_objc_uniqueid_from_string( s));
}


#endif

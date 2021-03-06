//  File implement/oglplus/enums/face_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/face.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
OGLPLUS_LIB_FUNC StrCRef ValueName_(
	Face*,
	GLenum value
)
#if (!OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(OGLPLUS_IMPL_EVN_FACE)
#define OGLPLUS_IMPL_EVN_FACE
{
switch(value)
{
#if defined GL_FRONT
	case GL_FRONT: return StrCRef("FRONT");
#endif
#if defined GL_BACK
	case GL_BACK: return StrCRef("BACK");
#endif
#if defined GL_FRONT_AND_BACK
	case GL_FRONT_AND_BACK: return StrCRef("FRONT_AND_BACK");
#endif
	default:;
}
OGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums


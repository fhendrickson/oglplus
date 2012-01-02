/**
 *  @file oglplus/sampler.hpp
 *  @brief Sampler object wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef OGLPLUS_SAMPLER_1107121519_HPP
#define OGLPLUS_SAMPLER_1107121519_HPP

#include <oglplus/config.hpp>
#include <oglplus/glfunc.hpp>
#include <oglplus/error.hpp>
#include <oglplus/object.hpp>
#include <oglplus/friend_of.hpp>
#include <oglplus/texture.hpp>
#include <cassert>

namespace oglplus {

// if samplers are available
#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_3_3 || GL_ARB_sampler_objects

/// Encapsulates sampler-related functions
/** @note Do not use this class directly, use Sampler instead.
 *  @see Sampler
 *
 *  @glsymbols
 *  @glfunref{GenSamplers}
 *  @glfunref{DeleteSamplers}
 *  @glfunref{IsSampler}
 */
class SamplerOps
 : public Named
{
protected:
	static void _init(GLsizei count, GLuint& _name)
	{
		OGLPLUS_GLFUNC(GenSamplers)(count, &_name);
		HandleIfError(OGLPLUS_ERROR_INFO(GenSamplers));
	}

	static void _cleanup(GLsizei count, GLuint& _name)
	{
		assert(_name != 0);
		OGLPLUS_GLFUNC(DeleteSamplers)(count, &_name);
	}

	static GLboolean _is_x(GLuint _name)
	{
		assert(_name != 0);
		return OGLPLUS_GLFUNC(IsSampler)(_name);
	}

	friend class FriendOf<SamplerOps>;
public:
	/// Bind this sampler to the specified texture unit
	/**
	 *  @glsymbols
	 *  @glfunref{BindSampler}
	 */
	void Bind(TextureUnitSelector unit) const
	{
		assert(_name != 0);
		OGLPLUS_GLFUNC(BindSampler)(GLuint(unit), _name);
		AssertNoError(OGLPLUS_ERROR_INFO(BindSampler));
	}

	/// Unbind the current sampler from the specified texture unit
	/**
	 *  @glsymbols
	 *  @glfunref{BindSampler}
	 */
	static void Unbind(TextureUnitSelector unit)
	{
		OGLPLUS_GLFUNC(BindSampler)(GLuint(unit), 0);
		AssertNoError(OGLPLUS_ERROR_INFO(BindSampler));
	}

	/// Sets the texture border color
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_BORDER_COLOR}
	 */
	void BorderColor(Vector<GLfloat, 4> color) const
	{
		OGLPLUS_GLFUNC(SamplerParameterfv)(
			_name,
			GL_TEXTURE_BORDER_COLOR,
			Data(color)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameterfv));
	}

	/// Sets the texture border color
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_BORDER_COLOR}
	 */
	void BorderColor(Vector<GLint, 4> color) const
	{
		OGLPLUS_GLFUNC(SamplerParameterIiv)(
			_name,
			GL_TEXTURE_BORDER_COLOR,
			Data(color)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameterIiv));
	}

	/// Sets the texture border color
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_BORDER_COLOR}
	 */
	void BorderColor(Vector<GLuint, 4> color) const
	{
		OGLPLUS_GLFUNC(SamplerParameterIuiv)(
			_name,
			GL_TEXTURE_BORDER_COLOR,
			Data(color)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameterIuiv));
	}

	/// Sets the compare mode
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_COMPARE_MODE}
	 */
	void CompareMode(TextureCompareMode mode) const
	{
		OGLPLUS_GLFUNC(SamplerParameteri)(
			_name,
			GL_TEXTURE_COMPARE_MODE,
			GLenum(mode)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameteri));
	}

	/// Sets the compare function
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_COMPARE_FUNC}
	 */
	void CompareFunc(CompareFunction func) const
	{
		OGLPLUS_GLFUNC(SamplerParameteri)(
			_name,
			GL_TEXTURE_COMPARE_FUNC,
			GLenum(func)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameteri));
	}

	/// Sets the LOD bias value
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_LOD_BIAS}
	 */
	void LODBias(GLfloat value) const
	{
		OGLPLUS_GLFUNC(SamplerParameterf)(
			_name,
			GL_TEXTURE_LOD_BIAS,
			value
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameterf));
	}

	/// Sets the magnification filter
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_MAG_FILTER}
	 */
	void MagFilter(TextureMagFilter filter) const
	{
		OGLPLUS_GLFUNC(SamplerParameteri)(
			_name,
			GL_TEXTURE_MAG_FILTER,
			GLenum(filter)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameteri));
	}

	/// Sets the minification filter
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_MIN_FILTER}
	 */
	void MinFilter(TextureMinFilter filter) const
	{
		OGLPLUS_GLFUNC(SamplerParameteri)(
			_name,
			GL_TEXTURE_MIN_FILTER,
			GLenum(filter)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameteri));
	}

	/// Sets minimal LOD value
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_MIN_LOD}
	 */
	void MinLOD(GLfloat value) const
	{
		OGLPLUS_GLFUNC(SamplerParameterf)(
			_name,
			GL_TEXTURE_MIN_LOD,
			value
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameterf));
	}

	/// Sets maximal LOD value
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_MAX_LOD}
	 */
	void MaxLOD(GLfloat value) const
	{
		OGLPLUS_GLFUNC(SamplerParameterf)(
			_name,
			GL_TEXTURE_MAX_LOD,
			value
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameterf));
	}

	/// Sets the wrap parameter (TEXTURE_WRAP_*)
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 */
	void Wrap(TextureWrapCoord coord, TextureWrap mode) const
	{
		OGLPLUS_GLFUNC(SamplerParameteri)(
			_name,
			GLenum(coord),
			GLenum(mode)
		);
		HandleIfError(OGLPLUS_ERROR_INFO(SamplerParameteri));
	}

	/// Sets the wrap parameter for the S coordinate
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_WRAP_S}
	 */
	void WrapS(TextureWrap mode) const
	{
		Wrap(TextureWrapCoord::S, mode);
	}

	/// Sets the wrap parameter for the T coordinate
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_WRAP_T}
	 */
	void WrapT(TextureWrap mode) const
	{
		Wrap(TextureWrapCoord::T, mode);
	}

	/// Sets the wrap parameter for the R coordinate
	/**
	 *  @glsymbols
	 *  @glfunref{SamplerParameter}
	 *  @gldefref{TEXTURE_WRAP_R}
	 */
	void WrapR(TextureWrap mode) const
	{
		Wrap(TextureWrapCoord::R, mode);
	}
};

#if OGLPLUS_DOCUMENTATION_ONLY
/// An @ref oglplus_object encapsulating the OpenGL sampler functionality
/**
 *  @ingroup objects
 */
class Sampler
 : public SamplerOps
{ };
#else
typedef Object<SamplerOps, true> Sampler;
#endif

#endif // sampler object

} // namespace oglplus

#endif // include guard

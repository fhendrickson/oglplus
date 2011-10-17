/**
 *  @file example/example.hpp
 *  @brief Declares a common base class for examples
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef __OGLPLUS_EXAMPLE_EXAMPLE_1119071146_HPP__
#define __OGLPLUS_EXAMPLE_EXAMPLE_1119071146_HPP__

#include <memory>

#ifndef _NDEBUG
#include <iostream>
#endif

namespace oglplus {

struct Example
{
	virtual ~Example(void)
	{ }

	// Hint for the main function whether to continue rendering
	// Implementations of the main function may choose to ignore
	// the result of this function
	virtual bool Continue(double duration)
	{
		return duration < 3.0; // [seconds]
	}

	virtual void Reshape(size_t width, size_t height) = 0;

	virtual bool UsesMouseMotion(void) const
	{
		return false;
	}

	virtual void MouseMoveNormalized(float x, float y, float aspect)
	{
	}

	virtual void MouseMove(size_t x, size_t y, size_t width, size_t height)
	{
		return MouseMoveNormalized(
			(float(x) - width * 0.5f) / (width * 0.5f),
			(float(y) - height* 0.5f) / (height* 0.5f),
			float(width)/height
		);
	}

	virtual void Render(double time) = 0;
};

std::unique_ptr<Example> makeExample(void);

} // namespace oglplus

#endif // include guard

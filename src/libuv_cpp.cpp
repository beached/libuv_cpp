// The MIT License (MIT)
//
// Copyright (c) 2018 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <daw/libuv_cpp.h>

#include "daw/libuv_cpp.h"

namespace daw {
	libuv_loop::libuv_loop( ) noexcept {}

	libuv_loop::libuv_loop( new_uv_loop_t ) noexcept
	  : m_loop( new ::uv_loop_t{} )
	  , m_owns_loop( true ) {

		::uv_loop_init( ptr( ) );
	}

	libuv_loop::libuv_loop( libuv_loop::pointer p, bool take_ownership ) noexcept
	  : m_loop( p )
	  , m_owns_loop( take_ownership ) {}

	libuv_loop::~libuv_loop( ) noexcept {
		auto tmp = std::exchange( m_loop, nullptr );
		if( m_owns_loop && tmp ) {
			::uv_loop_close( tmp );
			delete tmp;
			m_owns_loop = false;
		}
	}

	libuv_loop::operator libuv_loop::const_pointer( ) const noexcept {
		return ptr( );
	}

	libuv_loop::operator libuv_loop::pointer( ) noexcept {
		return ptr( );
	}

	int libuv_loop::close( ) noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		return ::uv_loop_close( ptr( ) );
	}

	int libuv_loop::run( uv_run_mode mode ) noexcept(
	  !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		return ::uv_run( ptr( ), mode );
	}

	bool libuv_loop::alive( ) const noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		return ::uv_loop_alive( ptr( ) ) != 0;
	}

	void libuv_loop::stop( ) noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		::uv_stop( ptr( ) );
	}

	size_t libuv_loop::uv_loop_size( ) noexcept {
		return ::uv_loop_size( );
	}

	int libuv_loop::back_end_fd( ) const
	  noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );
		return ::uv_backend_fd( ptr( ) );
	}

	int libuv_loop::back_end_timeout( ) const
	  noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		return ::uv_backend_timeout( ptr( ) );
	}

	std::chrono::milliseconds libuv_loop::now( ) const
	  noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		return std::chrono::milliseconds( ::uv_now( ptr( ) ) );
	}

	void libuv_loop::update_time( ) noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		::uv_update_time( ptr( ) );
	}

	void *libuv_loop::get_data( ) const noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		return ::uv_loop_get_data( ptr( ) );
	}

	void
	libuv_loop::set_data( void *data ) noexcept( !daw::exception::may_throw_v ) {
		daw::exception::precondition_check(
		  ptr( ), "Expected a loop that has not been closed" );

		::uv_loop_set_data( ptr( ), data );
	}

	libuv_loop::pointer libuv_loop::release( ) noexcept {
		daw::exception::precondition_check(
		  m_owns_loop, "Attempt to release an unowned pointer" );

		m_owns_loop = false;
		return ptr( );
	}
} // namespace daw

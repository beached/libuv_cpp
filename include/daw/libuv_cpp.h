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

#pragma once

#include <chrono>
#include <memory>
#include <uv.h>

#include <daw/daw_exception.h>
#include <daw/daw_traits.h>

namespace daw {
	class libuv_loop {
		std::unique_ptr<uv_loop_t> m_loop = std::make_unique<uv_loop_t>( );

	public:
		libuv_loop( ) noexcept {
			uv_loop_init( m_loop.get( ) );
		}

		libuv_loop( libuv_loop && ) noexcept = default;
		libuv_loop &operator=( libuv_loop && ) noexcept = default;

		~libuv_loop( ) {
			if( auto tmp = std::move( m_loop ); tmp ) {
				uv_loop_close( tmp.get( ) );
			}
		}

		operator uv_loop_t const *( ) const noexcept {
			return m_loop.get( );
		}

		operator uv_loop_t *( ) noexcept {
			return m_loop.get( );
		}

		template<typename... UV_Loop_Options,
		         std::enable_if_t<
		           daw::traits::are_same_types_v<std::decay_t<UV_Loop_Options>...>,
		           std::nullptr_t> = nullptr>
		int configure( UV_Loop_Options &&... options ) noexcept(
		  !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			return ::uv_loop_configure( m_loop.get( ),
			                            std::forward<UV_Loop_Options>( options )... );
		}

		int close( ) noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			return ::uv_loop_close( m_loop.get( ) );
		}

		int run( ::uv_run_mode mode ) noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			return ::uv_run( m_loop.get( ), mode );
		}

		bool alive( ) const noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			return ::uv_loop_alive( m_loop.get( ) ) != 0;
		}

		void stop( ) noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			::uv_stop( m_loop.get( ) );
		}

		static size_t uv_loop_size( ) noexcept {
			return ::uv_loop_size( );
		}

		int back_end_fd( ) const noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			return ::uv_backend_fd( m_loop.get( ) );
		}

		int back_end_timeout( ) const noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			return ::uv_backend_timeout( m_loop.get( ) );
		}

		std::chrono::milliseconds now( ) const
		  noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			return std::chrono::milliseconds( ::uv_now( m_loop.get( ) ) );
		}

		void update_time( ) noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );
			::uv_update_time( m_loop.get( ) );
		}

	private:
		template<typename Callback>
		static void walk_cb( uv_handle_t *ptr, void *vfunc ) noexcept(
		  daw::is_nothrow_callable_v<Callback, uv_handle_t &> ) {

			( *static_cast<Callback *>( vfunc ) )( *ptr );
		}

	public:
		void walk( uv_walk_cb walk_cb, void *arg ) noexcept(
		  !daw::exception::can_throw_v &&
		  noexcept( walk_cb( std::declval<uv_handle_t *>( ),
		                     std::declval<void *>( ) ) ) ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			::uv_walk( m_loop.get( ), walk_cb, arg );
		}

		template<typename Callback,
		         std::enable_if_t<daw::is_callable_v<Callback, uv_handle_t &>,
		                          std::nullptr_t> = nullptr>
		void walk( Callback &&cb ) noexcept(
		  !daw::exception::can_throw_v &&
		  daw::is_nothrow_callable_v<Callback, uv_handle_t &> ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			::uv_walk( m_loop.get( ), walk_cb<Callback>, static_cast<void *>( &std::forward<Callback>( cb ) ) );
		}

		void *get_data( ) const noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			return ::uv_loop_get_data( m_loop.get( ) );
		}

		void set_data( void *data ) noexcept( !daw::exception::can_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			::uv_loop_set_data( m_loop.get( ), data );
		}
	};
} // namespace daw

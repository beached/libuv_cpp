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
#include <daw/daw_value_ptr.h>

namespace daw {
	// tag to indicate we want a new loop and not the
	// uv_default_loop( )
	struct new_uv_loop_t {};
	inline constexpr new_uv_loop_t new_uv_loop = new_uv_loop_t{};

	struct libuv_loop {
		using pointer = ::uv_loop_t *;
		using const_pointer = ::uv_loop_t const *;

	private:
		pointer m_loop = ::uv_default_loop( );
		bool m_owns_loop = false;

		inline pointer ptr( ) noexcept {
			return m_loop;
		}

		inline const_pointer ptr( ) const noexcept {
			return m_loop;
		}

	public:
		libuv_loop( ) noexcept;
		explicit libuv_loop( new_uv_loop_t ) noexcept;
		explicit libuv_loop( pointer p, bool take_owner_ship = false ) noexcept;

		libuv_loop( libuv_loop && ) noexcept = default;
		libuv_loop &operator=( libuv_loop && ) noexcept = default;

		~libuv_loop( ) noexcept;

		operator const_pointer( ) const noexcept;

		operator pointer( ) noexcept;

		pointer release( ) noexcept;

		template<typename... UV_Loop_Options,
		         std::enable_if_t<
		           daw::traits::are_same_types_v<std::decay_t<UV_Loop_Options>...>,
		           std::nullptr_t> = nullptr>
		int configure( UV_Loop_Options &&... options ) noexcept(
		  !daw::exception::may_throw_v ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			return ::uv_loop_configure( ptr( ),
			                            std::forward<UV_Loop_Options>( options )... );
		}

		int close( ) noexcept( !daw::exception::may_throw_v );

		int run( ::uv_run_mode mode = ::uv_run_mode::UV_RUN_DEFAULT ) noexcept(
		  !daw::exception::may_throw_v );

		bool alive( ) const noexcept( !daw::exception::may_throw_v );

		void stop( ) noexcept( !daw::exception::may_throw_v );

		static size_t uv_loop_size( ) noexcept;

		int back_end_fd( ) const noexcept( !daw::exception::may_throw_v );

		int back_end_timeout( ) const noexcept( !daw::exception::may_throw_v );

		std::chrono::milliseconds now( ) const
		  noexcept( !daw::exception::may_throw_v );

		void update_time( ) noexcept( !daw::exception::may_throw_v );

	private:
		template<typename Callback>
		static void walk_cb( uv_handle_t *p, void *vfunc ) noexcept(
		  daw::is_nothrow_callable_v<Callback, uv_handle_t &> ) {

			( *static_cast<Callback *>( vfunc ) )( *p );
		}

	public:
		template<typename Callback,
		         std::enable_if_t<daw::is_callable_v<Callback, uv_handle_t &>,
		                          std::nullptr_t> = nullptr>
		void walk( Callback &&cb ) noexcept(
		  !daw::exception::may_throw_v &&
		  daw::is_nothrow_callable_v<Callback, uv_handle_t &> ) {
			daw::exception::precondition_check(
			  m_loop, "Expected a loop that has not been closed" );

			::uv_walk( ptr( ), walk_cb<Callback>,
			           static_cast<void *>( &std::forward<Callback>( cb ) ) );
		}

		void *get_data( ) const noexcept( !daw::exception::may_throw_v );

		void set_data( void *data ) noexcept( !daw::exception::may_throw_v );
	};
} // namespace daw

// The MIT License (MIT)
//
// Copyright (c) 2018 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <memory>
#include <uv.h>

namespace daw {
	class libuv_queue {
		std::unique_ptr<uv_loop_t> m_loop = std::make_unique<uv_loop_t>( );

	public:
		libuv_queue( ) noexcept {
			uv_loop_init( m_data.data( ) );	
		};

		libuv_queue( libuv_queue && ) noexcept = default;
		libuv_queue &operator=( libuv_queue && ) noexcept = default;

		~libuv_queue( ) {
			if( auto tmp = std::move( m_data ); tmp ) {
				uv_loop_close( tmp.data( ) );
			}
		}

		uv_loop_init const * operator const noexcept {
			return m_data.data( );
		}

		uv_loop_init * operator noexcept {
			return m_data.data( );
		}

		template<typename... UV_Loop_Options>
		int configure( uv_loop_option option ) noexcept {

		}
	};
} // namespace daw


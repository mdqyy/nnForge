/*
 *  Copyright 2011-2013 Maxim Milakov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#pragma once

#include "dropout_layer_config.h"

#include <vector>
#include <ostream>
#include <istream>
#include <memory>

namespace nnforge
{
	class layer_data : public std::vector<std::vector<float> >
	{
	public:
		layer_data();

		// The stream should be created with std::ios_base::binary flag
		void write(std::ostream& binary_stream_to_write_to) const;

		// The stream should be created with std::ios_base::binary flag
		void read(std::istream& binary_stream_to_read_from);

		bool is_empty() const;

		void apply_dropout_layer_config(
			const dropout_layer_config& dropout,
			bool is_direct);

	private:
		class mult_transform
		{
		public:
			mult_transform(float mult);
			
			float operator() (float in);

		private:
			float mult;
		};
	};

	typedef std::tr1::shared_ptr<layer_data> layer_data_smart_ptr;
	typedef std::tr1::shared_ptr<const layer_data> const_layer_data_smart_ptr;
	typedef std::vector<layer_data_smart_ptr> layer_data_list;
}

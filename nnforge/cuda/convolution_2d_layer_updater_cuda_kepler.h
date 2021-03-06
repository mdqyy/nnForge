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

#include "layer_updater_cuda.h"

namespace nnforge
{
	namespace cuda
	{
		class convolution_2d_layer_updater_cuda_kepler : public layer_updater_cuda
		{
		public:
			convolution_2d_layer_updater_cuda_kepler();

			virtual ~convolution_2d_layer_updater_cuda_kepler();

			virtual void enqueue_test(
				unsigned int offset_input_entry_id,
				cudaStream_t stream_id,
				const std::vector<const_cuda_linear_buffer_device_smart_ptr>& schema_data,
				const std::vector<cuda_linear_buffer_device_smart_ptr>& data,
				const_cuda_linear_buffer_device_smart_ptr input_neurons_buffer,
				cuda_linear_buffer_device_smart_ptr output_neurons_buffer,
				const std::vector<cuda_linear_buffer_device_smart_ptr>& additional_buffers,
				std::vector<cuda_memobject_smart_ptr>& dynamic_memobjects,
				unsigned int entry_count);

			virtual void enqueue_backprop(
				cudaStream_t stream_id,
				const std::vector<const_cuda_linear_buffer_device_smart_ptr>& schema_data,
				const std::vector<cuda_linear_buffer_device_smart_ptr>& data,
				const_cuda_linear_buffer_device_smart_ptr output_neurons_buffer,
				const_cuda_linear_buffer_device_smart_ptr input_neurons_buffer,
				cuda_linear_buffer_device_smart_ptr output_errors_buffer,
				cuda_linear_buffer_device_smart_ptr input_errors_buffer,
				const std::vector<cuda_linear_buffer_device_smart_ptr>& additional_buffers,
				std::vector<cuda_memobject_smart_ptr>& dynamic_memobjects,
				unsigned int entry_count);

			virtual void enqueue_update_weights(
				unsigned int offset_input_entry_id,
				cudaStream_t stream_id,
				const std::vector<cuda_linear_buffer_device_smart_ptr>& data,
				const std::vector<const_cuda_linear_buffer_device_smart_ptr>& schema_data,
				const std::vector<const_cuda_linear_buffer_device_smart_ptr>& training_speed,
				cuda_linear_buffer_device_smart_ptr output_errors_buffer,
				const_cuda_linear_buffer_device_smart_ptr input_neurons_buffer,
				const std::vector<cuda_linear_buffer_device_smart_ptr>& additional_buffers,
				std::vector<cuda_memobject_smart_ptr>& dynamic_memobjects,
				unsigned int entry_count);

		protected:
			virtual bool is_in_place_backprop() const;

			virtual void updater_configured();

			virtual std::vector<size_t> get_sizes_of_additional_buffers_fixed() const;

			virtual void set_max_entry_count(unsigned int max_entry_count);

			virtual std::vector<unsigned int> get_linear_addressing_through_texture_per_entry() const;

			virtual void fill_additional_buffers(const std::vector<cuda_linear_buffer_device_smart_ptr>& additional_buffers) const;

			virtual int get_dynamic_memobject_count() const;

			std::vector<int> window_sizes;

			int forward_x_block_size;
			int forward_x_block_count;
			int forward_input_feature_map_group_count;
			int forward_input_feature_map_group_size;
			int forward_output_feature_map_block_count;

			int backward_x_block_size;
			int backward_x_block_count;
			int backward_output_feature_map_group_count;
			int backward_output_feature_map_group_size;
			int backward_input_feature_map_block_count;

			int updater_output_y_group_count;
			int updater_output_y_group_size;
			int updater_output_feature_map_block_count;
			int updater_window_x_block_count;
		private:
			static int get_block_size(int width);

			static int get_threadblock_size_biases(int output_neuron_count);
		};
	}
}

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

#include "network_trainer.h"

#include <vector>

#include "neural_network_exception.h"

namespace nnforge
{
	network_trainer::network_trainer(network_schema_smart_ptr schema)
		: schema(schema)
		, iteration_count(50)
	{
	}

	network_trainer::~network_trainer()
	{
	}

	void network_trainer::train(
		supervised_data_reader& reader,
		network_data_peeker& peeker,
		network_data_pusher& progress_pusher,
		network_data_pusher& pusher)
	{
		initialize_train(reader);
		unsigned int max_batch_size = get_max_batch_size();

		if (max_batch_size == 0)
			throw neural_network_exception("The trainer is unable to train even a single network");

		std::vector<training_task_state> task_list;

		while(true)
		{
			while (task_list.size() < max_batch_size)
			{
				training_task_state new_task;
				std::pair<unsigned int, network_data_smart_ptr> data_with_key = peeker.peek(schema);
				if (data_with_key.second == 0)
					break;

				new_task.index_peeked = data_with_key.first;
				new_task.data = data_with_key.second;
				task_list.push_back(new_task);
			}

			if (task_list.size() == 0)
				break; // Nothing is left to be trained

			train_step(
				reader,
				task_list);

			for(int i = 0; i < task_list.size(); ++i)
				progress_pusher.push(task_list[i]);

			for(int i = static_cast<int>(task_list.size()) - 1; i >= 0; --i)
			{
				if (is_last_iteration(task_list[i]))
				{
					pusher.push(task_list[i]);
					task_list.erase(task_list.begin() + i);
				}
			}
		}
	}

	bool network_trainer::is_last_iteration(const training_task_state& state) const
	{
		return (state.history.size() >= iteration_count);
	}
}

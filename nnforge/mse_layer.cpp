/*
 *  Copyright 2011-2015 Maxim Milakov
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

#include "mse_layer.h"

#include "layer_factory.h"
#include "neural_network_exception.h"
#include "proto/nnforge.pb.h"

#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/format.hpp>

namespace nnforge
{
	const std::string mse_layer::layer_type_name = "MSE";

	mse_layer::mse_layer(float scale)
		: scale(scale)
	{
	}

	std::string mse_layer::get_type_name() const
	{
		return layer_type_name;
	}

	layer::ptr mse_layer::clone() const
	{
		return layer::ptr(new mse_layer(*this));
	}

	layer_configuration mse_layer::get_layer_configuration(const std::vector<layer_configuration>& input_configuration_list) const
	{
		if ((input_configuration_list[0].feature_map_count >= 0) && (input_configuration_list[1].feature_map_count >= 0) && (input_configuration_list[0].feature_map_count != input_configuration_list[1].feature_map_count))
			throw neural_network_exception((boost::format("Feature map counts in 2 input layers don't match: %1% and %2%") % input_configuration_list[0].feature_map_count % input_configuration_list[1].feature_map_count).str());

		return layer_configuration(1, input_configuration_list[0].dimension_count);
	}

	layer_configuration_specific mse_layer::get_output_layer_configuration_specific(const std::vector<layer_configuration_specific>& input_configuration_specific_list) const
	{
		if (input_configuration_specific_list[0].feature_map_count != input_configuration_specific_list[1].feature_map_count)
			throw neural_network_exception((boost::format("Feature map counts in 2 input layers don't match: %1% and %2%") % input_configuration_specific_list[0].feature_map_count % input_configuration_specific_list[1].feature_map_count).str());

		return layer_configuration_specific(1, input_configuration_specific_list[0].dimension_sizes);
	}

	bool mse_layer::get_input_layer_configuration_specific(
		layer_configuration_specific& input_configuration_specific,
		const layer_configuration_specific& output_configuration_specific,
		unsigned int input_layer_id) const
	{
		return false;
	}

	void mse_layer::write_proto(void * layer_proto) const
	{
		if (scale != 1.0F)
		{
			protobuf::Layer * layer_proto_typed = reinterpret_cast<protobuf::Layer *>(layer_proto);
			protobuf::MSEParam * param = layer_proto_typed->mutable_mse_param();

			param->set_scale(scale);
		}
	}

	void mse_layer::read_proto(const void * layer_proto)
	{
		const protobuf::Layer * layer_proto_typed = reinterpret_cast<const protobuf::Layer *>(layer_proto);
		if (!layer_proto_typed->has_mse_param())
		{
			scale = 1.0F;
		}
		else
		{
			scale = layer_proto_typed->mse_param().scale();
		}
	}

	float mse_layer::get_forward_flops(const std::vector<layer_configuration_specific>& input_configuration_specific_list) const
	{
		unsigned int neuron_count = get_output_layer_configuration_specific(input_configuration_specific_list).get_neuron_count();
		unsigned int per_item_flops = input_configuration_specific_list[0].feature_map_count * 3;

		return static_cast<float>(neuron_count) * static_cast<float>(per_item_flops);
	}

	float mse_layer::get_backward_flops(
		const std::vector<layer_configuration_specific>& input_configuration_specific_list,
		unsigned int input_layer_id) const
	{
		unsigned int neuron_count = input_configuration_specific_list[input_layer_id].get_neuron_count();
		unsigned int per_item_flops = input_configuration_specific_list[input_layer_id].feature_map_count * 2;

		return static_cast<float>(neuron_count) * static_cast<float>(per_item_flops);
	}
}

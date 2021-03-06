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

#include "training_momentum.h"

#include <algorithm>
#include <boost/format.hpp>

#include "neural_network_exception.h"

namespace nnforge
{
	training_momentum::training_momentum()
		: type(no_momentum)
		, momentum_val(0.0F)
	{
	}

	training_momentum::training_momentum(
		const std::string& momentum_type_str,
		float momentum_val)
		: momentum_val(momentum_val)
	{
		if (momentum_val > 0.0F)
		{
			std::string momentum_type_str_lower_case = momentum_type_str;
			std::transform(momentum_type_str_lower_case.begin(), momentum_type_str_lower_case.end(), momentum_type_str_lower_case.begin(), ::tolower);

			if (momentum_type_str_lower_case == "none")
				type = no_momentum;
			if (momentum_type_str_lower_case == "vanilla")
				type = vanilla_momentum;
			else if (momentum_type_str_lower_case == "nesterov")
				type = nesterov_momentum;
			else
				throw neural_network_exception((boost::format("Invalid momentum : %1%") % momentum_type_str).str());
		}
		else
		{
			type = no_momentum;
		}
	}
}

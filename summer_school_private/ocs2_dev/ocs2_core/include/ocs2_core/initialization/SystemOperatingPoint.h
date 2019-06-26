/******************************************************************************
Copyright (c) 2017, Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/
#ifndef SYSTEMOPERATINGPOINT_OCS2_H_
#define SYSTEMOPERATINGPOINT_OCS2_H_

#include <memory>

#include "ocs2_core/initialization/SystemOperatingTrajectoriesBase.h"

namespace ocs2{

/**
 * This is base class for initializing the SLQ-based algorithms.
 *
 * @tparam STATE_DIM: Dimension of the state space.
 * @tparam INPUT_DIM: Dimension of the control input space.
 * @tparam LOGIC_RULES_T: Logic Rules type (default NullLogicRules).
 */
template <size_t STATE_DIM, size_t INPUT_DIM, class LOGIC_RULES_T=NullLogicRules>
class SystemOperatingPoint : public SystemOperatingTrajectoriesBase<STATE_DIM, INPUT_DIM, LOGIC_RULES_T>
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	typedef SystemOperatingTrajectoriesBase<STATE_DIM, INPUT_DIM, LOGIC_RULES_T> Base;

	typedef std::shared_ptr<SystemOperatingPoint<STATE_DIM, INPUT_DIM, LOGIC_RULES_T>> Ptr;

	typedef typename Base::scalar_t             scalar_t;
	typedef typename Base::scalar_array_t       scalar_array_t;
	typedef typename Base::size_array_t         size_array_t;
	typedef typename Base::state_vector_t       state_vector_t;
	typedef typename Base::state_vector_array_t state_vector_array_t;
	typedef typename Base::input_vector_t       input_vector_t;
	typedef typename Base::input_vector_array_t input_vector_array_t;

	/**
	 * Default constructor
	 */
	SystemOperatingPoint()
	: SystemOperatingPoint(state_vector_t::Zero(), input_vector_t::Zero())
	{}

	/**
	 * Constructor
	 */
	SystemOperatingPoint(const state_vector_t& stateOperatingPoint, const input_vector_t& inputOperatingPoint)
	: stateOperatingPoint_(stateOperatingPoint),
	  inputOperatingPoint_(inputOperatingPoint)
	{}

	/**
	 * Default destructor.
	 */
	virtual ~SystemOperatingPoint() {}

	/**
	 * Initializes the operating trajectories class.
	 *
	 * @param [in] logicRulesMachine: A class which contains and parse the logic rules e.g
	 * method findActiveSubsystemHandle returns a Lambda expression which can be used to
	 * find the ID of the current active subsystem.
	 * @param [in] partitionIndex: index of the time partition.
	 * @param [in] algorithmName: The algorithm that class this class (default not defined).
	 */
	virtual void initializeModel(
			LogicRulesMachine<LOGIC_RULES_T>& logicRulesMachine,
			const size_t& partitionIndex,
			const char* algorithmName=NULL) override {

		Base::initializeModel(logicRulesMachine, partitionIndex, algorithmName);
	}

	/**
	 * Returns pointer to the class.
	 *
	 * @return A raw pointer to the class.
	 */
	virtual SystemOperatingPoint<STATE_DIM, INPUT_DIM, LOGIC_RULES_T>* clone() const override {

		return new SystemOperatingPoint<STATE_DIM, INPUT_DIM, LOGIC_RULES_T>(*this);
	}

	/**
	 * Gets the operating points for the system in time interval [startTime, finalTime] where there is
	 * no intermediate switches except possibly the end time.
	 *
	 * @param [in] initialState: Initial state.
	 * @param [in] startTime: Initial time.
	 * @param [in] finalTime: Final time.
	 * @param [out] timeTrajectory: Output time stamp trajectory.
	 * @param [out] stateTrajectory: Output state trajectory.
	 * @param [out] inputTrajectory: Output control input trajectory.
	 * @param [in] concatOutput: Whether to concatenate the output to the input trajectories or
	 * override (default).
	 */
	virtual void getSystemOperatingTrajectories(
			const state_vector_t& initialState,
			const scalar_t& startTime,
			const scalar_t& finalTime,
			scalar_array_t& timeTrajectory,
			state_vector_array_t& stateTrajectory,
			input_vector_array_t& inputTrajectory,
			bool concatOutput = false) override {

		if (concatOutput==false) {
			timeTrajectory.clear();
			stateTrajectory.clear();
			inputTrajectory.clear();
		}

		timeTrajectory.emplace_back(startTime);
		timeTrajectory.emplace_back(finalTime);

		stateTrajectory.emplace_back(stateOperatingPoint_);
		stateTrajectory.emplace_back(stateOperatingPoint_);

		inputTrajectory.emplace_back(inputOperatingPoint_);
		inputTrajectory.emplace_back(inputOperatingPoint_);
	}


protected:
	state_vector_t stateOperatingPoint_;
	input_vector_t inputOperatingPoint_;

};

} // namespace ocs2

#endif /* SYSTEMOPERATINGPOINT_OCS2_H_ */
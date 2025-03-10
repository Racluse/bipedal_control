/**
 * @file WeightedWbc.h
 * @author xiaobaige (zitongbai@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-24
 * @ref https://github.com/qiayuanl/legged_control
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "bipedal_wbc/WbcBase.h"
#include <ocs2_bipedal_robot/gait/MotionPhaseDefinition.h>

namespace ocs2{
namespace bipedal_robot {
class WeightedWbc : public WbcBase {
  using Vector6 = Eigen::Matrix<scalar_t, 6, 1>;
  using Matrix6 = Eigen::Matrix<scalar_t, 6, 6>;
public:
  explicit WeightedWbc(const PinocchioInterface& pinocchioInterface, CentroidalModelInfo info, const PinocchioEndEffectorKinematics& eeKinematics)
      : WbcBase(pinocchioInterface, info, eeKinematics) {
          lastQpSol_.setZero(getNumDecisionVars());
      };
  /**
   * @brief update the desired state, input, and measured state
   *  then solve the problem
   * 
   * @param stateDesired 
   * @param inputDesired 
   * @param rbdStateMeasured 
   * @param mode 
   * @param period 
   * @return vector_t 
   * 
   * @note 
   */
  vector_t update(const vector_t& stateDesired, const vector_t& inputDesired, const vector_t& rbdStateMeasured, size_t mode,
                scalar_t period) override;

  void loadTasksSetting(const std::string& taskFile, bool verbose) override;

  void clearLastQpSol() { lastQpSol_.setZero(getNumDecisionVars()); }

  void setWeights(scalar_t weightSwingLeg, scalar_t weightBaseAccel, scalar_t weightContactForce) {
    weightSwingLeg_ = weightSwingLeg;
    weightBaseAccel_ = weightBaseAccel;
    weightContactForce_ = weightContactForce;
  }

protected:
  virtual Task formulateConstraints();
  virtual Task formulateWeightedTasks(const vector_t& stateDesired, const vector_t& inputDesired, scalar_t period);

private:
  scalar_t weightSwingLeg_, weightBaseAccel_, weightContactForce_;
  vector_t lastQpSol_;
  size_t plannedMode_ = ModeNumber::STANCE;

};
}  // namespace bipedal
}  // namespace ocs2
/*********************************************************************
* Software Ligcense Agreement (BSD License)
*
*  Copyright (c) 2008, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: John Hsu */

#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <urdf_model/joint.h>
#include <tinyxml2.h>
#include <urdf_parser/urdf_parser.h>

#include "./pose.hpp"

namespace urdf{

bool parseJointDynamics(JointDynamics &jd, tinyxml2::XMLElement* config)
{
  jd.clear();

  // Get joint damping
  const char* damping_str = config->Attribute("damping");
  if (damping_str == NULL){
    jd.damping = 0;
  }
  else
  {
    try {
      jd.damping = strToDouble(damping_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  // Get joint friction
  const char* friction_str = config->Attribute("friction");
  if (friction_str == NULL){
    jd.friction = 0;
  }
  else
  {
    try {
      jd.friction = strToDouble(friction_str);
    } catch (std::runtime_error &) {
      return false;
    }
  }

  if (damping_str == NULL && friction_str == NULL)
  {
    return false;
  }
  else{
    return true;
  }
}

bool parseJointLimits(JointLimits &jl, tinyxml2::XMLElement* config)
{
  jl.clear();

  // Get lower joint limit
  const char* lower_str = config->Attribute("lower");
  if (lower_str == NULL){
    jl.lower = 0;
  }
  else
  {
    try {
      jl.lower = strToDouble(lower_str);
    } catch (std::runtime_error &) {
      return false;
    }
  }

  // Get upper joint limit
  const char* upper_str = config->Attribute("upper");
  if (upper_str == NULL){
    jl.upper = 0;
  }
  else
  {
    try {
      jl.upper = strToDouble(upper_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  // Get joint effort limit
  const char* effort_str = config->Attribute("effort");
  if (effort_str == NULL){
    return false;
  }
  else
  {
    try {
      jl.effort = strToDouble(effort_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  // Get joint velocity limit
  const char* velocity_str = config->Attribute("velocity");
  if (velocity_str == NULL){
    return false;
  }
  else
  {
    try {
      jl.velocity = strToDouble(velocity_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  return true;
}

bool parseJointSafety(JointSafety &js, tinyxml2::XMLElement* config)
{
  js.clear();

  // Get soft_lower_limit joint limit
  const char* soft_lower_limit_str = config->Attribute("soft_lower_limit");
  if (soft_lower_limit_str == NULL)
  {
    js.soft_lower_limit = 0;
  }
  else
  {
    try {
      js.soft_lower_limit = strToDouble(soft_lower_limit_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  // Get soft_upper_limit joint limit
  const char* soft_upper_limit_str = config->Attribute("soft_upper_limit");
  if (soft_upper_limit_str == NULL)
  {
    js.soft_upper_limit = 0;
  }
  else
  {
    try {
      js.soft_upper_limit = strToDouble(soft_upper_limit_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  // Get k_position_ safety "position" gain - not exactly position gain
  const char* k_position_str = config->Attribute("k_position");
  if (k_position_str == NULL)
  {
    js.k_position = 0;
  }
  else
  {
    try {
      js.k_position = strToDouble(k_position_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }
  // Get k_velocity_ safety velocity gain
  const char* k_velocity_str = config->Attribute("k_velocity");
  if (k_velocity_str == NULL)
  {
    return false;
  }
  else
  {
    try {
      js.k_velocity = strToDouble(k_velocity_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  return true;
}

bool parseJointCalibration(JointCalibration &jc, tinyxml2::XMLElement* config)
{
  jc.clear();

  // Get rising edge position
  const char* rising_position_str = config->Attribute("rising");
  if (rising_position_str == NULL)
  {
    jc.rising.reset();
  }
  else
  {
    try {
      jc.rising.reset(new double(strToDouble(rising_position_str)));
    } catch(std::runtime_error &) {
      return false;
    }
  }

  // Get falling edge position
  const char* falling_position_str = config->Attribute("falling");
  if (falling_position_str == NULL)
  {
    jc.falling.reset();
  }
  else
  {
    try {
      jc.falling.reset(new double(strToDouble(falling_position_str)));
    } catch(std::runtime_error &) {
      return false;
    }
  }

  return true;
}

bool parseJointMimic(JointMimic &jm, tinyxml2::XMLElement* config)
{
  jm.clear();

  // Get name of joint to mimic
  const char* joint_name_str = config->Attribute("joint");

  if (joint_name_str == NULL)
  {
    return false;
  }
  else
    jm.joint_name = joint_name_str;

  // Get mimic multiplier
  const char* multiplier_str = config->Attribute("multiplier");

  if (multiplier_str == NULL)
  {
    jm.multiplier = 1;
  }
  else
  {
    try {
      jm.multiplier = strToDouble(multiplier_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }


  // Get mimic offset
  const char* offset_str = config->Attribute("offset");
  if (offset_str == NULL)
  {
    jm.offset = 0;
  }
  else
  {
    try {
      jm.offset = strToDouble(offset_str);
    } catch(std::runtime_error &) {
      return false;
    }
  }

  return true;
}

bool parseJoint(Joint &joint, tinyxml2::XMLElement* config)
{
  joint.clear();

  // Get Joint Name
  const char *name = config->Attribute("name");
  if (!name)
  {
    return false;
  }
  joint.name = name;

  // Get transform from Parent Link to Joint Frame
  tinyxml2::XMLElement *origin_xml = config->FirstChildElement("origin");
  if (!origin_xml)
  {
    joint.parent_to_joint_origin_transform.clear();
  }
  else
  {
    if (!parsePoseInternal(joint.parent_to_joint_origin_transform, origin_xml))
    {
      joint.parent_to_joint_origin_transform.clear();
      return false;
    }
  }

  // Get Parent Link
  tinyxml2::XMLElement *parent_xml = config->FirstChildElement("parent");
  if (parent_xml)
  {
    const char *pname = parent_xml->Attribute("link");
    if (!pname)
    {
    }
    else
    {
      joint.parent_link_name = std::string(pname);
    }
  }

  // Get Child Link
  tinyxml2::XMLElement *child_xml = config->FirstChildElement("child");
  if (child_xml)
  {
    const char *pname = child_xml->Attribute("link");
    if (!pname)
    {
    }
    else
    {
      joint.child_link_name = std::string(pname);
    }
  }

  // Get Joint type
  const char* type_char = config->Attribute("type");
  if (!type_char)
  {
    return false;
  }

  std::string type_str = type_char;
  if (type_str == "planar")
    joint.type = Joint::PLANAR;
  else if (type_str == "floating")
    joint.type = Joint::FLOATING;
  else if (type_str == "revolute")
    joint.type = Joint::REVOLUTE;
  else if (type_str == "continuous")
    joint.type = Joint::CONTINUOUS;
  else if (type_str == "prismatic")
    joint.type = Joint::PRISMATIC;
  else if (type_str == "fixed")
    joint.type = Joint::FIXED;
  else
  {
    return false;
  }

  // Get Joint Axis
  if (joint.type != Joint::FLOATING && joint.type != Joint::FIXED)
  {
    // axis
    tinyxml2::XMLElement *axis_xml = config->FirstChildElement("axis");
    if (!axis_xml){
      joint.axis = Vector3(1.0, 0.0, 0.0);
    }
    else{
      if (axis_xml->Attribute("xyz")){
        try {
          joint.axis.init(axis_xml->Attribute("xyz"));
        }
        catch (ParseError &e) {
          joint.axis.clear();
          return false;
        }
      }
    }
  }

  // Get limit
  tinyxml2::XMLElement *limit_xml = config->FirstChildElement("limit");
  if (limit_xml)
  {
    joint.limits.reset(new JointLimits());
    if (!parseJointLimits(*joint.limits, limit_xml))
    {
      joint.limits.reset();
      return false;
    }
  }
  else if (joint.type == Joint::REVOLUTE)
  {
    return false;
  }
  else if (joint.type == Joint::PRISMATIC)
  {
    return false;
  }

  // Get safety
  tinyxml2::XMLElement *safety_xml = config->FirstChildElement("safety_controller");
  if (safety_xml)
  {
    joint.safety.reset(new JointSafety());
    if (!parseJointSafety(*joint.safety, safety_xml))
    {
      joint.safety.reset();
      return false;
    }
  }

  // Get calibration
  tinyxml2::XMLElement *calibration_xml = config->FirstChildElement("calibration");
  if (calibration_xml)
  {
    joint.calibration.reset(new JointCalibration());
    if (!parseJointCalibration(*joint.calibration, calibration_xml))
    {
      joint.calibration.reset();
      return false;
    }
  }

  // Get Joint Mimic
  tinyxml2::XMLElement *mimic_xml = config->FirstChildElement("mimic");
  if (mimic_xml)
  {
    joint.mimic.reset(new JointMimic());
    if (!parseJointMimic(*joint.mimic, mimic_xml))
    {
      joint.mimic.reset();
      return false;
    }
  }

  // Get Dynamics
  tinyxml2::XMLElement *prop_xml = config->FirstChildElement("dynamics");
  if (prop_xml)
  {
    joint.dynamics.reset(new JointDynamics());
    if (!parseJointDynamics(*joint.dynamics, prop_xml))
    {
      joint.dynamics.reset();
      return false;
    }
  }

  return true;
}


/* exports */
bool exportPose(Pose &pose, tinyxml2::XMLElement* xml);

bool exportJointDynamics(JointDynamics &jd, tinyxml2::XMLElement* xml)
{
  tinyxml2::XMLElement *dynamics_xml = xml->GetDocument()->NewElement("dynamics");
  dynamics_xml->SetAttribute("damping", urdf_export_helpers::values2str(jd.damping).c_str() );
  dynamics_xml->SetAttribute("friction", urdf_export_helpers::values2str(jd.friction).c_str() );
  xml->LinkEndChild(dynamics_xml);
  return true;
}

bool exportJointLimits(JointLimits &jl, tinyxml2::XMLElement* xml)
{
  tinyxml2::XMLElement *limit_xml = xml->GetDocument()->NewElement("limit");
  limit_xml->SetAttribute("effort", urdf_export_helpers::values2str(jl.effort).c_str());
  limit_xml->SetAttribute("velocity", urdf_export_helpers::values2str(jl.velocity).c_str());
  limit_xml->SetAttribute("lower", urdf_export_helpers::values2str(jl.lower).c_str());
  limit_xml->SetAttribute("upper", urdf_export_helpers::values2str(jl.upper).c_str());
  xml->LinkEndChild(limit_xml);
  return true;
}

bool exportJointSafety(JointSafety &js, tinyxml2::XMLElement* xml)
{
  tinyxml2::XMLElement *safety_xml = xml->GetDocument()->NewElement("safety_controller");
  safety_xml->SetAttribute("k_position", urdf_export_helpers::values2str(js.k_position).c_str());
  safety_xml->SetAttribute("k_velocity", urdf_export_helpers::values2str(js.k_velocity).c_str());
  safety_xml->SetAttribute("soft_lower_limit", urdf_export_helpers::values2str(js.soft_lower_limit).c_str());
  safety_xml->SetAttribute("soft_upper_limit", urdf_export_helpers::values2str(js.soft_upper_limit).c_str());
  xml->LinkEndChild(safety_xml);
  return true;
}

bool exportJointCalibration(JointCalibration &jc, tinyxml2::XMLElement* xml)
{
  if (jc.falling || jc.rising)
  {
    tinyxml2::XMLElement *calibration_xml = xml->GetDocument()->NewElement("calibration");
    if (jc.falling)
      calibration_xml->SetAttribute("falling", urdf_export_helpers::values2str(*jc.falling).c_str());
    if (jc.rising)
      calibration_xml->SetAttribute("rising", urdf_export_helpers::values2str(*jc.rising).c_str());
    //calibration_xml->SetAttribute("reference_position", urdf_export_helpers::values2str(jc.reference_position) );
    xml->LinkEndChild(calibration_xml);
  }
  return true;
}

bool exportJointMimic(JointMimic &jm, tinyxml2::XMLElement* xml)
{
  if (!jm.joint_name.empty())
  {
    tinyxml2::XMLElement *mimic_xml = xml->GetDocument()->NewElement("mimic");
    mimic_xml->SetAttribute("offset", urdf_export_helpers::values2str(jm.offset).c_str());
    mimic_xml->SetAttribute("multiplier", urdf_export_helpers::values2str(jm.multiplier).c_str());
    mimic_xml->SetAttribute("joint", jm.joint_name.c_str());
    xml->LinkEndChild(mimic_xml);
  }
  return true;
}

bool exportJoint(Joint &joint, tinyxml2::XMLElement* xml)
{
  tinyxml2::XMLElement * joint_xml = xml->GetDocument()->NewElement("joint");
  joint_xml->SetAttribute("name", joint.name.c_str());
  if (joint.type == urdf::Joint::PLANAR)
    joint_xml->SetAttribute("type", "planar");
  else if (joint.type == urdf::Joint::FLOATING)
    joint_xml->SetAttribute("type", "floating");
  else if (joint.type == urdf::Joint::REVOLUTE)
    joint_xml->SetAttribute("type", "revolute");
  else if (joint.type == urdf::Joint::CONTINUOUS)
    joint_xml->SetAttribute("type", "continuous");
  else if (joint.type == urdf::Joint::PRISMATIC)
    joint_xml->SetAttribute("type", "prismatic");
  else if (joint.type == urdf::Joint::FIXED)
    joint_xml->SetAttribute("type", "fixed");
  else

  // origin
  exportPose(joint.parent_to_joint_origin_transform, joint_xml);

  // axis
  tinyxml2::XMLElement * axis_xml = joint_xml->GetDocument()->NewElement("axis");
  axis_xml->SetAttribute("xyz", urdf_export_helpers::values2str(joint.axis).c_str());
  joint_xml->LinkEndChild(axis_xml);

  // parent
  tinyxml2::XMLElement * parent_xml = joint_xml->GetDocument()->NewElement("parent");
  parent_xml->SetAttribute("link", joint.parent_link_name.c_str());
  joint_xml->LinkEndChild(parent_xml);

  // child
  tinyxml2::XMLElement * child_xml = joint_xml->GetDocument()->NewElement("child");
  child_xml->SetAttribute("link", joint.child_link_name.c_str());
  joint_xml->LinkEndChild(child_xml);

  if (joint.dynamics)
    exportJointDynamics(*(joint.dynamics), joint_xml);
  if (joint.limits)
    exportJointLimits(*(joint.limits), joint_xml);
  if (joint.safety)
    exportJointSafety(*(joint.safety), joint_xml);
  if (joint.calibration)
    exportJointCalibration(*(joint.calibration), joint_xml);
  if (joint.mimic)
    exportJointMimic(*(joint.mimic), joint_xml);

  xml->LinkEndChild(joint_xml);
  return true;
}



}

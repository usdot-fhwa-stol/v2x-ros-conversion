/*
 * Copyright (C) 2022 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include <gtest/gtest.h>
#include <j2735_convertor/spat_convertor.hpp>

namespace j2735_convertor
{

TEST(SPATConvertorTest, convertSPATWithFullRoadAuthorityID)
{
    j2735_v2x_msgs::msg::SPAT in_msg;
    in_msg.time_stamp = 12345;
    in_msg.time_stamp_exists = true;

    j2735_v2x_msgs::msg::IntersectionState intersection;
    intersection.id.id = 100;
    intersection.revision = 1;
    intersection.status.intersection_status_object = 0;

    intersection.road_authority_id_exists = true;
    intersection.road_authority_id.choice = j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID;
    intersection.road_authority_id.full_rd_auth_id = {0x01, 0x02, 0x03, 0x04};

    j2735_v2x_msgs::msg::MovementState movement;
    movement.signal_group = 1;
    j2735_v2x_msgs::msg::MovementEvent event;
    event.event_state.movement_phase_state = j2735_v2x_msgs::msg::MovementPhaseState::STOP_AND_REMAIN;
    event.timing_exists = true;
    event.timing.min_end_time = 5000;
    movement.state_time_speed.movement_event_list.push_back(event);
    intersection.states.movement_list.push_back(movement);

    in_msg.intersections.intersection_state_list.push_back(intersection);

    carma_v2x_msgs::msg::SPAT out_msg;
    SPATConvertor::convert(in_msg, out_msg);

    ASSERT_EQ(out_msg.intersection_state_list.size(), 1);
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id_exists, true);
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id.choice,
              j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID);
    std::vector<uint8_t> expected = {0x01, 0x02, 0x03, 0x04};
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id.full_rd_auth_id, expected);
}

TEST(SPATConvertorTest, convertSPATWithRelRoadAuthorityID)
{
    j2735_v2x_msgs::msg::SPAT in_msg;

    j2735_v2x_msgs::msg::IntersectionState intersection;
    intersection.id.id = 200;
    intersection.revision = 1;
    intersection.status.intersection_status_object = 0;

    intersection.road_authority_id_exists = true;
    intersection.road_authority_id.choice = j2735_v2x_msgs::msg::RoadAuthorityID::RELATIVE_ROAD_AUTHORITY_ID;
    intersection.road_authority_id.rel_rd_auth_id = {0xAA, 0xBB};

    j2735_v2x_msgs::msg::MovementState movement;
    movement.signal_group = 1;
    j2735_v2x_msgs::msg::MovementEvent event;
    event.event_state.movement_phase_state = j2735_v2x_msgs::msg::MovementPhaseState::PERMISSIVE_MOVEMENT_ALLOWED;
    movement.state_time_speed.movement_event_list.push_back(event);
    intersection.states.movement_list.push_back(movement);

    in_msg.intersections.intersection_state_list.push_back(intersection);

    carma_v2x_msgs::msg::SPAT out_msg;
    SPATConvertor::convert(in_msg, out_msg);

    ASSERT_EQ(out_msg.intersection_state_list.size(), 1);
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id_exists, true);
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id.choice,
              j2735_v2x_msgs::msg::RoadAuthorityID::RELATIVE_ROAD_AUTHORITY_ID);
    std::vector<uint8_t> expected = {0xAA, 0xBB};
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id.rel_rd_auth_id, expected);
}

TEST(SPATConvertorTest, convertSPATWithoutRoadAuthorityID)
{
    j2735_v2x_msgs::msg::SPAT in_msg;

    j2735_v2x_msgs::msg::IntersectionState intersection;
    intersection.id.id = 300;
    intersection.revision = 1;
    intersection.status.intersection_status_object = 0;
    intersection.road_authority_id_exists = false;

    j2735_v2x_msgs::msg::MovementState movement;
    movement.signal_group = 1;
    j2735_v2x_msgs::msg::MovementEvent event;
    event.event_state.movement_phase_state = j2735_v2x_msgs::msg::MovementPhaseState::STOP_AND_REMAIN;
    movement.state_time_speed.movement_event_list.push_back(event);
    intersection.states.movement_list.push_back(movement);

    in_msg.intersections.intersection_state_list.push_back(intersection);

    carma_v2x_msgs::msg::SPAT out_msg;
    SPATConvertor::convert(in_msg, out_msg);

    ASSERT_EQ(out_msg.intersection_state_list.size(), 1);
    EXPECT_EQ(out_msg.intersection_state_list[0].road_authority_id_exists, false);
}

} // namespace j2735_convertor

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
#include <j2735_convertor/map_convertor.hpp>

namespace j2735_convertor
{

TEST(MAPConvertorTest, convertMAPWithFullRoadAuthorityID)
{
    j2735_v2x_msgs::msg::MapData in_msg;
    in_msg.msg_issue_revision = 1;
    in_msg.intersections_exists = true;

    j2735_v2x_msgs::msg::IntersectionGeometry intersection;
    intersection.id.id = 500;
    intersection.revision = 1;
    intersection.ref_point.latitude = 400000000;
    intersection.ref_point.longitude = -800000000;

    intersection.road_authority_id_exists = true;
    intersection.road_authority_id.choice = j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID;
    intersection.road_authority_id.full_rd_auth_id = {0x06, 0x07, 0x08, 0x09, 0x0A};

    in_msg.intersections.push_back(intersection);

    carma_v2x_msgs::msg::MapData out_msg;
    MapConvertor::convert(in_msg, out_msg);

    ASSERT_EQ(out_msg.intersections.size(), 1);
    EXPECT_EQ(out_msg.intersections[0].road_authority_id_exists, true);
    EXPECT_EQ(out_msg.intersections[0].road_authority_id.choice,
              j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID);
    std::vector<uint8_t> expected = {0x06, 0x07, 0x08, 0x09, 0x0A};
    EXPECT_EQ(out_msg.intersections[0].road_authority_id.full_rd_auth_id, expected);
}

TEST(MAPConvertorTest, convertMAPWithRelRoadAuthorityID)
{
    j2735_v2x_msgs::msg::MapData in_msg;
    in_msg.msg_issue_revision = 1;
    in_msg.intersections_exists = true;

    j2735_v2x_msgs::msg::IntersectionGeometry intersection;
    intersection.id.id = 600;
    intersection.revision = 1;
    intersection.ref_point.latitude = 400000000;
    intersection.ref_point.longitude = -800000000;

    intersection.road_authority_id_exists = true;
    intersection.road_authority_id.choice = j2735_v2x_msgs::msg::RoadAuthorityID::RELATIVE_ROAD_AUTHORITY_ID;
    intersection.road_authority_id.rel_rd_auth_id = {0xCC, 0xDD, 0xEE};

    in_msg.intersections.push_back(intersection);

    carma_v2x_msgs::msg::MapData out_msg;
    MapConvertor::convert(in_msg, out_msg);

    ASSERT_EQ(out_msg.intersections.size(), 1);
    EXPECT_EQ(out_msg.intersections[0].road_authority_id_exists, true);
    EXPECT_EQ(out_msg.intersections[0].road_authority_id.choice,
              j2735_v2x_msgs::msg::RoadAuthorityID::RELATIVE_ROAD_AUTHORITY_ID);
    std::vector<uint8_t> expected = {0xCC, 0xDD, 0xEE};
    EXPECT_EQ(out_msg.intersections[0].road_authority_id.rel_rd_auth_id, expected);
}

TEST(MAPConvertorTest, convertMAPWithoutRoadAuthorityID)
{
    j2735_v2x_msgs::msg::MapData in_msg;
    in_msg.msg_issue_revision = 1;
    in_msg.intersections_exists = true;

    j2735_v2x_msgs::msg::IntersectionGeometry intersection;
    intersection.id.id = 700;
    intersection.revision = 1;
    intersection.ref_point.latitude = 400000000;
    intersection.ref_point.longitude = -800000000;
    intersection.road_authority_id_exists = false;

    in_msg.intersections.push_back(intersection);

    carma_v2x_msgs::msg::MapData out_msg;
    MapConvertor::convert(in_msg, out_msg);

    ASSERT_EQ(out_msg.intersections.size(), 1);
    EXPECT_EQ(out_msg.intersections[0].road_authority_id_exists, false);
}

} // namespace j2735_convertor

/*
 * Copyright (C) 2019-2021 LEIDOS.
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

#include "cpp_message/Map_Message.h"
#include <gtest/gtest.h>
#include <boost/optional/optional_io.hpp> //to print boost::optional

namespace cpp_message
{

    int chartoint(char input)
    {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if (input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");
    }

    void hextobin(const char *src, char *target)
    {
        while (*src && src[1])
        {
            *(target++) = chartoint(*src) * 16 + chartoint(src[1]);
            src += 2;
        }
    }

    std::vector<char> Hex2Bytes(const std::string &hex)
    {
        std::vector<char> bytes;

        for (unsigned int i = 0; i < hex.length(); i += 2)
        {
            std::string byteString = hex.substr(i, 2);
            char byte = (char)strtol(byteString.c_str(), NULL, 16);
            bytes.push_back(byte);
        }

        return bytes;
    }

    TEST(MapMessageTest, testDecodeMapMessage)
    {
        std::string hex_message = "0012815a38033020a04bda0d4cdcf8143d4dc48811860224164802280008002297d4bc80a0a0a9825825923a90b2f2e418986f41b7006480602403812020084015480010004521d9f001414160c7c42a1879858619502a42a060e927100662000400105be6bf41c8aded5816ebc050507dcb860ec57aead5079e02828900890001000417223a50728b750f9c6ea9e8ae480a0a0f68746ad447c002828900a0880704404020803b9000200062b68d5305d1f9269a725027d8352f72867d6c82403340004000c53f5b761abbb7d35d3c0813ec1a3baac16bfc048050240301202008402208001000310fe55f849acd608d8ace136b440000dfe4808880008002086365c0017d1612eb34026067404895390907bd848050440302201c100024000200000090026180a0a0f2852600140001000000169fc1585bd1da000b00008000000a3bb2f439459a80060000400000046d55c416c67f40414032a0304000";

        std::vector<char> new_binary_input = Hex2Bytes(hex_message);
        std::vector<uint8_t> new_binary_input_int;
        for (int i = 0; i < new_binary_input.size(); i++)
        {
            new_binary_input_int.push_back(new_binary_input[i]);
        }

        auto node = std::make_shared<rclcpp::Node>("test_node");
        cpp_message::Map_Message worker(node->get_node_logging_interface());

        auto res = worker.decode_map_message(new_binary_input_int);

        if (res)
        {
            // Intersection Handling
            EXPECT_EQ(res.get().intersections_exists, true);
            EXPECT_EQ(res.get().intersections.size(), 1);

            // IntersectionID
            EXPECT_EQ(res.get().intersections[0].id.id, 9709);
            EXPECT_EQ(res.get().intersections[0].id.region_exists, false);
            EXPECT_EQ(res.get().intersections[0].id.region, 0);

            // Intersection Lane Set
            EXPECT_EQ(res.get().intersections[0].lane_set.lane_list.size(), 12);

            // Intersection Lane Width
            EXPECT_EQ(res.get().intersections[0].lane_width_exists, true);
            EXPECT_EQ(res.get().intersections[0].lane_width, 274);

            // Intersection Name
            EXPECT_EQ(res.get().intersections[0].name_exists, false);

            // Intersection Preempt Priority Data
            EXPECT_EQ(res.get().intersections[0].preempt_priority_data_exists, false);

            // Intersection Speed Limits
            EXPECT_EQ(res.get().intersections[0].speed_limits_exists, false);
            EXPECT_EQ(res.get().intersections[0].speed_limits.speed_limits.size(), 0);

            // Intersection Ref Points
            EXPECT_EQ(res.get().intersections[0].ref_point.elevation_exists, true);
            EXPECT_EQ(res.get().intersections[0].ref_point.elevation, 390);
            EXPECT_NEAR(res.get().intersections[0].ref_point.latitude, 3.8955 * pow(10, 8), pow(10, 8));
            EXPECT_NEAR(res.get().intersections[0].ref_point.longitude, -7.71493 * pow(10, 8), pow(10, 8));
            EXPECT_EQ(res.get().intersections[0].revision, 3);

            // Layer ID
            EXPECT_EQ(res.get().layer_id, 1);
            EXPECT_EQ(res.get().layer_id_exists, true);

            // Layer Type
            EXPECT_EQ(res.get().layer_type.layer_type, j2735_v2x_msgs::msg::LayerType::INTERSECTION_DATA);

            // Msg Issue Revision
            EXPECT_EQ(res.get().msg_issue_revision, 3);

            // Restriction List
            EXPECT_EQ(res.get().restriction_list_exists, false);
            EXPECT_EQ(res.get().restriction_list.restriction_class_list.size(), 0);

            // Road Segments
            EXPECT_EQ(res.get().road_segments_exists, false);
            EXPECT_EQ(res.get().road_segments.road_segment_list.size(), 0);

            // Time Stamp
            EXPECT_EQ(res.get().time_stamp_exists, false);
            EXPECT_EQ(res.get().time_stamp, 0);

            // Data Parameters
            EXPECT_EQ(res.get().data_parameters_exists, false);

            // Road Authority ID
            EXPECT_EQ(res.get().intersections[0].road_authority_id_exists, true);
            EXPECT_EQ(res.get().intersections[0].road_authority_id.choice, j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID);
            // OID arcs 1.2.3.4 are BER-encoded as: 40*1+2=0x2A, 3, 4
            std::vector<uint8_t> expected_id = {0x2A, 0x03, 0x04};
            EXPECT_EQ(res.get().intersections[0].road_authority_id.full_rd_auth_id, expected_id);
        }
        else
            EXPECT_TRUE(false);
    }

    TEST(MapMessageTest, testDecodeMapMessageNoVerify)
    {   // this test is added since it does not contain layerType and was triggering
        // seg fault. only confirms that the decode will succeed.
        std::string hex_message = "0012827c08000308b3e59102d413f963d187964412f76fc500010069022543dcd81c3b2c838168120c01400000100004c70ecbded2a1edce4c70ece11d2a1ec248c70ece8e52a1ebf04101e0418048000004000098e1d9786a543dc6418e1da7442543d12498e1dae19a543cbf90202808300d000000a000131c3b2f454a87b9a531c3b4ecf4a87a33631c3b5c73ca8798f514048120100800108000000031c3b2ee7ca87bdf831c3b2f314a87bf160005200000000c70ecb91d2a1ef928c70ecba412a1efdf43019000000c000131c3b2d9c4a87bea231c3b345dca87d93b31c3b384f4a87ea6d140982203c10301d0000008000131c3b2cd0ca87be8931c3b33c5ca87d9c331c3b37b7ca87eac90407020604200000040004638765832950f7f92638765944950f8180638765c45950f8c74638765f1e950f95c80809040009200000000c70eca8232a1ef754c70eca5772a1ef90c00288000000031c3b29b1ca87bcc731c3b28fcca87bd400c0b400000100004c70eca61b2a1eee38c70ec8c972a1f014cc70ec860b2a1f0460100a0c18188000004000098e1d94fbe543dd0318e1d8ab82543e53c18e1d7f66e543ed9d8204c183035000000a000231c3b2a77ca87b8bc31c3b1f01ca87c17031c3b15c0ca87c91731c3aff47ca87d9e814090320381800388000000031c3b2a7e4a87b57b31c3b2a3eca87b472000f200000000c70ecac2d2a1ed4b8c70ecab052a1ecffc3041000000c000031c3b2bbf4a87b4b031c3b25a54a879d631405042014203045000000a000031c3b2c57ca87b45331c3b2640ca879ca814020420482000488000000031c3b2f1a4a87b4f331c3b2ff64a87b4470013200000000c70ecbd092a1ed800c70ecc0952a1ed5680";

        std::vector<char> new_binary_input = Hex2Bytes(hex_message);
        std::vector<uint8_t> new_binary_input_int;
        for (int i = 0; i < new_binary_input.size(); i++)
        {
            new_binary_input_int.push_back(new_binary_input[i]);
        }

        auto node = std::make_shared<rclcpp::Node>("test_node");
        cpp_message::Map_Message worker(node->get_node_logging_interface());

        auto res = worker.decode_map_message(new_binary_input_int);

        if (res)
        {   
            EXPECT_TRUE(true);
        }
        else
            EXPECT_TRUE(false);
    }
}

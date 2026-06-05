/*
 * Copyright (C) 2021 LEIDOS.
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

/**
 * CPP File containing SPAT Message method implementations
 */

#include "cpp_message/SPAT_Message.h"

namespace cpp_message
{

    template <typename T>
    T *create_store_shared(std::vector<std::shared_ptr<void>> &shared_pointers)
    {
        auto obj_shared = std::make_shared<T>();
        shared_pointers.push_back(obj_shared);
        return obj_shared.get();
    }

    template <typename T>
    T *create_store_shared_array(std::vector<std::shared_ptr<void>> &shared_pointers, int size)
    {
        std::shared_ptr<T[]> array_shared(new T[size]{0});
        shared_pointers.push_back(array_shared);
        return array_shared.get();
    }

    //Convert the SPAT j2735 message to cav_msgs
    boost::optional<j2735_v2x_msgs::msg::SPAT> SPAT_Message::decode_spat_message(std::vector<uint8_t> &binary_array)
    {
        //Decode the binary message into SPAT message
        j2735_v2x_msgs::msg::SPAT output;
        //decode results - stored in binary array
        asn_dec_rval_t rval;
        MessageFrame_t *message = nullptr;

        //copy from vector to array
        size_t len = binary_array.size();

        uint8_t buf[len];
        std::copy(binary_array.begin(), binary_array.end(), buf);
        //use asn1c lib to decode

        rval = uper_decode(0, &asn_DEF_MessageFrame, (void **) &message, buf, len, 0, 0);
        //if decode success
        if (rval.code == RC_OK)
        {
            //1. Decode time stamp - Minute of the year
            long int *minute_of_the_year = new long int;
            bool is_time_stamp_exists;
            if (message->value.choice.SPAT.timeStamp)
            {
                is_time_stamp_exists = true;
                minute_of_the_year = message->value.choice.SPAT.timeStamp;
            }
            else
            {
                is_time_stamp_exists = false;
                *minute_of_the_year = DEFAULT_MINUTE_OF_YEAR_;
                // RCLCPP_DEBUG_STREAM(  get_logger(), "Minute of the year value doesn't exist, set to Default");
            }

            output.time_stamp_exists = is_time_stamp_exists;
            output.time_stamp = *minute_of_the_year;
            //2. Decode name
           
            size_t str_len = 0;
            if(message->value.choice.SPAT.name){
                std::string name = "";
                str_len = message->value.choice.SPAT.name->size;
                for (size_t i = 0; i < str_len; i++)
                {
                    name += message->value.choice.SPAT.name->buf[i];
                }
                output.name_exists = true;
                output.name = name;
            }
            else{
                output.name_exists = false;
            }

            //3. Decode Intersection
            j2735_v2x_msgs::msg::IntersectionStateList intersections;

            //IntersectionStateList is an array of intersectionState - of size 1-32
            for (size_t i = 0; i < message->value.choice.SPAT.intersections.list.count; i++)
            {
                if(!message->value.choice.SPAT.intersections.list.array[i]){
                    continue;
                }
                j2735_v2x_msgs::msg::IntersectionState intersection;
                IntersectionState_t *state = new IntersectionState_t;
                state = message->value.choice.SPAT.intersections.list.array[i];
                //Decode intersection name
                if (state->name)
                {
                    intersection.name_exists = true;

                    for (size_t j = 0; j < state->name->size; j++)
                    {
                        intersection.name += state->name->buf[i];
                    }
                }
                else
                {
                    intersection.name_exists = false;
                    intersection.name = DEFAULT_STRING_;
                    // RCLCPP_DEBUG_STREAM(  get_logger(), "Intersection name doesn't exist, set to Default");
                }
                //Decode id
                j2735_v2x_msgs::msg::IntersectionReferenceID id;
                id.id = state->id.id;
                if (state->id.region)
                {
                    id.region_exists = true;
                    id.region = *state->id.region;
                }
                else
                {
                    id.region_exists = false;
                    id.region = j2735_v2x_msgs::msg::IntersectionReferenceID::REGION_UNAVAILABLE;
                    // RCLCPP_DEBUG_STREAM(  get_logger(), "Intersection ID doesn't exist, set to Region Unavailable");
                }

                intersection.id = id;

                //Decode Msg count
                intersection.revision = state->revision;

                //Decode status
                int bit_assigned = 0; 
                for(size_t t = 0;t< state->status.size;t++){
                    if(state->status.buf[i] == 1){
                        bit_assigned = t;
                        break;
                    }
                }
                intersection.status.intersection_status_object = bit_assigned;

                //Minute of the year

                bool moy_exists = false;
                if (state->moy)
                {
                    MinuteOfTheYear_t *moy = new MinuteOfTheYear_t;
                    moy = state->moy;
                    intersection.moy = *moy;
                    intersection.moy_exists = true;
                }
                else
                {
                    intersection.moy_exists = false;
                    intersection.moy = j2735_v2x_msgs::msg::IntersectionState::MOY_INVALID;
                    // RCLCPP_DEBUG_STREAM(  get_logger(), "Intersection moy doesn't exis, set to MOY_INVALID");
                }
                //Time stamp
                if (state->timeStamp)
                {
                    DSecond_t *time_stamp = new DSecond_t;
                    time_stamp = state->timeStamp;
                    intersection.time_stamp = *time_stamp;
                    intersection.time_stamp_exists = true;
                }
                else
                {
                    intersection.time_stamp = j2735_v2x_msgs::msg::IntersectionState::TIME_STAMP_UNAVAILABLE;
                    intersection.time_stamp_exists = false;
                    // RCLCPP_DEBUG_STREAM(  get_logger(), "Intersection time stamp doesn't exist, value set to TIME_STAMP_UNAVAILABLE");
                }
                //Enabled lanes list
                
                bool enabled_lanes_exists = false;
                if (state->enabledLanes)
                {
                    enabled_lanes_exists = true;
                    j2735_v2x_msgs::msg::EnabledLaneList enabled_lanes_list;
                    for (size_t j = 0; j < state->enabledLanes->list.count; j++)
                    {
                        LaneID_t *enabled_lane_id = new LaneID_t;
                        enabled_lane_id = state->enabledLanes->list.array[j];
                        enabled_lanes_list.lane_id_list.push_back(*enabled_lane_id);
                    }
                    intersection.enabled_lanes = enabled_lanes_list;
                }
                else{
                    intersection.enabled_lanes_exists = false;
                }
                intersection.enabled_lanes_exists = enabled_lanes_exists;

                //MovementList
                j2735_v2x_msgs::msg::MovementList movement_states;
                //movement states is an array of Movement states
                for (size_t j = 0; j < state->states.list.count; j++)
                {
                    if(!state->states.list.array[j]){
                        continue;
                    }
                    j2735_v2x_msgs::msg::MovementState movement_state;
                    movement_state.movement_name_exists = false;
                    if (state->states.list.array[j]->movementName)
                    {
                        movement_state.movement_name_exists = true;

                        size_t len = state->states.list.array[j]->movementName->size;
                        for (int k = 0; k < len; k++)
                        {
                            movement_state.movement_name += state->states.list.array[j]->movementName->buf[k];
                        }

                    }

                    //Signal Group ID
                    movement_state.signal_group = SIGNAL_GROUP_UNAVAILABLE_;
                    if (state->states.list.array[j]->signalGroup)
                    {
                        movement_state.signal_group = state->states.list.array[j]->signalGroup;
                    }

                    //State Time Speed Movement Event List
                    j2735_v2x_msgs::msg::MovementEventList movement_event_list;
                    for (int k = 0; k < state->states.list.array[j]->state_time_speed.list.count; k++)
                    {
                        if(!state->states.list.array[j]->state_time_speed.list.array[k]){
                            continue;
                        }
                        j2735_v2x_msgs::msg::MovementEvent movement_event;
                        //Decode movement event
                        //1. MovementPhaseState
                        if(state->states.list.array[j]->state_time_speed.list.array[k]->eventState){
                            movement_event.event_state.movement_phase_state = state->states.list.array[j]->state_time_speed.list.array[k]->eventState;
                        }
                        else{
                            movement_event.event_state.movement_phase_state = j2735_v2x_msgs::msg::MovementPhaseState::UNAVAILABLE;
                            // RCLCPP_DEBUG_STREAM(  get_logger(), "Movement event - event state, value doesn't exist. Set to default UNAVAILABLE");
                        }

                        //2. TimeChangeDetails
                        movement_event.timing_exists = false;
                        if (state->states.list.array[j]->state_time_speed.list.array[k]->timing)
                        {
                            movement_event.timing_exists = true;

                            j2735_v2x_msgs::msg::TimeChangeDetails timing;
                            timing.start_time_exists = false;
                            if (state->states.list.array[j]->state_time_speed.list.array[k]->timing->startTime)
                            {
                                timing.start_time_exists = true;
                                SPAT_TimeMark_t *start_time = new SPAT_TimeMark_t;
                                start_time = state->states.list.array[j]->state_time_speed.list.array[k]->timing->startTime;
                                timing.start_time = *start_time;
                            }

                            timing.min_end_time = state->states.list.array[j]->state_time_speed.list.array[k]->timing->minEndTime;

                            timing.max_end_time_exists = false;
                            if (state->states.list.array[j]->state_time_speed.list.array[k]->timing->maxEndTime)
                            {
                                timing.max_end_time_exists = true;
                                SPAT_TimeMark_t *end_time = new SPAT_TimeMark_t;
                                end_time = state->states.list.array[j]->state_time_speed.list.array[k]->timing->maxEndTime;
                                timing.max_end_time = *end_time;
                            }
                            timing.likely_time_exists = false;
                            if (state->states.list.array[j]->state_time_speed.list.array[k]->timing->likelyTime)
                            {
                                timing.likely_time_exists = true;
                                SPAT_TimeMark_t *likely_time = new SPAT_TimeMark_t;
                                likely_time = state->states.list.array[j]->state_time_speed.list.array[k]->timing->likelyTime;
                                timing.likely_time = *likely_time;
                            }

                            timing.confidence_exists = false;
                            if (state->states.list.array[j]->state_time_speed.list.array[k]->timing->confidence)
                            {
                                timing.confidence_exists = true;
                                TimeIntervalConfidence_t *confidence = new TimeIntervalConfidence_t;
                                confidence = state->states.list.array[j]->state_time_speed.list.array[k]->timing->confidence;
                                timing.confidence = *confidence;
                            }

                            timing.next_time_exists = false;
                            if (state->states.list.array[j]->state_time_speed.list.array[k]->timing->nextTime)
                            {
                                timing.next_time_exists = true;
                                SPAT_TimeMark_t *next_time = new SPAT_TimeMark_t;
                                next_time = state->states.list.array[j]->state_time_speed.list.array[k]->timing->nextTime;
                                timing.next_time = *next_time;
                            }
                            movement_event.timing = timing;
                        }
                        //3. Advisory Speed List
                        movement_event.speeds_exists = false;
                        if (state->states.list.array[j]->state_time_speed.list.array[k]->speeds)
                        {
                            movement_event.speeds_exists = true;
                            
                            for (size_t l = 0; l < state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.count; l++)
                            {
                                j2735_v2x_msgs::msg::AdvisorySpeed advisory_speed;

                                advisory_speed.type.advisory_speed_type = state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.array[l]->type;

                                advisory_speed.speed_exists = false;
                                if (state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.array[l]->speed)
                                {
                                    advisory_speed.speed_exists = true;
                                    SpeedAdvice_t *speed_advice = new SpeedAdvice_t;
                                    speed_advice = state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.array[l]->speed;
                                    advisory_speed.speed = *speed_advice;
                                }
                                else
                                {
                                    advisory_speed.speed = j2735_v2x_msgs::msg::AdvisorySpeed::SPEED_UNAVAILABLE;
                                    // RCLCPP_DEBUG_STREAM(  get_logger(), "Advisory speed- speed doesn't exist, assigned default value speed_unavailable");
                                }

                                SpeedConfidence_t *confidence = new SpeedConfidence_t;
                                confidence = state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.array[l]->confidence;
                                advisory_speed.confidence.speed_confidence = *confidence;

                                advisory_speed.distance_exists = false;
                                if (state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.array[l]->distance)
                                {
                                    advisory_speed.distance_exists = true;
                                    ZoneLength_t *distance = new ZoneLength_t;
                                    distance = state->states.list.array[j]->state_time_speed.list.array[k]->speeds->list.array[l]->distance;
                                    advisory_speed.distance = *distance;
                                }
                                else
                                {
                                    advisory_speed.distance = j2735_v2x_msgs::msg::AdvisorySpeed::DISTANCE_UNKNOWN;
                                    // RCLCPP_DEBUG_STREAM(  get_logger(), "Advisory speed - distance, value doesn't exist, set to default distance_unknown");
                                }

                                // RESTRICTION CLASS ID not DEFINED in incoming state message

                                movement_event.speeds.advisory_speed_list.push_back(advisory_speed);
                            }
                        }

                        movement_event_list.movement_event_list.push_back(movement_event);
                    }
                    movement_state.state_time_speed.movement_event_list = movement_event_list.movement_event_list;
                    movement_states.movement_list.push_back(movement_state);
                }
                intersection.states = movement_states;

                //ManeuverAssistList
                j2735_v2x_msgs::msg::ManeuverAssistList maneuver_assist_list;
                bool maneuver_assist_list_exists = false;
                if (state->maneuverAssistList)
                {
                    maneuver_assist_list_exists = true;
                }
                intersection.maneuever_assist_list_exists = maneuver_assist_list_exists;
                intersection.maneuever_assist_list = maneuver_assist_list;

                // roadAuthorityID
                if (state->roadAuthorityID)
                {
                    intersection.road_authority_id_exists = true;
                    if (state->roadAuthorityID->present == RoadAuthorityID_PR_fullRdAuthID)
                    {
                        intersection.road_authority_id.choice = j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID;
                        auto& oid = state->roadAuthorityID->choice.fullRdAuthID;
                        intersection.road_authority_id.full_rd_auth_id.assign(oid.buf, oid.buf + oid.size);
                    }
                    else if (state->roadAuthorityID->present == RoadAuthorityID_PR_relRdAuthID)
                    {
                        intersection.road_authority_id.choice = j2735_v2x_msgs::msg::RoadAuthorityID::RELATIVE_ROAD_AUTHORITY_ID;
                        auto& oid = state->roadAuthorityID->choice.relRdAuthID;
                        intersection.road_authority_id.rel_rd_auth_id.assign(oid.buf, oid.buf + oid.size);
                    }
                }
                else
                {
                    intersection.road_authority_id_exists = false;
                }

                output.intersections.intersection_state_list.push_back(intersection);
            }

            //4. Regional - not implemented yet

            ASN_STRUCT_FREE(asn_DEF_MessageFrame, message);
            return boost::optional<j2735_v2x_msgs::msg::SPAT>(output);
        }

        // RCLCPP_WARN_STREAM( rclcpp::get_logger(), "SPAT Message decoding failed");
        ASN_STRUCT_FREE(asn_DEF_MessageFrame, message);
        return boost::optional<j2735_v2x_msgs::msg::SPAT>{};
    }

    boost::optional<std::vector<uint8_t>> SPAT_Message::encode_spat_message(const j2735_v2x_msgs::msg::SPAT &plainMessage)
    {
        std::vector<std::shared_ptr<void>> shared_ptrs;

        uint8_t buffer[2048] = {0};
        size_t buffer_size = sizeof(buffer);
        asn_enc_rval_t ec;
        MessageFrame_t *message = create_store_shared<MessageFrame_t>(shared_ptrs);

        message->messageId = 19;
        message->value.present = MessageFrame__value_PR_SPAT;

        //Encode timestamp
        auto timestamp = create_store_shared<MinuteOfTheYear_t>(shared_ptrs);
        if(plainMessage.time_stamp_exists){
            *timestamp = plainMessage.time_stamp;
        }
        else{
            *timestamp = DEFAULT_TIME_STAMP_;
        }
        message->value.choice.SPAT.timeStamp = timestamp;

        //Encode Descriptive Name
        if(plainMessage.name_exists){
            auto spat_name = create_store_shared<DescriptiveName_t>(shared_ptrs);
            size_t name_size = plainMessage.name.size();
            auto name_buf = create_store_shared_array<uint8_t>(shared_ptrs, name_size);
            for(size_t i = 0; i < name_size; i++){
                name_buf[i] = plainMessage.name[i];
            }
            spat_name->buf = name_buf;
            spat_name->size = name_size;
            message->value.choice.SPAT.name = spat_name;
        }

        //Encode Intersections
        for(size_t i = 0; i < plainMessage.intersections.intersection_state_list.size(); i++)
        {
            auto intersectionState = create_store_shared<IntersectionState_t>(shared_ptrs);

            if(plainMessage.intersections.intersection_state_list[i].name_exists){
                auto int_name = create_store_shared<DescriptiveName_t>(shared_ptrs);
                size_t name_string_size = plainMessage.intersections.intersection_state_list[i].name.size();
                auto name_buf = create_store_shared_array<uint8_t>(shared_ptrs, name_string_size);
                for(size_t j = 0; j < name_string_size; j++){
                    name_buf[j] = plainMessage.intersections.intersection_state_list[i].name[j];
                }
                int_name->buf = name_buf;
                int_name->size = name_string_size;
                intersectionState->name = int_name;
            }

            intersectionState->id.id = plainMessage.intersections.intersection_state_list[i].id.id;
            intersectionState->revision = plainMessage.intersections.intersection_state_list[i].revision;

            //Encode Intersection Status (16-bit BIT STRING = 2 bytes)
            auto status_buf = create_store_shared_array<uint8_t>(shared_ptrs, 2);
            int bit_to_set = plainMessage.intersections.intersection_state_list[i].status.intersection_status_object;
            if(bit_to_set >= 0 && bit_to_set < 14){
                status_buf[bit_to_set / 8] |= (0x80 >> (bit_to_set % 8));
            }
            intersectionState->status.buf = status_buf;
            intersectionState->status.size = 2;

            //Encode MinuteOfTheYear
            auto minute_of_year = create_store_shared<MinuteOfTheYear_t>(shared_ptrs);
            if(plainMessage.intersections.intersection_state_list[i].moy_exists){
               *minute_of_year = plainMessage.intersections.intersection_state_list[i].moy;
            }
            else{
                *minute_of_year = plainMessage.intersections.intersection_state_list[i].MOY_INVALID;
            }
            intersectionState->moy = minute_of_year;

            //Encode time stamp
            auto state_time_stamp = create_store_shared<DSecond_t>(shared_ptrs);
            if(plainMessage.intersections.intersection_state_list[i].time_stamp_exists){
                *state_time_stamp = plainMessage.intersections.intersection_state_list[i].time_stamp;
            }
            else{
                *state_time_stamp = plainMessage.intersections.intersection_state_list[i].TIME_STAMP_UNAVAILABLE;
            }
            intersectionState->timeStamp = state_time_stamp;

            //Encode Enabled lanes
            if(plainMessage.intersections.intersection_state_list[i].enabled_lanes_exists){
                auto enabled_lanes = create_store_shared<EnabledLaneList_t>(shared_ptrs);
                for(size_t j = 0; j < plainMessage.intersections.intersection_state_list[i].enabled_lanes.lane_id_list.size(); j++){
                    auto lane_id = create_store_shared<LaneID_t>(shared_ptrs);
                    *lane_id = plainMessage.intersections.intersection_state_list[i].enabled_lanes.lane_id_list[j];
                    asn_sequence_add(&enabled_lanes->list, lane_id);
                }
                intersectionState->enabledLanes = enabled_lanes;
            }

            //Encode Movement States
            for(size_t j = 0; j < plainMessage.intersections.intersection_state_list[i].states.movement_list.size(); j++){
                auto movementstate = create_store_shared<MovementState_t>(shared_ptrs);

                //Movement name
                if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].movement_name_exists){
                    auto mv_name = create_store_shared<DescriptiveName_t>(shared_ptrs);
                    size_t movement_name_size = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].movement_name.size();
                    auto mv_name_buf = create_store_shared_array<uint8_t>(shared_ptrs, movement_name_size);
                    for(size_t k = 0; k < movement_name_size; k++){
                        mv_name_buf[k] = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].movement_name[k];
                    }
                    mv_name->buf = mv_name_buf;
                    mv_name->size = movement_name_size;
                    movementstate->movementName = mv_name;
                }

                //Signal group
                if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].signal_group){
                    movementstate->signalGroup = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].signal_group;
                }
                else{
                    movementstate->signalGroup = DEFAULT_SIGNAL_GROUP_;
                }

                //Encode State time speed
                for(size_t k = 0; k < plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list.size(); k++){
                    auto movement_event = create_store_shared<MovementEvent_t>(shared_ptrs);

                    movement_event->eventState = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].event_state.movement_phase_state;

                    //Time Change Details
                    if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing_exists){
                        auto time_change_details = create_store_shared<TimeChangeDetails_t>(shared_ptrs);

                        //start time
                        auto state_start_time = create_store_shared<SPAT_TimeMark_t>(shared_ptrs);
                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.start_time_exists){
                            *state_start_time = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.start_time;
                        }
                        else{
                            *state_start_time = DEFAULT_TIME_MARK_;
                        }
                        time_change_details->startTime = state_start_time;

                        //min_end_time
                        time_change_details->minEndTime = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.min_end_time;

                        //max end time
                        auto state_max_end_time = create_store_shared<SPAT_TimeMark_t>(shared_ptrs);
                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.max_end_time_exists){
                            *state_max_end_time = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.max_end_time;
                        }
                        else{
                            *state_max_end_time = DEFAULT_TIME_MARK_;
                        }
                        time_change_details->maxEndTime = state_max_end_time;

                        //likely time
                        auto state_likely_time = create_store_shared<SPAT_TimeMark_t>(shared_ptrs);
                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.likely_time_exists){
                            *state_likely_time = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.likely_time;
                        }
                        else{
                            *state_likely_time = DEFAULT_TIME_MARK_;
                        }
                        time_change_details->likelyTime = state_likely_time;

                        //confidence
                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.confidence_exists){
                            auto state_confidence = create_store_shared<TimeIntervalConfidence_t>(shared_ptrs);
                            *state_confidence = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.confidence;
                            time_change_details->confidence = state_confidence;
                        }

                        //next time
                        auto next_time = create_store_shared<SPAT_TimeMark_t>(shared_ptrs);
                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.next_time_exists){
                            *next_time = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].timing.next_time;
                        }
                        else{
                            *next_time = DEFAULT_TIME_MARK_;
                        }
                        time_change_details->nextTime = next_time;

                        movement_event->timing = time_change_details;
                    }

                    //Advisory Speed List
                    if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds_exists){
                        auto advisory_speed_list = create_store_shared<AdvisorySpeedList_t>(shared_ptrs);
                        for(size_t l = 0; l < plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list.size(); l++){
                            auto advisory_speed = create_store_shared<AdvisorySpeed_t>(shared_ptrs);

                            advisory_speed->type = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list[l].type.advisory_speed_type;

                            auto speed_advice = create_store_shared<SpeedAdvice_t>(shared_ptrs);
                            if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list[l].speed_exists){
                                *speed_advice = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list[l].speed;
                            }
                            else{
                               *speed_advice = j2735_v2x_msgs::msg::AdvisorySpeed::SPEED_UNAVAILABLE;
                            }
                            advisory_speed->speed = speed_advice;

                            auto speed_confidence = create_store_shared<SpeedConfidence_t>(shared_ptrs);
                            *speed_confidence = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list[l].confidence.speed_confidence;
                            advisory_speed->confidence = speed_confidence;

                            auto zone_length = create_store_shared<ZoneLength_t>(shared_ptrs);
                            if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list[l].distance_exists){
                                *zone_length = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].state_time_speed.movement_event_list[k].speeds.advisory_speed_list[l].distance;
                            }
                            else{
                                *zone_length = j2735_v2x_msgs::msg::AdvisorySpeed::DISTANCE_UNKNOWN;
                            }
                            advisory_speed->distance = zone_length;

                            asn_sequence_add(&advisory_speed_list->list, advisory_speed);
                        }
                        movement_event->speeds = advisory_speed_list;
                    }

                    asn_sequence_add(&movementstate->state_time_speed.list, movement_event);
                }

                //Maneuver Assist List
                if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list_exists){
                    auto maneuver_assist_list = create_store_shared<ManeuverAssistList_t>(shared_ptrs);
                    size_t maneuver_assist_list_size = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list.size();

                    for(size_t k = 0; k < maneuver_assist_list_size; k++){
                        auto connection_assist = create_store_shared<ConnectionManeuverAssist_t>(shared_ptrs);
                        connection_assist->connectionID = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].connection_id;

                        auto queue_length = create_store_shared<ZoneLength_t>(shared_ptrs);
                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].queue_length_exists){
                            *queue_length = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].queue_length;
                        }
                        else{
                            *queue_length = DEFAULT_QUEUE_LENGTH_;
                        }
                        connection_assist->queueLength = queue_length;

                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].wait_on_stop_exists){
                            auto wait_on_stop = create_store_shared<WaitOnStopline_t>(shared_ptrs);
                            *wait_on_stop = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].wait_on_stop;
                            connection_assist->waitOnStop = wait_on_stop;
                        }

                        if(plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].ped_bicycle_detect_exists){
                            auto ped_bicycle_detect = create_store_shared<PedestrianBicycleDetect_t>(shared_ptrs);
                            *ped_bicycle_detect = plainMessage.intersections.intersection_state_list[i].states.movement_list[j].maneuver_assist_list.connection_maneuver_assist_list[k].ped_bicycle_detect;
                            connection_assist->pedBicycleDetect = ped_bicycle_detect;
                        }

                        asn_sequence_add(&maneuver_assist_list->list, connection_assist);
                    }
                    movementstate->maneuverAssistList = maneuver_assist_list;
                }

                asn_sequence_add(&intersectionState->states.list, movementstate);
            }

            // roadAuthorityID
            if(plainMessage.intersections.intersection_state_list[i].road_authority_id_exists){
                auto road_auth_id = create_store_shared<RoadAuthorityID_t>(shared_ptrs);
                const auto& ra_msg = plainMessage.intersections.intersection_state_list[i].road_authority_id;
                if(ra_msg.choice == j2735_v2x_msgs::msg::RoadAuthorityID::FULL_ROAD_AUTHORITY_ID){
                    road_auth_id->present = RoadAuthorityID_PR_fullRdAuthID;
                    road_auth_id->choice.fullRdAuthID.size = ra_msg.full_rd_auth_id.size();
                    auto rd_buf = create_store_shared_array<uint8_t>(shared_ptrs, ra_msg.full_rd_auth_id.size());
                    memcpy(rd_buf, ra_msg.full_rd_auth_id.data(), ra_msg.full_rd_auth_id.size());
                    road_auth_id->choice.fullRdAuthID.buf = rd_buf;
                }
                else if(ra_msg.choice == j2735_v2x_msgs::msg::RoadAuthorityID::RELATIVE_ROAD_AUTHORITY_ID){
                    road_auth_id->present = RoadAuthorityID_PR_relRdAuthID;
                    road_auth_id->choice.relRdAuthID.size = ra_msg.rel_rd_auth_id.size();
                    auto rd_buf = create_store_shared_array<uint8_t>(shared_ptrs, ra_msg.rel_rd_auth_id.size());
                    memcpy(rd_buf, ra_msg.rel_rd_auth_id.data(), ra_msg.rel_rd_auth_id.size());
                    road_auth_id->choice.relRdAuthID.buf = rd_buf;
                }
                intersectionState->roadAuthorityID = road_auth_id;
            }

            asn_sequence_add(&message->value.choice.SPAT.intersections.list, intersectionState);
        }

        //encode message
        char errbuf[256] = {0};
        size_t errlen = sizeof(errbuf);
        if(asn_check_constraints(&asn_DEF_MessageFrame, message, errbuf, &errlen) != 0){
            RCLCPP_WARN_STREAM( node_logging_::get_logger(), "SPAT constraint check failed: " << errbuf);
        }
        ec = uper_encode_to_buffer(&asn_DEF_MessageFrame, 0 , message, buffer, buffer_size);
        if(ec.encoded == -1) {
            return boost::optional<std::vector<uint8_t>>{};
        }

        size_t array_length=(ec.encoded + 7) / 8;
        std::vector<uint8_t> b_array(array_length);
        for(size_t i=0;i<array_length;i++)b_array[i]=buffer[i];

        return boost::optional<std::vector<uint8_t>>(b_array);
    }

}
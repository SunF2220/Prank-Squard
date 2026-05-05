#pragma once

#include "archetype_templates.h"
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

// 骨骼姿势定义
struct SkeletonPose {
    godot::Vector2 head_pos;
    godot::Vector2 neck_pos;
    godot::Vector2 hip_pos;
    godot::Vector2 left_shoulder;
    godot::Vector2 left_elbow;
    godot::Vector2 left_hand;
    godot::Vector2 right_shoulder;
    godot::Vector2 right_elbow;
    godot::Vector2 right_hand;
    godot::Vector2 left_knee;
    godot::Vector2 left_foot;
    godot::Vector2 right_knee;
    godot::Vector2 right_foot;
};

// 动作引擎 - 根据动作类型生成每帧的骨骼姿势
class PoseEngine {
public:
    PoseEngine();
    ~PoseEngine() = default;

    void set_archetype(const ArchetypeParams& arch) { archetype_ = arch; }
    SkeletonPose get_pose(int frame_idx, int total_frames) const;

    static SkeletonPose idle_pose(float time);
    static SkeletonPose walk_pose(float time);
    static SkeletonPose punch_pose(float progress);
    static SkeletonPose kick_pose(float progress);
    static SkeletonPose jump_pose(float time);
    static SkeletonPose hurt_pose(float time);
    static SkeletonPose defend_pose(float time);

private:
    ArchetypeParams archetype_;
};

} // namespace prank

#include "pose_engine.h"

namespace prank {

PoseEngine::PoseEngine() {
}

SkeletonPose PoseEngine::get_pose(int frame_idx, int total_frames) const {
    float progress = (total_frames > 1) ? static_cast<float>(frame_idx) / (total_frames - 1) : 0.0f;
    return idle_pose(progress);
}

SkeletonPose PoseEngine::idle_pose(float time) {
    SkeletonPose p;
    float breath = godot::Math::sin(time * 2.0f) * 2.0f;

    p.head_pos = godot::Vector2(0, -95 + breath);
    p.neck_pos = godot::Vector2(0, -75 + breath);
    p.hip_pos = godot::Vector2(0, -35);
    p.left_shoulder = godot::Vector2(-10, -72 + breath);
    p.left_elbow = godot::Vector2(-18, -52);
    p.left_hand = godot::Vector2(-22, -35);
    p.right_shoulder = godot::Vector2(10, -72 + breath);
    p.right_elbow = godot::Vector2(18, -52);
    p.right_hand = godot::Vector2(22, -35);
    p.left_knee = godot::Vector2(-5, -15);
    p.left_foot = godot::Vector2(-8, 0);
    p.right_knee = godot::Vector2(5, -15);
    p.right_foot = godot::Vector2(8, 0);

    return p;
}

SkeletonPose PoseEngine::walk_pose(float time) {
    SkeletonPose p = idle_pose(0.0f);
    float swing = godot::Math::sin(time * 3.0f) * 12.0f;

    p.left_elbow.x -= 5 + swing * 0.5f;
    p.left_hand.x -= 3 + swing;
    p.right_elbow.x += 5 - swing * 0.5f;
    p.right_hand.x += 3 - swing;
    p.left_knee.x += swing * 0.3f;
    p.left_foot.x += swing * 0.5f;
    p.right_knee.x -= swing * 0.3f;
    p.right_foot.x -= swing * 0.5f;

    return p;
}

SkeletonPose PoseEngine::punch_pose(float progress) {
    SkeletonPose p = idle_pose(0.0f);

    if (progress < 0.5f) {
        // Wind up
        p.right_shoulder.x += 5;
        p.right_elbow.x += 10;
        p.right_hand.x += 15;
        p.right_hand.y -= 10;
    } else {
        // Extend
        p.right_shoulder.x += 10;
        p.right_elbow.x += 35;
        p.right_hand.x += 50;
        p.right_hand.y -= 5;
    }

    return p;
}

SkeletonPose PoseEngine::kick_pose(float progress) {
    SkeletonPose p = idle_pose(0.0f);

    if (progress < 0.5f) {
        p.right_knee.x += 15;
        p.right_knee.y -= 30;
        p.right_foot.x += 20;
        p.right_foot.y -= 20;
    } else {
        p.right_knee.x += 25;
        p.right_knee.y -= 10;
        p.right_foot.x += 40;
        p.right_foot.y -= 5;
    }

    return p;
}

SkeletonPose PoseEngine::jump_pose(float time) {
    SkeletonPose p = idle_pose(0.0f);
    p.right_knee.y -= 20;
    p.right_foot.y -= 25;
    p.left_knee.y -= 15;
    p.left_foot.y -= 20;
    p.left_elbow.y -= 8;
    p.right_elbow.y -= 5;
    return p;
}

SkeletonPose PoseEngine::hurt_pose(float time) {
    SkeletonPose p = idle_pose(0.0f);
    float wobble = godot::Math::sin(time * 10.0f) * 3.0f;
    p.head_pos.x += wobble;
    p.neck_pos.x += wobble;
    p.hip_pos.x += wobble * 0.5f;
    return p;
}

SkeletonPose PoseEngine::defend_pose(float time) {
    SkeletonPose p = idle_pose(0.0f);
    p.left_elbow.x -= 5;
    p.left_elbow.y -= 15;
    p.left_hand.x -= 8;
    p.left_hand.y -= 25;
    p.right_elbow.x += 5;
    p.right_elbow.y -= 15;
    p.right_hand.x += 8;
    p.right_hand.y -= 25;
    return p;
}

} // namespace prank

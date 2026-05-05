#include "balance_validator.h"

namespace prank {

BalanceValidator::BalanceValidator() {
}

BalanceValidator::BalanceReport BalanceValidator::validate(
        const CharacterStats& stats,
        const godot::Vector<FrameData>& frames) const {

    BalanceReport report;

    // Validate stat ranges
    if (stats.max_hp < MIN_HP) {
        report.warnings += "HP too low. ";
        report.is_valid = false;
    }
    if (stats.max_hp > MAX_HP) {
        report.warnings += "HP too high. ";
        report.is_valid = false;
    }
    if (stats.max_mp < MIN_MP) {
        report.warnings += "MP too low. ";
        report.is_valid = false;
    }
    if (stats.max_mp > MAX_MP) {
        report.warnings += "MP too high. ";
        report.is_valid = false;
    }
    if (stats.speed < MIN_SPEED) {
        report.warnings += "Speed too low. ";
        report.is_valid = false;
    }
    if (stats.speed > MAX_SPEED) {
        report.warnings += "Speed too high. ";
        report.is_valid = false;
    }
    if (stats.weight < MIN_WEIGHT) {
        report.warnings += "Weight too low. ";
        report.is_valid = false;
    }
    if (stats.weight > MAX_WEIGHT) {
        report.warnings += "Weight too high. ";
        report.is_valid = false;
    }

    // Calculate total combo damage
    int combo_damage = 0;
    for (const auto& frame : frames) {
        for (int i = 0; i < frame.itr_count; ++i) {
            combo_damage += frame.itrs[i].damage;
        }
    }
    report.total_damage_output = combo_damage;

    if (combo_damage > MAX_TOTAL_DAMAGE) {
        report.warnings += "Combo damage too high (" + godot::String::num_int64(combo_damage) + "). ";
        report.is_valid = false;
    }

    // Power rating = weighted sum of stats
    report.power_rating = stats.max_hp * 0.3f + stats.max_mp * 0.1f
                        + stats.speed * 15.0f + stats.jump_power * 5.0f
                        + combo_damage * 1.5f;

    if (report.warnings.is_empty()) {
        report.warnings = "All checks passed.";
    }

    return report;
}

} // namespace prank

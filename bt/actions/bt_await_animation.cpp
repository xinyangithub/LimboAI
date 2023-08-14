/**
 * bt_await_animation.cpp
 * =============================================================================
 * Copyright 2021-2023 Serhii Snitsaruk
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#include "bt_await_animation.h"

//**** Setters / Getters

void BTAwaitAnimation::set_animation_player(Ref<BBNode> p_animation_player) {
	animation_player_param = p_animation_player;
	emit_changed();
	if (Engine::get_singleton()->is_editor_hint() && animation_player_param.is_valid()) {
		animation_player_param->connect(SNAME("changed"), Callable(this, SNAME("emit_changed")));
	}
}

void BTAwaitAnimation::set_animation_name(StringName p_animation_name) {
	animation_name = p_animation_name;
	emit_changed();
}

void BTAwaitAnimation::set_max_time(double p_max_time) {
	max_time = p_max_time;
	emit_changed();
}

//**** Task Implementation

String BTAwaitAnimation::get_configuration_warning() const {
	String warning = BTAction::get_configuration_warning();
	if (!warning.is_empty()) {
		warning += "\n";
	}

	if (animation_player_param.is_null()) {
		warning += "Animation Player parameter is not set.\n";
	} else {
		if (animation_player_param->get_value_source() == BBParam::SAVED_VALUE && animation_player_param->get_saved_value().is_zero()) {
			warning += "Path to AnimationPlayer node is not set.\n";
		} else if (animation_player_param->get_value_source() == BBParam::BLACKBOARD_VAR && animation_player_param->get_variable().is_empty()) {
			warning += "AnimationPlayer blackboard variable is not set.\n";
		}
	}
	if (animation_name == StringName()) {
		warning += "Animation Name is required in order to wait for the animation to finish.\n";
	}
	if (max_time <= 0.0) {
		warning += "Max time should be greater than 0.0.\n";
	}

	return warning;
}

String BTAwaitAnimation::_generate_name() const {
	return "AwaitAnimation" +
			(animation_name != StringName() ? vformat(" \"%s\"", animation_name) : " ???") +
			vformat("  max_time: %ss", Math::snapped(max_time, 0.001));
}

void BTAwaitAnimation::_setup() {
	setup_failed = true;
	ERR_FAIL_COND_MSG(animation_player_param.is_null(), "BTAwaitAnimation: AnimationPlayer parameter is not set.");
	animation_player = Object::cast_to<AnimationPlayer>(animation_player_param->get_value(get_agent(), get_blackboard()));
	ERR_FAIL_COND_MSG(animation_player == nullptr, "BTAwaitAnimation: Failed to get AnimationPlayer.");
	ERR_FAIL_COND_MSG(animation_name == StringName(), "BTAwaitAnimation: Animation Name is not set.");
	ERR_FAIL_COND_MSG(!animation_player->has_animation(animation_name), vformat("BTAwaitAnimation: Animation not found: %s", animation_name));
	setup_failed = false;
}

int BTAwaitAnimation::_tick(double p_delta) {
	ERR_FAIL_COND_V_MSG(setup_failed == true, FAILURE, "BTAwaitAnimation: _setup() failed - returning FAILURE.");

	// ! Doing this check instead of using signal due to a bug in Godot: https://github.com/godotengine/godot/issues/76127
	if (animation_player->is_playing() && animation_player->get_assigned_animation() == animation_name) {
		if (get_elapsed_time() < max_time) {
			return RUNNING;
		} else if (max_time > 0.0) {
			WARN_PRINT(vformat("BTAwaitAnimation: Waiting time for the \"%s\" animation exceeded the allocated %s sec.", animation_name, max_time));
		}
	}
	return SUCCESS;
}

//**** Godot

void BTAwaitAnimation::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_animation_player", "p_anim_player"), &BTAwaitAnimation::set_animation_player);
	ClassDB::bind_method(D_METHOD("get_animation_player"), &BTAwaitAnimation::get_animation_player);
	ClassDB::bind_method(D_METHOD("set_animation_name", "p_anim_name"), &BTAwaitAnimation::set_animation_name);
	ClassDB::bind_method(D_METHOD("get_animation_name"), &BTAwaitAnimation::get_animation_name);
	ClassDB::bind_method(D_METHOD("set_max_time", "p_time_sec"), &BTAwaitAnimation::set_max_time);
	ClassDB::bind_method(D_METHOD("get_max_time"), &BTAwaitAnimation::get_max_time);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation_player", PROPERTY_HINT_RESOURCE_TYPE, "BBNode"), "set_animation_player", "get_animation_player");
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "animation_name"), "set_animation_name", "get_animation_name");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_time", PROPERTY_HINT_RANGE, "0.0,100.0"), "set_max_time", "get_max_time");
}

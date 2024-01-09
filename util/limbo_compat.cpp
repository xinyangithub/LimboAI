/**
 * limbo_def.cpp
 * =============================================================================
 * Copyright 2021-2023 Serhii Snitsaruk
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#include "limbo_compat.h"

#ifdef LIMBOAI_MODULE

void EDIT_SCRIPT(const String &p_path) {
	Ref<Resource> res = ScriptEditor::get_singleton()->open_file(p_path);
	ERR_FAIL_COND_MSG(res.is_null(), "Failed to load script: " + p_path);
	EditorNode::get_singleton()->edit_resource(res);
}

#endif // LIMBOAI_MODULE

#ifdef LIMBOAI_GDEXTENSION

#include "godot_cpp/classes/editor_interface.hpp"
#include <godot_cpp/classes/editor_settings.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/script.hpp>
#include <godot_cpp/classes/translation_server.hpp>

using namespace godot;

Variant _EDITOR_GET(const String &p_setting) {
	Ref<EditorSettings> es = EditorInterface::get_singleton()->get_editor_settings();
	ERR_FAIL_COND_V(es.is_null() || !es->has_setting(p_setting), Variant());
	return es->get(p_setting);
}

String TTR(const String &p_text, const String &p_context) {
	if (TranslationServer::get_singleton()) {
		return TranslationServer::get_singleton()->translate(p_text, p_context);
	}

	return p_text;
}

void EDIT_SCRIPT(const String &p_path) {
	Ref<Script> res = RESOURCE_LOAD(p_path, "Script");
	ERR_FAIL_COND_MSG(res.is_null(), "Failed to load script: " + p_path);
	EditorInterface::get_singleton()->edit_script(res);
}

Variant _GLOBAL_DEF(const String &p_var, const Variant &p_default, bool p_restart_if_changed, bool p_ignore_value_in_docs, bool p_basic, bool p_internal) {
	Variant ret;
	if (!ProjectSettings::get_singleton()->has_setting(p_var)) {
		ProjectSettings::get_singleton()->set(p_var, p_default);
	}
	ret = GLOBAL_GET(p_var);

	ProjectSettings::get_singleton()->set_initial_value(p_var, p_default);
	// ProjectSettings::get_singleton()->set_builtin_order(p_var);
	ProjectSettings::get_singleton()->set_as_basic(p_var, p_basic);
	ProjectSettings::get_singleton()->set_restart_if_changed(p_var, p_restart_if_changed);
	// ProjectSettings::get_singleton()->set_ignore_value_in_docs(p_var, p_ignore_value_in_docs);
	ProjectSettings::get_singleton()->set_as_internal(p_var, p_internal);
	return ret;
}

Variant _GLOBAL_DEF(const PropertyInfo &p_info, const Variant &p_default, bool p_restart_if_changed, bool p_ignore_value_in_docs, bool p_basic, bool p_internal) {
	Variant ret = _GLOBAL_DEF(p_info.name, p_default, p_restart_if_changed, p_ignore_value_in_docs, p_basic, p_internal);

	Dictionary dic_info;
	dic_info["type"] = p_info.type;
	dic_info["name"] = p_info.name;
	dic_info["class_name"] = p_info.class_name;
	dic_info["hint"] = p_info.hint;
	dic_info["hint_string"] = p_info.hint_string;
	dic_info["usage"] = p_info.usage;

	ProjectSettings::get_singleton()->add_property_info(dic_info);
	return ret;
}

#endif // LIMBOAI_GDEXTENSION
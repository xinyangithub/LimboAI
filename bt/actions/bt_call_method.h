/**
 * bt_call_method.h
 * =============================================================================
 * Copyright 2021-2023 Serhii Snitsaruk
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#ifndef BT_CALL_METHOD_H
#define BT_CALL_METHOD_H

#include "bt_action.h"

#include "modules/limboai/blackboard/bb_param/bb_node.h"

class BTCallMethod : public BTAction {
	GDCLASS(BTCallMethod, BTAction);

private:
	StringName method_name;
	Ref<BBNode> node_param;
	Array args;

protected:
	static void _bind_methods();

	virtual String _generate_name() const override;
	virtual int _tick(double p_delta) override;

public:
	void set_method_name(StringName p_method_name);
	StringName get_method_name() const { return method_name; }

	void set_node_param(Ref<BBNode> p_object);
	Ref<BBNode> get_node_param() const { return node_param; }

	void set_args(Array p_args);
	Array get_args() const { return args; }

	virtual String get_configuration_warning() const override;
};

#endif // BT_CALL_METHOD
/* limbo_ai_editor_plugin.h */

#ifndef LIMBO_AI_EDITOR_PLUGIN_H
#define LIMBO_AI_EDITOR_PLUGIN_H
#ifdef TOOLS_ENABLED

#include "../bt/behavior_tree.h"
#include "core/object.h"
#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/tree.h"

class TaskTree : public Control {
	GDCLASS(TaskTree, Control);

private:
	Tree *tree;
	Ref<BehaviorTree> bt;
	Ref<BTTask> last_selected;

	TreeItem *_create_tree(const Ref<BTTask> &p_task, TreeItem *p_parent, int p_idx = -1);
	void _update_item(TreeItem *p_item);
	void _update_tree();
	TreeItem *_find_item(const Ref<BTTask> &p_task) const;

	void _on_item_selected();
	void _on_item_rmb_selected(const Vector2 &p_pos);

protected:
	static void _bind_methods();

public:
	void load_bt(const Ref<BehaviorTree> &p_behavior_tree);
	Ref<BehaviorTree> get_bt() const { return bt; }
	void update_tree() { _update_tree(); }
	void update_task(const Ref<BTTask> &p_task);
	Ref<BTTask> get_selected() const;
	void deselect();

	TaskTree();
	~TaskTree();
};

class LimboAIEditor : public Control {
	GDCLASS(LimboAIEditor, Control);

private:
	enum Action {
		ACTION_REMOVE,
		ACTION_MOVE_UP,
		ACTION_MOVE_DOWN,
		ACTION_DUPLICATE,
		ACTION_MAKE_ROOT,
	};

	EditorNode *editor;
	Button *header;
	TaskTree *task_tree;
	PopupMenu *menu;
	FileDialog *save_dialog;
	FileDialog *load_dialog;

	void _add_task(const Ref<BTTask> &p_prototype);
	void _update_header();
	void _new_bt();
	void _save_bt(String p_path);
	void _load_bt(String p_path);

	void _on_tree_rmb(const Vector2 &p_menu_pos);
	void _on_action_selected(int p_id);
	void _on_task_selected(const Ref<BTTask> &p_task) const;
	void _on_visibility_changed() const;
	void _on_header_pressed() const;
	void _on_save_pressed();

protected:
	static void _bind_methods();

public:
	LimboAIEditor(EditorNode *p_editor);
	~LimboAIEditor();
};

class LimboAIEditorPlugin : public EditorPlugin {
	GDCLASS(LimboAIEditorPlugin, EditorPlugin);

private:
	EditorNode *editor;
	LimboAIEditor *limbo_ai_editor;

protected:
	void _notification(int p_notification);

public:
	virtual String get_name() const { return "LimboAI"; }
	virtual const Ref<Texture> get_icon() const;
	bool has_main_screen() const { return true; }
	virtual void make_visible(bool p_visible);

	LimboAIEditorPlugin(EditorNode *p_editor);
	~LimboAIEditorPlugin();
};

#endif // TOOLS_ENABLED

#endif // LIMBO_AI_EDITOR_PLUGIN_H
#pragma once

struct InputState;

class ShortcutsManager
{
public:
	ShortcutsManager() = default;
	~ShortcutsManager() = default;
	ShortcutsManager(const ShortcutsManager&) = delete;
	ShortcutsManager& operator=(const ShortcutsManager&) = delete;
	void processInput(const InputState& inputState) const;
};
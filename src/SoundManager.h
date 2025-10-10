#pragma once

class SoundManager
{
public:
	SoundManager() = delete;
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	static bool initialize();
	static void loadBanks();
	static void processAudio();
	static void close();
};


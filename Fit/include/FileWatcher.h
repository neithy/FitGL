#pragma once

#include <FitClasses.h>
#include <thread>
#include <vector>

class fit::FileWatcher {
public:
	FileWatcher();
	virtual ~FileWatcher();

	void run();
	void stop();
	void addDir(std::string fileName);

	void setCallback(std::function<void()> callback) {
		this->callback = callback;
	}

protected:
	void loop();
	std::function<void()> callback;
	std::vector<std::string> dirs;

	std::unique_ptr<std::thread> thread;
	bool stopFlag = false;
	bool running = false;
	
	void* interruptEvent;
};

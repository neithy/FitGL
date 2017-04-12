#pragma once

#include "FileWatcher.h"
#include <filesystem>
#include <iostream>
#include <chrono>

#ifdef WIN32
#include <Windows.h>
#endif

fit::FileWatcher::FileWatcher(){
	interruptEvent = CreateEvent(0, 0, 0, "interrupt");
}

fit::FileWatcher::~FileWatcher(){
	if (running)stop();
}

void fit::FileWatcher::run(){
	if (running)return;
	running = true;
	stopFlag = false;
	thread = std::make_unique<std::thread>(std::bind(&FileWatcher::loop, this));	
}

void fit::FileWatcher::stop(){
	if (running) {
		stopFlag = true;
		SetEvent(interruptEvent);
		thread->join();
	}
}

void fit::FileWatcher::addDir(std::string fileName){
	dirs.push_back(fileName);
	if (running) {
		stop();
		run();
	}
}

void fit::FileWatcher::loop(){
#ifdef WIN32
	
	auto file = dirs[0];
	auto handle = FindFirstChangeNotification(file.c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);
	if (handle == INVALID_HANDLE_VALUE)	{
		std::cout << "handle invalid\n";
	}
	
	while (!stopFlag) {
		std::cout << "waiting... \n";
		void* handles[] = { handle, interruptEvent };
		auto wait = WaitForMultipleObjects(2, handles, false, INFINITE);
		FindNextChangeNotification(handle);
		std::cout << wait << "\n";

		std::cout << " thread out\n";
	}

#endif
	running = false;
}

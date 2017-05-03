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
  std::vector<void*> handles;
  handles.push_back(interruptEvent);
  for (auto& dir : dirs) {
    auto handle = FindFirstChangeNotification(dir.c_str(), false, FILE_NOTIFY_CHANGE_LAST_WRITE);
    
    if (handle == INVALID_HANDLE_VALUE) {
      //std::cout << "handle invalid\n";
    } else {
      handles.push_back(handle);
    }
  }
	
	while (!stopFlag) {
		auto wait = WaitForMultipleObjects(handles.size(), handles.data(), false, INFINITE);
    //std::cout << "wait " << wait << "\n";
    for (auto &h : handles)FindNextChangeNotification(h);
    if (wait > WAIT_OBJECT_0 && wait <= WAIT_OBJECT_0 + handles.size()) {
      if (callback)callback();
      //std::cout << "change, callback\n";
    }
	}

#endif
	running = false;
}

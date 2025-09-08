#ifndef FLAGS_H
#define FLAGS_H

#include <functional>
#include <iostream>
#include <string>

struct Settings{
	bool rawText {false};
	bool objects {false};
	bool noWrite {false};
	bool error {false};
	std::string fileName;
};

typedef std::function<void(Settings&)> NoArgHandle;

const std::unordered_map<std::string, NoArgHandle> NoArgs{
	{"-r", [](Settings& settings) {settings.rawText = true; }},
		{"--raw", [](Settings& settings) {settings.rawText = true; }},
		{"-q", [](Settings& settings) {settings.objects = true; }},
		{"--questions", [](Settings& settings) {settings.objects = true; }},
		{"-n", [](Settings& settings) {settings.noWrite = true; }},
		{"--nowrite", [](Settings& settings) {settings.noWrite = true; }},
};

Settings getFlags(int argc, char** argv);

#endif

#include "flags.h"

Settings getFlags(int argc, char** argv){
	Settings output;
	
	std::string opt;
	for(int i = 1; i < argc; i++){
		opt = argv[i];	
		if(auto j {NoArgs.find(opt)}; j != NoArgs.end()){
			j->second(output);
		}
		else if(output.fileName.empty()) output.fileName = argv[i];
		else std::cerr << "invalid parameter" << std::endl;
	}

	return output;
};

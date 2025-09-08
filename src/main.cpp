#include <iostream>
#include <filesystem>

#include "helper.h"
#include "question.h"
#include "readQuestion.h"
#include "writeQuestion.h"
#include "flags.h"



int main(int argc, char **argv){

	Settings settings = getFlags(argc, argv);

	if(argc < 2){
		std::cout << "Error: provide Filename" << std::endl;
		return 1;
	};


	std::vector<Question> questions;
	if(!settings.fileName.empty()){
		questions = getQuestions(settings.fileName, settings);
	}
	else{
		std::cerr << "could not read fileName" << std::endl;
		return 1;
	}
	
	if(settings.rawText && !settings.error){
		std::cout << "\n\nrawText:" << std::endl;
		std::vector<std::string> rawText = rawQuestionText(readInput(settings.fileName)); 
		printV(rawText);
	}


	if(settings.objects && !settings.error){
		std::cout << "\nQuestion Objects:" << std::endl;
		for(int i = 0; i < questions.size(); i++){
			std::cout << std::endl;
			std::cout << questions[i] << std::endl;
			questions[i].printAnswers();
			questions[i].printPoints();
		};
	}

	if(std::filesystem::exists(settings.fileName) && !settings.noWrite && !settings.error){
		writeQuestions(questions, settings.fileName);
	}
	else if(settings.noWrite && !settings.error){}
	else if(settings.error){
		std::cerr << "error detected - no output generated" << std::endl;
	}
	else{
		std::cerr << "could not find file \"" << settings.fileName << "\"" << std::endl;
	}

	return 0;
}

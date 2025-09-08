#include "helper.h"
#include "readQuestion.h"



void eraseWS(std::string& str){
	//function to remove leading whitespace
	while(str.front() == ' ') str.erase(0,1);
};

void eraseTWS(std::string& str){
	//function to remove trailling whilespace
	while(str.back() == ' ') str.pop_back();
};
	
void replaceEscape(std::string& str){
	//replaces html escape charaters (ILIAS crashes otherwise)
	for(int i = 0; i < str.length(); i++){
		switch (str.at(i)){
			case '<':
				str.replace(i,1,"&lt;");
				break;
			case '>':
				str.replace(i,1,"&gt;");
				break;
			case '&':
				str.replace(i,1,"&amp;");
				break;
			case '\"':
				str.replace(i,1,"&quot;");
				break;
		}
	}	
};

std::string readInput(std::string fileName){
	//get raw text from file

	std::string output;
	char buffer = {};
	std::ifstream readFile;
	readFile.open(fileName);

	if(readFile.is_open()){
		while(readFile.get(buffer)){
			output.push_back(buffer);
		};
	readFile.close();
	}
	else {
		std::cerr << "error reading file" << std::endl;
	}

       	
	return output;
};

int countQuestions(std::string rawText){
	//get total number of questions

	int count = 0;
	int position = 0;
	while ((position = rawText.find("[q]", position)) != std::string::npos){
		//find next occurence after the position and add to counter
		position++;
		count++;
	};
	return count;
};

int getQuestionType(std::string question){
	//get question type as defined in question.h
	// question type -1 indicates an error

	eraseWS(question);
	int questionId = -1;
	question.erase(0,3);
	//remove "[q]"

	eraseWS(question);

	switch (question[1]){
		//relevant char is at position 1 t.e. [s]
		case 's':
			questionId = SINGLE_CHOICE;
			break;
		case 'm':
			questionId = MULTIPLE_CHOICE;
			break;
		case 'o':
			questionId = OPEN_QUESTION;
			break;
		case 'g':
			questionId = GAP_QUESTION;
			break;
				
	};

	return questionId;
};

std::vector<std::string> rawQuestionText(std::string rawText){
	//seperate questions into a vector

	int numQuestions = countQuestions(rawText); 
	std::vector<std::string> output;
	int positions[numQuestions];
	int position = 0;	

	for(int i = 0; i < rawText.length(); i++){
		if(rawText.substr(i,3) == "[q]"){
			positions[position] = i;
			position++;
		};
	};
	//fill positions with indices of "[q]"

	for(int i = 0; i < numQuestions; i++){
		if(i == numQuestions - 1){
			output.push_back(rawText.substr(positions[i], rawText.length()));
		}
		else{
			output.push_back(rawText.substr(positions[i], positions[i+1] - positions[i]));
		}
	}
	//sort parts into output vector

	for(int i = 0; i < output.size(); i++){
		while(output[i].back() == ' ') output[i].pop_back();
		while(output[i].back() == '\n') output[i].pop_back();
	};
	//erase white space at the end of all questions
	
	return output;
};


std::string getQuestionText(std::string questionText){
	//extracts the question text from raw question texts
	std::string output;
	int startOfText;
	int endOfText;
	if(questionText.find('\n') != std::string::npos) startOfText = questionText.find('\n');
		else std::cerr << "could not find '\n' in question text" << std::endl;
	if(questionText.find("\n\n") != std::string::npos) endOfText = questionText.find("\n\n");
		else std::cerr << "could not find \"\n\n\" in question text" << std::endl;
	
	output = questionText.substr(startOfText,endOfText - startOfText);

	eraseWS(questionText);

	if(output[0] == '\n') output.erase(0,1);
	//erase new line char

	eraseTWS(output);
	//erase white space at the end just to be sure

	return output;
};

std::string getQuestionTextOpen(std::string questionText){
	//extracts question text for open questions (no following text)
	std::string output;

	int startOfText;
       	if(questionText.find('\n') != std::string::npos) startOfText = questionText.find('\n');
		else std::cerr << "could not find '\n' in question text" << std::endl;
	output = questionText.substr(startOfText); 

	eraseWS(output);
	if(output[0] == '\n') output.erase(0,1);
	//erase new line char
	eraseTWS(output);
	//erase white space at the end just to be sure


	return output;
};


std::string getQuestionName(std::string questionText){
	//extracts the question name from raw question texts
	std::string output;

	eraseWS(questionText);
	questionText.erase(0,3);
	//erase "[q]"
	eraseWS(questionText);

	questionText.erase(0,3);
	//erase question type
	eraseWS(questionText);

	if(questionText[0] == '['){
		questionText.erase(0,questionText.find(']') + 1);
	};
	//erase optional point multiplier

	int endOfText;
       	if(questionText.find('\n') != std::string::npos) endOfText = questionText.find('\n');
		else std::cerr << "could not find '\n' after question name" << std::endl;

	output = questionText.substr(0,endOfText);

	eraseWS(questionText);
	
	if(output[0] == '\n') output.erase(0,1);
	//erase new line char if there
	//after question name implementation no longer nessessary

	eraseTWS(output);
	//erase white space at the end just to be sure

	return output;
};

std::vector<std::string> getAnswers(std::string questionText){
	//sort answers into vector
	//seperation by new line char
	std::vector<std::string> output;
	int endOfText;
       	if(questionText.find("\n\n\n") != std::string::npos) endOfText = questionText.find("\n\n\n");
		else{
		       	std::cerr << "could not find \"\n\n\" in question text" << std::endl;
			return output;
		}
	questionText.erase(0,endOfText + 3);
	// 3x '\n' because of the new line at the end of the question text
	//erase question text plus new line chars

	std::istringstream inputStream(questionText);
	for(std::string buffer; std::getline(inputStream, buffer);){
		output.push_back(buffer);
	};
	//sort answers into vector

	for(int i = 0; i < output.size(); i++){
		eraseWS(output[i]);
		output[i].erase(0,1);
		eraseWS(output[i]);
		eraseTWS(output[i]);
	};
	//erase leading and trailling whitespace and _/-
	
	for(int i = 0; i < output.size(); i++){
		if(!output[i].length()){
		       output.erase(output.begin() + i);
		};
	};

	for(int i = 0; i < output.size(); i++){
		if(output.back().empty()){
		       output.pop_back();
		};
	};
	
	//erase possible lines with new line chars	

	return output;
};

std::vector<std::string> getGapText(std::string questionText){
	std::vector<std::string> output;
	std::vector<int> positions;
	positions.push_back(0);

	int endOfText;
       	if(questionText.find("\n\n\n") != std::string::npos) endOfText = questionText.find("\n\n\n");
		else {
			std::cerr << "could not find \"\n\n\" in the question text" << std::endl;
			return output;
		}
	questionText.erase(0,endOfText + 3);
	//erase question text plus new line chars

	for(int i = 0; i < questionText.length(); i++){
		if((questionText.substr(i,5) == "[gap]") || (questionText.substr(i,6) == "[/gap]")){
			positions.push_back(i);
		};
	};
	//fill positions with indices of "[gap]" or "[/gap]"

	positions.push_back(questionText.length() - 1);
	if(positions[1] == 0) positions.erase(positions.begin());
	
	for(int i = 0; i < positions.size() - 1; i++){
		output.push_back(questionText.substr(positions[i], positions[i+1] - positions[i]));
	};
	//fill output with text and gaps


	return output;
};




std::vector<double> getPointsMC(std::string questionText){
	//sort answers into vector
	//seperation by new line char
	std::vector<double> output;

	double multiplier = 1.0;
	eraseWS(questionText);
	questionText.erase(0,3);
	//erase "[q]"
	eraseWS(questionText);

	questionText.erase(0,3);
	//erase question type
	eraseWS(questionText);

	if(questionText[0] == '['){
		multiplier = stoi(questionText.substr(1,questionText.find(']')));
	};
	//get optional multiplier


	int endOfText;
       	if(questionText.find("\n\n\n") != std::string::npos) endOfText = questionText.find("\n\n\n");
		else {
			std::cerr << "could not find \"\n\n\" in the question text" << std::endl;
			return output;
		}
	questionText.erase(0,endOfText + 3);
	//erase question text plus new line chars

	std::vector<std::string> answers;
	std::istringstream inputStream(questionText);
	for(std::string buffer; std::getline(inputStream, buffer);){
		answers.push_back(buffer);
	};
	//sort answers into vector

	for(int i = 0; i < answers.size(); i++){
		eraseWS(answers[i]);
	};
	//erase leading whitespace

	for(int i = 0; i < answers.size(); i++){
		if(answers[i].empty()){
		       answers.erase(answers.begin() + i);
		};
	};
	//erase empty lines
	
	for(auto answer : answers){
		switch (answer[0]){
			case '_':
				output.push_back(1.0 * multiplier);
				break;
			case '-':
				output.push_back(-1.0 * multiplier);
				break;
		};
	};
	//get right and wrong answers


	int numAnswers = output.size();
	double sum = 0;
	for(int i = 0; i < numAnswers - 1; i++){
		output[i] = output[i]/numAnswers;
		output[i] = trunc(output[i] * 100)/100;
		//round 2 decimal places
		sum += std::abs(output[i]);
		//get sum of points of all but the last element
	};

	output[output.size() - 1] = (multiplier - sum) * output[output.size() - 1]/std::abs(output[output.size() - 1]);
/*	
	for(int i = 0; i < output.size(); i++){
		output[i] *= multiplier;
	};
	//multiply points by desired maximum points
	*/

	return output;
};


std::vector<double> getPointsSC(std::string questionText){
	//sort answers into vector
	//seperation by new line char
	std::vector<double> output;

	int multiplier = 1;
	eraseWS(questionText);
	questionText.erase(0,3);
	//erase "[q]"
	eraseWS(questionText);

	questionText.erase(0,3);
	//erase question type
	eraseWS(questionText);

	if(questionText[0] == '['){
		multiplier = stoi(questionText.substr(1,questionText.find(']')));
	};
	//get optional multiplier


	int endOfText;
       	if(questionText.find("\n\n\n") != std::string::npos) endOfText = questionText.find("\n\n\n");
		else {
			std::cerr << "could not find \"\n\n\" in the question text" << std::endl;
			return output;
		}
	questionText.erase(0,endOfText + 3);
	//erase question text plus new line chars

	std::vector<std::string> answers;
	std::istringstream inputStream(questionText);
	for(std::string buffer; std::getline(inputStream, buffer);){
		answers.push_back(buffer);
	};
	//sort answers into vector

	for(int i = 0; i < answers.size(); i++){
		eraseWS(answers[i]);
	};
	//erase leading whitespace

	for(int i = 0; i < answers.size(); i++){
		if(answers[i].empty()){
		       answers.erase(answers.begin() + i);
		};
	};
	//erase possible lines with new line chars	
	
	for(auto answer : answers){
		switch (answer[0]){
			case '_':
				output.push_back(1);
				break;
			case '-':
				output.push_back(0);
				break;
		};
	};
	//get right and wrong answers

	for(int i = 0; i < output.size(); i++){
		output[i] *= multiplier;
	};
	//multiply points by desired maximum points

	return output;
};


std::vector<double> getPointsOpen(std::string questionText){
	std::vector<double> output;

	int multiplier = 1;
	eraseWS(questionText);
	questionText.erase(0,3);
	//erase "[q]"
	eraseWS(questionText);

	questionText.erase(0,3);
	//erase question type
	eraseWS(questionText);

	if(questionText[0] == '['){
		multiplier = stoi(questionText.substr(1,questionText.find(']')));
	};
	//get optional multiplier

	output.push_back(multiplier);

	return output;
};

std::vector<double> getPointsGap(std::string questionText){
	std::vector<double> output;

	int numGaps = 0;
	for(int i = 0; i < questionText.length() - 5; i++){
		if(questionText.substr(i,5) == "[gap]") numGaps++;
	};

	int multiplier = 1;
	eraseWS(questionText);
	questionText.erase(0,3);
	//erase "[q]"
	eraseWS(questionText);

	questionText.erase(0,3);
	//erase question type
	eraseWS(questionText);

	if(questionText[0] == '['){
		multiplier = stoi(questionText.substr(1,questionText.find(']')));
	};
	//get optional multiplier

	double sum = 0;
	for(int i = 0; i < numGaps - 1; i++){
		output.push_back(trunc(100/numGaps)/100);
		//round 2 decimal places
		sum += trunc(100/numGaps)/100;
		//get sum of points of all but the last element
	};

	output.push_back(1.0 - sum);
	
	for(int i = 0; i < output.size(); i++){
		output[i] *= multiplier;
	};
	//multiply points by desired maximum points

	return output;
};

std::vector<Question> getQuestions(std::string fileName, Settings& settings){
	std::vector<Question> output;

	std::string rawText = readInput(fileName);
	replaceEscape(rawText);
	//replace html escape charaters
	std::vector<std::string> rawQuestions = rawQuestionText(rawText);
		
	for(int i = 0; i < rawQuestions.size(); i++){
		std::string qName = getQuestionName(rawQuestions[i]);
		if(qName.empty()){
			std::cerr << "no question name found in question " << i+1 << std::endl;
			std::cerr << "~~~~~~~~~" << std::endl;
			std::cerr << rawQuestions[i] << std::endl;
			std::cerr << "~~~~~~~~~" << std::endl;
			settings.error = true;
			continue;
		}

		int qType = getQuestionType(rawQuestions[i]);
		if(qType == -1){
			std::cerr << "question type error in question " << i+1 << std::endl;
			std::cerr << "~~~~~~~~~" << std::endl;
			std::cerr << rawQuestions[i] << std::endl;
			std::cerr << "~~~~~~~~~" << std::endl;
			settings.error = true;
			continue;
		}

		std::string qText;
		std::vector<std::string> qAnswers;
		std::vector<double> qPoints;
		std::vector<std::string> qGapText;
		switch (qType){
			case SINGLE_CHOICE:
				qText = getQuestionText(rawQuestions[i]);
				if(qText.empty()){
					std::cerr << "question text missing in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qAnswers = getAnswers(rawQuestions[i]);
				if(qAnswers.empty()){
					std::cerr << "answers missing in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qPoints = getPointsSC(rawQuestions[i]);
				if(qPoints.empty()){
					std::cerr << "points error in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				output.push_back(Question(qName, qType, qText, qAnswers, qPoints));
				break;

			case MULTIPLE_CHOICE:
				qText = getQuestionText(rawQuestions[i]);
				if(qText.empty()){
					std::cerr << "question text missing in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qAnswers = getAnswers(rawQuestions[i]);
				if(qAnswers.empty()){
					std::cerr << "answers missing in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qPoints = getPointsMC(rawQuestions[i]);
				if(qPoints.empty()){
					std::cerr << "points error in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				output.push_back(Question(qName, qType, qText, qAnswers, qPoints));
				break;
			case OPEN_QUESTION:
				qText = getQuestionTextOpen(rawQuestions[i]);
				if(qText.empty()){
					std::cerr << "question text missing in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qPoints = getPointsOpen(rawQuestions[i]);
				output.push_back(Question(qName, qType, qText, qPoints));
				break;
			case GAP_QUESTION:
				qText = getQuestionText(rawQuestions[i]);
				if(qText.empty()){
					std::cerr << "question text missing in question " << i+1 << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qGapText = getGapText(rawQuestions[i]);
				if(qGapText.empty()){
					std::cerr << "gap text missing in question " << i+1<< std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					std::cerr << rawQuestions[i] << std::endl;
					std::cerr << "~~~~~~~~~" << std::endl;
					settings.error = true;
					continue;
				}
				qPoints = getPointsGap(rawQuestions[i]);
				output.push_back(Question(qName, qType, qText, qGapText, qPoints));
				break;
		};
	};

	return output;
};







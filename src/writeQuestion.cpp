#include "question.h"
#include "writeQuestion.h"

std::string getMetaData(Question& question){
    std::string output;
    std::string type;
    switch (question.type()){
        case SINGLE_CHOICE:
            type = "SINGLE CHOICE QUESTION";
            break;
        case MULTIPLE_CHOICE:
            type = "MULTIPLE CHOICE QUESTION";
            break;
        case OPEN_QUESTION:
            type = "OPEN QUESTION";
            break;
        case GAP_QUESTION:
            type = "GAP QUESTION";
            break;
    };

    output = std::format("<itemmetadata><qtimetadata><qtimetadatafield><fieldlabel>QUESTIONTYPE</fieldlabel><fieldentry>{}"
        "</fieldentry></qtimetadatafield></qtimetadata></itemmetadata>", type);

    return output;
};

std::string answerPresentation(std::string answer, int numAnswer){
    //gets answers in <Presentation> as items (easier for loop later)

    std::string output = std::format("<response_label ident=\"{}\"><material><mattext texttype=\"text/plain\">{}"
        "</mattext></material></response_label>", numAnswer, answer);

    return output;
};

std::string answerPointsSC(double points, int numAnswer){
    //gets processing items in <resprocessing> (easier for loop later)

    std::string output = std::format("<respcondition continue=\"Yes\"><conditionvar><varequal respident=\"MCSR\">{}"
        "</varequal></conditionvar><setvar action=\"Add\">{}</setvar></respcondition>",numAnswer,points);

    return output;
};

std::string answerPointsMC(double points, int numAnswer){
    //gets processing items in <resprocessing> (easier for loop later)

    std::string output = std::format("<respcondition continue=\"Yes\"><conditionvar><varequal respident=\"MCMR\">{}"
        "</varequal></conditionvar><setvar action=\"Add\">{}"
        "</setvar></respcondition><respcondition continue=\"Yes\"><conditionvar><not><varequal respident=\"MCMR\">{}"
        "</varequal></not></conditionvar><setvar action=\"Add\">{}"
        "</setvar></respcondition>", numAnswer, points, numAnswer, points * (-1.0));

    return output;
};

std::string getPresentation(Question& question){
    std::string output;
    std::vector<std::string> answers;
    switch (question.type()){
        case SINGLE_CHOICE:
            //single choice presentation
            output.append("<presentation label=\"");
            output.append(question.name());
            output.append("\"><flow><material><mattext texttype=\"text/plain\">");
            output.append(question.text());
            output.append("</mattext></material><response_lid ident=\"MCSR\" rcardinality=\"Single\"><render_choice shuffle=\"Yes\">");

            answers = question.answers();
            for(int i = 0; i < answers.size(); i++){
                output.append(answerPresentation(answers[i], i));
            };

            output.append("</render_choice></response_lid></flow></presentation>");
            break;

        case MULTIPLE_CHOICE:
            //multiple choice presentation
            output.append("<presentation label=\"");
            output.append(question.name());
            output.append("\"><flow><material><mattext texttype=\"text/plain\">");
            output.append(question.text());
            output.append("</mattext></material><response_lid ident=\"MCSR\" rcardinality=\"Multiple\"><render_choice shuffle=\"Yes\">");

            answers = question.answers();
            for(int i = 0; i < answers.size(); i++){
                output.append(answerPresentation(answers[i], i));
            };

            output.append("</render_choice></response_lid></flow></presentation>");
            break;

        case OPEN_QUESTION:
            //open question presentation
            output.append("<presentation label=\"");
            output.append(question.name());
            output.append("\"><flow><material><mattext texttype=\"text/plain\">");
            output.append(question.text());
            output.append("</mattext></material><response_str ident=\"TEXT\" rcardinality=\"Ordered\"><render_fib fibtype=\"String\" prompt=\"Box\"><response_label ident=\"A\"></response_label></render_fib></response_str></flow></presentation>");
            break;

        case GAP_QUESTION:
            //gap question presentation

            std::string buffer;
            int gapNum = 0;
            answers = question.answers();
            //answers = gapText sections!

            for(int i = 0; i < answers.size(); i++){
                if (answers[i].substr(0,5) == "[gap]"){
                    //gap presentation

                    answers[i].erase(0,5);
                    if(answers[i].find(';') == std::string::npos){
                        //only one answer
                        buffer.append(std::format("<response_str ident=\"gap_{}\" rcardinality=\"Single\">"
                        "<render_fib fibtype=\"String\" prompt=\"Box\" columns=\"12\" maxchars=\"0\">"
                        "</render_fib></response_str>", gapNum));
                        gapNum++;
                    }
                    else{
                        //multiple answers found
                        std::vector<std::string> items;
                        //vector for the individual answers
                        std::string itemPresentation;
                        //items in string form
                        
                        int next = 0;
                        while(next != std::string::npos){
                            next = answers[i].find(';');
                            items.push_back(answers[i].substr(0,next));
                            answers[i].erase(0,next + 1);
                        };
                        //get items
			for(int i = 0; i < items.size(); i++){
				while(items[i].front() == ' ') items[i].erase(0,1);
				while(items[i].back() == ' ') items[i].pop_back();
			}
			//erase leading and trailing whitespace

                        for(int j = 0; j < items.size(); j++){
                            itemPresentation.append(std::format("<response_label ident=\"{}"
                                "\"><material><mattext>{}"
                                "</mattext></material></response_label>",j, items[j]));
                        };
                        //get item presentation

                        buffer.append(std::format("<response_str ident=\"gap_{}"
                            "\" rcardinality=\"Single\"><render_choice shuffle=\"Yes\">{}"
                            "</render_choice></response_str>", gapNum, itemPresentation));
                        gapNum++;
                    };
                        

                    
                } else if (answers[i].substr(0,6) == "[/gap]"){
                    //text presentation
                    answers[i].erase(0,6);
                    buffer.append(std::format("<material><mattext texttype=\"text/plain\">{}</mattext></material>", answers[i]));
                } else{
                    //text presentation
                    buffer.append(std::format("<material><mattext texttype=\"text/plain\">{}</mattext></material>", answers[i]));
                };
            };

            output = std::format("<presentation label=\"{}"
		"\"><flow><material>"
                "<mattext texttype=\"text/plain\">{}"
                "</mattext></material>{}"
                "<material><mattext texttype=\"text/plain\"></mattext></material></flow></presentation>",question.name(), question.text(), buffer);

            break;
            
    };
    return output;
};

std::string getResprocessing(Question& question){
    std::string output;
    std::vector<std::string> answers;
    std::vector<double> points;
        switch (question.type()){
            case SINGLE_CHOICE:
                output = "<resprocessing><outcomes><decvar></decvar></outcomes>";

                answers = question.answers();
                points = question.points();
                for (int i = 0; i < answers.size(); i++){
                    output.append(answerPointsSC(points[i], i));
                };

                output.append("</resprocessing>");
                break;
            case MULTIPLE_CHOICE:
                output = "<resprocessing><outcomes><decvar></decvar></outcomes>";

                answers = question.answers();
                points = question.points();
                for (int i = 0; i < answers.size(); i++){
                    output.append(answerPointsMC(points[i], i));
                };

                output.append("</resprocessing>");
                break;
            case OPEN_QUESTION:
                output = std::format("<resprocessing scoremodel=\"HumanRater\"><outcomes>"
                    "<decvar varname=\"WritingScore\" vartype=\"Integer\" minvalue=\"0\" maxvalue=\"{}"
                "\"></decvar></outcomes><respcondition continue=\"Yes\"><conditionvar><varequal respident=\"points\">1"
                "</varequal></conditionvar><displayfeedback feedbacktype=\"Response\" linkrefid=\"response_allcorrect\" />"
                "</respcondition><respcondition continue=\"Yes\"><conditionvar><not><varequal respident=\"points\">1</varequal>"
                "</not></conditionvar><displayfeedback feedbacktype=\"Response\" linkrefid=\"response_onenotcorrect\" />"
                "</respcondition><respcondition><conditionvar><other>tutor_rated</other></conditionvar></respcondition></resprocessing>", question.points()[0]);
                break;
            case GAP_QUESTION:
                //resprocessing gap question
                std::string buffer;
                int gapNum = 0;
                answers = question.answers();
                points = question.points();
                //answers = gapText sections!
                std::vector<std::string> items;
                //vector for the individual answers
                

                for(int i = 0; i < answers.size(); i++){
                    items.clear();
                    //clear previous items 

                    if(answers[i].substr(0,5) == "[gap]"){
                        answers[i].erase(0,5);
                        //erase "[gap]" at the beginning
                        

                      if(answers[i].find(';') != std::string::npos){
                        //multiple answers
                        
                        int next = 0;
                        while(next != std::string::npos){
                            next = answers[i].find(';');
                            items.push_back(answers[i].substr(0,next));
                            answers[i].erase(0,next + 1);
                        };
                        //get items
                        
                        
                        buffer.append(std::format("<respcondition continue=\"Yes\"><conditionvar><varequal respident=\"gap_{}"
                            "\">{}"
                            "</varequal></conditionvar><setvar action=\"Add\">{}"
                            "</setvar></respcondition>", gapNum, items[0], points[gapNum]));
                        //set correct answer (first in items)

                        for(int j = 1; j < items.size(); j++){
                            buffer.append(std::format("<respcondition continue=\"Yes\"><conditionvar><varequal respident=\"gap_{}"
                            "\">{}"
                            "</varequal></conditionvar><setvar action=\"Add\">0"
                            "</setvar></respcondition>", gapNum, items[j]));
                            
                        };
                        //incorrect answers (all but the first item)

                        gapNum++;
                        

                      } else {
                        //only one answer
                        buffer.append(std::format("<respcondition continue=\"Yes\"><conditionvar><varequal respident=\"gap_{}"
                            "\">{}"
                            "</varequal></conditionvar><setvar action=\"Add\">{}"
                            "</setvar></respcondition>", gapNum, answers[i], points[i]));
                        gapNum++;
                      };
                    };
                };
                output = std::format("<resprocessing><outcomes><decvar></decvar></outcomes>{}"
                    "</resprocessing>", buffer);
                
                break;
        };

    return output;
};


std::string getQuestionString(std::vector<Question>& questions){
    std::string output;
    std::string buffer;
    output.append(HEADER);
    output.append(BEGIN);

    for(int i = 0; i < questions.size(); i++){
        
       buffer = std::format("<item ident=\"il_0_qst_00000\" title=\"{}"
           "\" maxattempts=\"0\"><qticomment></qticomment><duration>P0Y0M0DT0H1M0S</duration>{}{}{}</item>"
            ,questions[i].name(), getMetaData(questions[i]), getPresentation(questions[i]), getResprocessing(questions[i]));
        
        output.append(buffer);
        
    };

    output.append(END);

    return output;
};

void writeQuestions(std::vector<Question>& questions, const std::string& fileName){
    std::string output = getQuestionString(questions);
    std::string name = fileName;
    name.append(".qti.xml");

    if(std::filesystem::exists(name)){
        std::cout << "fileName already in use" << std::endl;
    }
    else{
        std::ofstream outputFile;
        outputFile.open(name);
        if(outputFile.is_open()){
            outputFile << output;
            outputFile.close();
        }
        else{
            std::cout << "Error: unable to open file" << std::endl;
        };
    }
};

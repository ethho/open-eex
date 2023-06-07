#include "messages.hpp"

void tokenize(std::vector<std::string>& tokens, std::string command){
    //raw form of delimiter
    std::regex re(R"(\r\n)");

    //i don't like iterators
    std::sregex_token_iterator start(command.begin(),command.end(), re,-1);
    std::sregex_token_iterator end = std::sregex_token_iterator();
    //copy the elements of the regex iterator into the vector
    std::sregex_token_iterator it;
    for(it = start; it != end; it++){
        tokens.push_back(it->str());
    }

    // Additional check to remove empty strings
    tokens.erase(std::remove_if(tokens.begin(),tokens.end(), [](std::string const& s) {return s.size() == 0;}),tokens.end());
 
    return;
}

std::vector<std::string> generate_tokens(std::string command){
    std::vector<std::string> tokens;
    if(command.size() > 0){
        tokenize(tokens,command);
    }
    return tokens;
}

//
//  main.cpp
//  FormalLanguagesPrac1Task12
//
//  Created by Сергей Миллер on 19.10.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <iostream>
#include <string>
#include "RPNparser.h"

int main(int argc, const char * argv[]) {
    // aba.*.* acbaaba
    string regExp = arg[1];
    string word = arg[2];
    int ans;
    try {
        ans = findSizeOfMaxWordSuffixInRegExp(regExp, word);
        if(ans == -1) {
            std::cout << "INF" << std::endl;
        } else {
            std::cout << ans << std:: endl;
        }
    } catch (BadRegExpInRPNException e) {
        std::cerr << "Error: regular expression \"" << regExp << "\" is incorrect" << std::endl;
    }
    return 0;
}

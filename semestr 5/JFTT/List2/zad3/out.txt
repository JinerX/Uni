#include <iostream>



/**
 * 
 * DOXYGEN COMMENT
 * 
 */



//! Doxygen single line comment
int main() {
    std::cout << "//!Doxygen comment inside string should not be detected" << std::endl; 
    std::cout << "eloelo\\
                  320\\
                  coscos";
    return 0;

}
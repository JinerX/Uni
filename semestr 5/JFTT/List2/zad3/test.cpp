#include <iostream>

// standard comment 1

/**
 * 
 * DOXYGEN COMMENT
 * 
 */

/*

Standard comment multine
//! docygen comment within standard multiline
*/

//! Doxygen single line comment
int main() {
    std::cout << "//!Doxygen comment inside string should not be detected" << std::endl; // comment in the same line as code
    std::cout << "eloelo\\
                  320\\
                  coscos";
    return 0;

}
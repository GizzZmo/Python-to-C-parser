#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

#endif // TOKEN_H
```



    return 0;
}
```

//**This refactored code separates concerns into different modules, enhances readability, and improves error handling.

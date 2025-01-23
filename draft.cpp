
//**Dear Python Developers Community
//**Let's dive deeper into each part of the analysis and the implementation of a menu along with code generation.
//**### Step 1: Lexical Analysis (Tokenization)
//**Lexical analysis involves breaking the input code into tokens, which are the smallest units of meaning (e.g., keywords, operators, identifiers). 
//**Here's an expanded implementation:
//**```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};
struct Token {
    TokenType type;
    std::string value;
};
std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::regex token_regex(R"((\bdef\b|\bprint\b|\w+|[0-9]+|".*?"|\s+|[^\w\s]+))");
    auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto tokens_end = std::sregex_iterator();
    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
        std::smatch match = *i;
        std::string token_str = match.str();
        TokenType type;
        if (std::regex_match(token_str, std::regex(R"(\bdef\b|\bprint\b)"))) {
            type = KEYWORD;
        } else if (std::regex_match(token_str, std::regex(R"(\w+)"))) {
            type = IDENTIFIER;
        } else if (std::regex_match(token_str, std::regex(R"([0-9]+)"))) {
            type = NUMBER;
        } else if (std::regex_match(token_str, std::regex(R"(".*?")"))) {
            type = STRING;
        } else if (std::regex_match(token_str, std::regex(R"(\s+)"))) {
            type = WHITESPACE;
        } else {
            type = OPERATOR;
        }
        tokens.push_back({type, token_str});
    }
    return tokens;
}
int main() {
    std::ifstream file("example.py");
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<Token> tokens = tokenize(code);
    for (const auto& token : tokens) {
        std::cout << "Token(" << token.value << ", Type: " << token.type << ")" << std::endl;
    }
    return 0;
}
```
### Step 2: Syntax Analysis (Parsing)
This step involves structuring tokens into a syntax tree. Here’s how you can parse tokens and create a basic syntax tree:
```cpp
#include <iostream>
#include <vector>
#include <string>
struct Node {
    std::string value;
    std::vector<Node> children;
};
Node parse(const std::vector<Token>& tokens) {
    Node root;
    Node current;
    for (const auto& token : tokens) {
        if (token.type == KEYWORD) {
            if (current.value.empty()) {
                current.value = token.value;
            } else {
                root.children.push_back(current);
                current = Node{token.value, {}};
            }
        } else {
            current.children.push_back(Node{token.value, {}});
        }
    }
    if (!current.value.empty()) {
        root.children.push_back(current);
    }
    return root;
}
void printTree(const Node& node, int depth = 0) {
    std::cout << std::string(depth, ' ') << node.value << std::endl;
    for (const auto& child : node.children) {
        printTree(child, depth + 2);
    }
}
int main() {
    std::vector<Token> tokens = {
        {KEYWORD, "def"}, {IDENTIFIER, "main"}, {OPERATOR, "("}, {OPERATOR, ")"}, 
        {OPERATOR, ":"}, {KEYWORD, "print"}, {OPERATOR, "("}, {STRING, "\"Hello\""}, {OPERATOR, ")"}
    };
    Node syntaxTree = parse(tokens);
    printTree(syntaxTree);
    return 0;
}
```
//**### Step 3: Semantic Analysis
//**Semantic analysis checks if the parsed code is logically correct. Here's a simplified example:
//**```cpp
#include <iostream>
#include <string>
bool checkSemantics(const Node& node) {
    for (const auto& child : node.children) {
        if (child.value == "print") {
            if (child.children.size() == 0 || child.children[0].value[0] != '"') {
                std::cerr << "Error: 'print' requires a string argument" << std::endl;
                return false;
            }
        }
    }
    return true;
}
int main() {
    Node syntaxTree = {
        "def",
        {
            {"main", {{"(", {}}, {")", {}}, {":", {}}, {"print", {{"\"Hello\"", {}}}}}}
        }
    };
    if (!checkSemantics(syntaxTree)) {
        std::cerr << "Semantic check failed" << std::endl;
        return 1;
    }
    return 0;
}
```
//**### Step 4: Code Generation
//**Finally, generate the equivalent C++ code:
//**```cpp
#include <iostream>
#include <string>
#include <vector>
std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::cout << ";
            for (const auto& grandChild : child.children) {
                code += grandChild.value;
            }
            code += " << std::endl;\n";
        }
        code += "}";
    }
    return code;
}
int main() {
    Node syntaxTree = {
        "def",
        {
            {"main", {{"(", {}}, {")", {}}, {":", {}}, {"print", {{"\"Hello\"", {}}}}}}
        }
    };
    std::string cppCode = generateCode(syntaxTree);
    std::cout << cppCode << std::endl;
    return 0;
}
```
### Menu Implementation
Here’s how you can implement a simple menu to combine all the parts:
```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};
struct Token {
    TokenType type;
    std::string value;
};
std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::regex token_regex(R"((\bdef\b|\bprint\b|\w+|[0-9]+|".*?"|\s+|[^\w\s]+))");
    auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto tokens_end = std::sregex_iterator();
    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
        std::smatch match = *i;
        std::string token_str = match.str();
        TokenType type;
        if (std::regex_match(token_str, std::regex(R"(\bdef\b|\bprint\b)"))) {
            type = KEYWORD;
        } else if (std::regex_match(token_str, std::regex(R"(\w+)"))) {
            type = IDENTIFIER;
        } else if (std::regex_match(token_str, std::regex(R"([0-9]+)"))) {
            type = NUMBER;
        } else if (std::regex_match(token_str, std::regex(R"(".*?")"))) {
            type = STRING;
        } else if (std::regex_match(token_str, std::regex(R"(\s+)"))) {
            type = WHITESPACE;
        } else {
            type = OPERATOR;
        }
        tokens.push_back({type, token_str});
    }
    return tokens;
}
struct Node {
    std::string value;
    std::vector<Node> children;
};
Node parse(const std::vector<Token>& tokens) {
    Node root;
    Node current;
    for (const auto& token : tokens) {
        if (token.type == KEYWORD) {
            if (current.value.empty()) {
                current.value = token.value;
            } else {
                root.children.push_back(current);
                current = Node{token.value, {}};
            }
        } else {
            current.children.push_back(Node{token.value, {}});
        }
    }
    if (!current.value.empty()) {
        root.children.push_back(current);
    }
    return root;
}
bool checkSemantics(const Node& node) {
    for (const auto& child : node.children) {
        if (child.value == "print") {
            if (child.children.size() == 0 || child.children[0].value[0] != '"') {
                std::cerr << "Error: 'print' requires a string argument" << std::endl;
                return false;
            }
        }
    }
    return true;
}
std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::
			
//**Let's continue building the code generation and the complete menu implementation.
//**### Expanded Code Generation
//**Expanding on the code generation part to handle a few more Python constructs and make the generated C++ code more robust.
//**```cpp
#include <iostream>
#include <string>
#include <vector>
std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::cout << ";
            for (const auto& grandChild : child.children) {
                code += grandChild.value;
            }
            code += " << std::endl;\n";
        } else if (child.value == ":") {
            code += " {\n";
        } else {
            code += child.value;
        }
    }
    code += "}\n";
    return code;
}
int main() {
    Node syntaxTree = {
        "def",
        {
            {"main", {{"(", {}}, {")", {}}, {":", {}}, {"print", {{"\"Hello\"", {}}}}}}
        }
    };
    std::string cppCode = generateCode(syntaxTree);
    std::cout << cppCode << std::endl;
    return 0;
}
```
//**### Implementing a Menu
//**Now, let's create a simple menu that allows users to load a Python file, tokenize it, parse it, check its semantics, and generate the corresponding C++ code.
//**```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};
struct Token {
    TokenType type;
    std::string value;
};
struct Node {
    std::string value;
    std::vector<Node> children;
};
std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::regex token_regex(R"((\bdef\b|\bprint\b|\w+|[0-9]+|".*?"|\s+|[^\w\s]+))");
    auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto tokens_end = std::sregex_iterator();
    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
        std::smatch match = *i;
        std::string token_str = match.str();
        TokenType type;
        if (std::regex_match(token_str, std::regex(R"(\bdef\b|\bprint\b)"))) {
            type = KEYWORD;
        } else if (std::regex_match(token_str, std::regex(R"(\w+)"))) {
            type = IDENTIFIER;
        } else if (std::regex_match(token_str, std::regex(R"([0-9]+)"))) {
            type = NUMBER;
        } else if (std::regex_match(token_str, std::regex(R"(".*?")"))) {
            type = STRING;
        } else if (std::regex_match(token_str, std::regex(R"(\s+)"))) {
            type = WHITESPACE;
        } else {
            type = OPERATOR;
        }
        tokens.push_back({type, token_str});
    }
    return tokens;
}
Node parse(const std::vector<Token>& tokens) {
    Node root;
    Node current;
    for (const auto& token : tokens) {
        if (token.type == KEYWORD) {
            if (current.value.empty()) {
                current.value = token.value;
            } else {
                root.children.push_back(current);
                current = Node{token.value, {}};
            }
        } else {
            current.children.push_back(Node{token.value, {}});
        }
    }
    if (!current.value.empty()) {
        root.children.push_back(current);
    }
    return root;
}
bool checkSemantics(const Node& node) {
    for (const auto& child : node.children) {
        if (child.value == "print") {
            if (child.children.size() == 0 || child.children[0].value[0] != '"') {
                std::cerr << "Error: 'print' requires a string argument" << std::endl;
                return false;
            }
        }
    }
    return true;
}
std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::cout << ";
            for (const auto& grandChild : child.children) {
                code += grandChild.value;
            }
            code += " << std::endl;\n";
        } else if (child.value == ":") {
            code += " {\n";
        } else {
            code += child.value;
        }
    }
    code += "}\n";
    return code;
}
void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token(" << token.value << ", Type: " << token.type << ")" << std::endl;
    }
}
void printTree(const Node& node, int depth = 0) {
    std::cout << std::string(depth, ' ') << node.value << std::endl;
    for (const auto& child : node.children) {
        printTree(child, depth + 2);
    }
}
int main() {
    std::string filename;
    std::ifstream file;
    std::string code;
    std::vector<Token> tokens;
    Node syntaxTree;
    bool exit = false;
    while (!exit) {
        std::cout << "Menu:\n";
        std::cout << "1. Load Python file\n";
        std::cout << "2. Tokenize\n";
        std::cout << "3. Parse\n";
        std::cout << "4. Check Semantics\n";
        std::cout << "5. Generate C++ Code\n";
        std::cout << "6. Exit\n";
        std::cout << "Choose an option: ";
        int option;
        std::cin >> option;
        switch (option) {
            case 1:
                std::cout << "Enter filename: ";
                std::cin >> filename;
                file.open(filename);
                if (file.is_open()) {
                    code.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    std::cout << "File loaded.\n";
                } else {
                    std::cerr << "Failed to open file.\n";
                }
                break;
            case 2:
                if (!code.empty()) {
                    tokens = tokenize(code);
                    printTokens(tokens);
                } else {
                    std::cerr << "Load a file first.\n";
                }
                break;
            case 3:
                if (!tokens.empty()) {
                    syntaxTree = parse(tokens);
                    printTree(syntaxTree);
                } else {
                    std::cerr << "Tokenize the code first.\n";
                }
                break;
            case 4:
                if (!syntaxTree.value.empty()) {
                    if (checkSemantics(syntaxTree)) {
                        std::cout << "Semantic check passed.\n";
                    }
                } else {
                    std::cerr << "Parse the code first.\n";
                }
                break;
            case 5:
                if (!syntaxTree.value.empty()) {
                    std::string cppCode = generateCode(syntaxTree);
                    std::cout << "Generated C++ Code:\n" << cppCode << std::endl;
                } else {
                    std::cerr << "Parse the code first.\n";
                }
                break;
            case 6:
                exit = true;
                break;
            default:
                std::cerr << "Invalid option.\n";
                break;
        }
    }
    return 0;
}
```
//**This implementation creates a basic menu-driven application that allows you to:
//**1. Load a Python file
//**2. Tokenize the code
//**3. Parse the tokens into a syntax tree
//**4. Check the semantics
//**5. Generate equivalent C++ code
//**You can expand and improve each step to handle more Python features and constructs as needed. Have fun coding!
//**Greetings by 
//**on Arve Ovesen
//**Notepad++
//**https://www.bing.com/search?q=notepad%20%20...

//**Dear Python Developers Community

//**Let's dive deeper into each part of the analysis and the implementation of a menu along with code generation.

//**### Step 1: Lexical Analysis (Tokenization)
//**Lexical analysis involves breaking the input code into tokens, which are the smallest units of meaning (e.g., keywords, operators, identifiers). 

//**Here's an expanded implementation:

//**```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::regex token_regex(R"((\bdef\b|\bprint\b|\w+|[0-9]+|".*?"|\s+|[^\w\s]+))");
    auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto tokens_end = std::sregex_iterator();

    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
        std::smatch match = *i;
        std::string token_str = match.str();
        TokenType type;

        if (std::regex_match(token_str, std::regex(R"(\bdef\b|\bprint\b)"))) {
            type = KEYWORD;
        } else if (std::regex_match(token_str, std::regex(R"(\w+)"))) {
            type = IDENTIFIER;
        } else if (std::regex_match(token_str, std::regex(R"([0-9]+)"))) {
            type = NUMBER;
        } else if (std::regex_match(token_str, std::regex(R"(".*?")"))) {
            type = STRING;
        } else if (std::regex_match(token_str, std::regex(R"(\s+)"))) {
            type = WHITESPACE;
        } else {
            type = OPERATOR;
        }

        tokens.push_back({type, token_str});
    }

    return tokens;
}

int main() {
    std::ifstream file("example.py");
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<Token> tokens = tokenize(code);

    for (const auto& token : tokens) {
        std::cout << "Token(" << token.value << ", Type: " << token.type << ")" << std::endl;
    }

    return 0;
}
```

//**### Step 2: Syntax Analysis (Parsing)
//**This step involves structuring tokens into a syntax tree. Here’s how you can parse tokens and create a basic syntax tree:

//**```cpp
#include <iostream>
#include <vector>
#include <string>

struct Node {
    std::string value;
    std::vector<Node> children;
};

Node parse(const std::vector<Token>& tokens) {
    Node root;
    Node current;
    for (const auto& token : tokens) {
        if (token.type == KEYWORD) {
            if (current.value.empty()) {
                current.value = token.value;
            } else {
                root.children.push_back(current);
                current = Node{token.value, {}};
            }
        } else {
            current.children.push_back(Node{token.value, {}});
        }
    }
    if (!current.value.empty()) {
        root.children.push_back(current);
    }
    return root;
}

void printTree(const Node& node, int depth = 0) {
    std::cout << std::string(depth, ' ') << node.value << std::endl;
    for (const auto& child : node.children) {
        printTree(child, depth + 2);
    }
}

int main() {
    std::vector<Token> tokens = {
        {KEYWORD, "def"}, {IDENTIFIER, "main"}, {OPERATOR, "("}, {OPERATOR, ")"}, 
        {OPERATOR, ":"}, {KEYWORD, "print"}, {OPERATOR, "("}, {STRING, "\"Hello\""}, {OPERATOR, ")"}
    };
    Node syntaxTree = parse(tokens);
    printTree(syntaxTree);
    return 0;
}
```

//**### Step 3: Semantic Analysis
//**Semantic analysis checks if the parsed code is logically correct. Here's a simplified example:

//**```cpp
#include <iostream>
#include <string>

bool checkSemantics(const Node& node) {
    for (const auto& child : node.children) {
        if (child.value == "print") {
            if (child.children.size() == 0 || child.children[0].value[0] != '"') {
                std::cerr << "Error: 'print' requires a string argument" << std::endl;
                return false;
            }
        }
    }
    return true;
}

int main() {
    Node syntaxTree = {
        "def",
        {
            {"main", {{"(", {}}, {")", {}}, {":", {}}, {"print", {{"\"Hello\"", {}}}}}}
        }
    };
    if (!checkSemantics(syntaxTree)) {
        std::cerr << "Semantic check failed" << std::endl;
        return 1;
    }
    return 0;
}
```

//**### Step 4: Code Generation
//**Finally, generate the equivalent C++ code:

//**```cpp
#include <iostream>
#include <string>
#include <vector>

std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::cout << ";
            for (const auto& grandChild : child.children) {
                code += grandChild.value;
            }
            code += " << std::endl;\n";
        }
        code += "}";
    }
    return code;
}

int main() {
    Node syntaxTree = {
        "def",
        {
            {"main", {{"(", {}}, {")", {}}, {":", {}}, {"print", {{"\"Hello\"", {}}}}}}
        }
    };
    std::string cppCode = generateCode(syntaxTree);
    std::cout << cppCode << std::endl;
    return 0;
}
```

//**### Menu Implementation
//**Here’s how you can implement a simple menu to combine all the parts:

//**```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::regex token_regex(R"((\bdef\b|\bprint\b|\w+|[0-9]+|".*?"|\s+|[^\w\s]+))");
    auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto tokens_end = std::sregex_iterator();

    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
        std::smatch match = *i;
        std::string token_str = match.str();
        TokenType type;

        if (std::regex_match(token_str, std::regex(R"(\bdef\b|\bprint\b)"))) {
            type = KEYWORD;
        } else if (std::regex_match(token_str, std::regex(R"(\w+)"))) {
            type = IDENTIFIER;
        } else if (std::regex_match(token_str, std::regex(R"([0-9]+)"))) {
            type = NUMBER;
        } else if (std::regex_match(token_str, std::regex(R"(".*?")"))) {
            type = STRING;
        } else if (std::regex_match(token_str, std::regex(R"(\s+)"))) {
            type = WHITESPACE;
        } else {
            type = OPERATOR;
        }

        tokens.push_back({type, token_str});
    }

    return tokens;
}

struct Node {
    std::string value;
    std::vector<Node> children;
};

Node parse(const std::vector<Token>& tokens) {
    Node root;
    Node current;
    for (const auto& token : tokens) {
        if (token.type == KEYWORD) {
            if (current.value.empty()) {
                current.value = token.value;
            } else {
                root.children.push_back(current);
                current = Node{token.value, {}};
            }
        } else {
            current.children.push_back(Node{token.value, {}});
        }
    }
    if (!current.value.empty()) {
        root.children.push_back(current);
    }
    return root;
}

bool checkSemantics(const Node& node) {
    for (const auto& child : node.children) {
        if (child.value == "print") {
            if (child.children.size() == 0 || child.children[0].value[0] != '"') {
                std::cerr << "Error: 'print' requires a string argument" << std::endl;
                return false;
            }
        }
    }
    return true;
}

std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::

//**Let's continue building the code generation and the complete menu implementation.

//**### Expanded Code Generation

//**Expanding on the code generation part to handle a few more Python constructs and make the generated C++ code more robust.

//**```cpp
#include <iostream>
#include <string>
#include <vector>

std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::cout << ";
            for (const auto& grandChild : child.children) {
                code += grandChild.value;
            }
            code += " << std::endl;\n";
        } else if (child.value == ":") {
            code += " {\n";
        } else {
            code += child.value;
        }
    }
    code += "}\n";
    return code;
}

int main() {
    Node syntaxTree = {
        "def",
        {
            {"main", {{"(", {}}, {")", {}}, {":", {}}, {"print", {{"\"Hello\"", {}}}}}}
        }
    };
    std::string cppCode = generateCode(syntaxTree);
    std::cout << cppCode << std::endl;
    return 0;
}
```

//**### Implementing a Menu

//**Now, let's create a simple menu that allows users to load a Python file, tokenize it, parse it, check its semantics, and generate the corresponding C++ code.

//**```cpp
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, WHITESPACE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

struct Node {
    std::string value;
    std::vector<Node> children;
};

std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;
    std::regex token_regex(R"((\bdef\b|\bprint\b|\w+|[0-9]+|".*?"|\s+|[^\w\s]+))");
    auto tokens_begin = std::sregex_iterator(code.begin(), code.end(), token_regex);
    auto tokens_end = std::sregex_iterator();

    for (std::sregex_iterator i = tokens_begin; i != tokens_end; ++i) {
        std::smatch match = *i;
        std::string token_str = match.str();
        TokenType type;

        if (std::regex_match(token_str, std::regex(R"(\bdef\b|\bprint\b)"))) {
            type = KEYWORD;
        } else if (std::regex_match(token_str, std::regex(R"(\w+)"))) {
            type = IDENTIFIER;
        } else if (std::regex_match(token_str, std::regex(R"([0-9]+)"))) {
            type = NUMBER;
        } else if (std::regex_match(token_str, std::regex(R"(".*?")"))) {
            type = STRING;
        } else if (std::regex_match(token_str, std::regex(R"(\s+)"))) {
            type = WHITESPACE;
        } else {
            type = OPERATOR;
        }

        tokens.push_back({type, token_str});
    }

    return tokens;
}

Node parse(const std::vector<Token>& tokens) {
    Node root;
    Node current;
    for (const auto& token : tokens) {
        if (token.type == KEYWORD) {
            if (current.value.empty()) {
                current.value = token.value;
            } else {
                root.children.push_back(current);
                current = Node{token.value, {}};
            }
        } else {
            current.children.push_back(Node{token.value, {}});
        }
    }
    if (!current.value.empty()) {
        root.children.push_back(current);
    }
    return root;
}

bool checkSemantics(const Node& node) {
    for (const auto& child : node.children) {
        if (child.value == "print") {
            if (child.children.size() == 0 || child.children[0].value[0] != '"') {
                std::cerr << "Error: 'print' requires a string argument" << std::endl;
                return false;
            }
        }
    }
    return true;
}

std::string generateCode(const Node& node) {
    std::string code;
    for (const auto& child : node.children) {
        if (child.value == "def") {
            code += "void " + child.children[0].value + "() {\n";
        } else if (child.value == "print") {
            code += "std::cout << ";
            for (const auto& grandChild : child.children) {
                code += grandChild.value;
            }
            code += " << std::endl;\n";
        } else if (child.value == ":") {
            code += " {\n";
        } else {
            code += child.value;
        }
    }
    code += "}\n";
    return code;
}

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token(" << token.value << ", Type: " << token.type << ")" << std::endl;
    }
}

void printTree(const Node& node, int depth = 0) {
    std::cout << std::string(depth, ' ') << node.value << std::endl;
    for (const auto& child : node.children) {
        printTree(child, depth + 2);
    }
}

int main() {
    std::string filename;
    std::ifstream file;
    std::string code;
    std::vector<Token> tokens;
    Node syntaxTree;
    bool exit = false;

    while (!exit) {
        std::cout << "Menu:\n";
        std::cout << "1. Load Python file\n";
        std::cout << "2. Tokenize\n";
        std::cout << "3. Parse\n";
        std::cout << "4. Check Semantics\n";
        std::cout << "5. Generate C++ Code\n";
        std::cout << "6. Exit\n";
        std::cout << "Choose an option: ";
        int option;
        std::cin >> option;

        switch (option) {
            case 1:
                std::cout << "Enter filename: ";
                std::cin >> filename;
                file.open(filename);
                if (file.is_open()) {
                    code.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();
                    std::cout << "File loaded.\n";
                } else {
                    std::cerr << "Failed to open file.\n";
                }
                break;

            case 2:
                if (!code.empty()) {
                    tokens = tokenize(code);
                    printTokens(tokens);
                } else {
                    std::cerr << "Load a file first.\n";
                }
                break;

            case 3:
                if (!tokens.empty()) {
                    syntaxTree = parse(tokens);
                    printTree(syntaxTree);
                } else {
                    std::cerr << "Tokenize the code first.\n";
                }
                break;

            case 4:
                if (!syntaxTree.value.empty()) {
                    if (checkSemantics(syntaxTree)) {
                        std::cout << "Semantic check passed.\n";
                    }
                } else {
                    std::cerr << "Parse the code first.\n";
                }
                break;

            case 5:
                if (!syntaxTree.value.empty()) {
                    std::string cppCode = generateCode(syntaxTree);
                    std::cout << "Generated C++ Code:\n" << cppCode << std::endl;
                } else {
                    std::cerr << "Parse the code first.\n";
                }
                break;

            case 6:
                exit = true;
                break;

            default:
                std::cerr << "Invalid option.\n";
                break;
        }
    }

    return 0;
}
```

//**This implementation creates a basic menu-driven application that allows you to:

//**1. Load a Python file
//**2. Tokenize the code
//**3. Parse the tokens into a syntax tree
//**4. Check the semantics
//**5. Generate equivalent C++ code

//**You can expand and improve each step to handle more Python features and constructs as needed. Have fun coding!

//**Greetings by 
//**Jon Arve Ovesen

//**Notepad++
//**https://www.bing.com/search?q=notepad++&cvid=9291ec639f394aa3b61450ee49e0ac2e&form=WSBSTK






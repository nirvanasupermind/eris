#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <utility>
#include <string>
#include <vector>
#include <regex>
#include <cctype>

#include "token.hpp"
#include "util.hpp"

namespace eris
{
    /**
     * Tokenizer spec.
     */

    std::vector<std::pair<std::string, std::string> > Spec{
        // --------------------------------------------
        // Whitespace:

        {"^\\s+", "SKIP"},

        // --------------------------------------------
        // Comments:

        // Skip single-line comments:
        {"^//.*", "SKIP"},

        // Skip multi-line comments:
        {"^/\\*[\\s\\S]*?\\*/", "SKIP"},

        // --------------------------------------------
        // Numbers:

        {"^\\d+", "NUMBER"},

        // --------------------------------------------
        // Strings:

        {"^\"[^\"]*\"", "STRING"}};

    /**
     * Tokenizer class.
     *
     * Lazily pulls a token from the stream.
     */
    class Tokenizer
    {
    public:
        std::string string;
        int cursor;
        int line;

        Tokenizer() = default;

        /**
         * Initializes the string.
         */
        void init(const std::string &string)
        {
            this->string = string;
            this->cursor = 0;
            this->line = 1;
        }

        /**
         * Whether the tokenizer reached EOF.
         */
        bool isEOF() const
        {
            return this->cursor == this->string.size();
        }

        /**
         * Whether we still have more tokens.
         */
        bool hasMoreTokens() const
        {
            return this->cursor < this->string.size();
        }

        /**
         * Obtains next token.
         */
        Token getNextToken()
        {
            if (!hasMoreTokens())
            {
                return Token("EOF", "<EOF>");
            }

            std::string string = this->string.substr(this->cursor);

            for (std::size_t i = 0; i < Spec.size(); i++)
            {
                std::regex regex(Spec.at(i).first);
                std::string tokenType = Spec.at(i).second;

                std::string lexeme = match(regex, string);

                // Couldn't match this rule, continue.
                if (lexeme == "")
                {
                    continue;
                }

                // Should skip token, e.g. whitespace.
                if (tokenType == "SKIP") 
                {
                    return getNextToken();
                }

                return Token(tokenType, lexeme);
            }

            throw std::string(std::to_string(line) + ": unexpected token: \"" + string[0] + "\"");
            return Token();
        }

        /**
         * Matches a token for the regular expression.
         */
        std::string match(const std::regex &regex, const std::string &string)
        {
            std::smatch sm;
            std::regex_search(string, sm, regex);
            std::string matched = sm.str();

            if (matched != "")
            {
                this->cursor += matched.size();
                this->line += countNewlines(matched);
            }

            return matched;
        }
    };
}

#endif
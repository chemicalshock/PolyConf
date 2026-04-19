// -------------------------------------------------------------
//
//!\file xml_parser.cpp
//!\brief XML parser for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "xml_parser.hpp"

// inbuilt
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// internal
#include "node.hpp"

//
//!\brief Trim whitespace from both ends
//
std::string POLYCONF_xml_trim(const std::string& text)
{
    std::size_t start = 0;
    std::size_t end = text.size();

    while (start < end && std::isspace(static_cast<unsigned char>(text[start])))
    {
        ++start;
    }

    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])))
    {
        --end;
    }

    return text.substr(start, end - start);
}

//
//!\brief Decode XML text entities
//
std::string POLYCONF_xml_unescape_text(const std::string& text)
{
    std::string output;
    std::size_t i = 0;

    output.reserve(text.size());

    while (i < text.size())
    {
        if (text.compare(i, 5, "&amp;") == 0)
        {
            output.push_back('&');
            i += 5;
            continue;
        }

        if (text.compare(i, 4, "&lt;") == 0)
        {
            output.push_back('<');
            i += 4;
            continue;
        }

        if (text.compare(i, 4, "&gt;") == 0)
        {
            output.push_back('>');
            i += 4;
            continue;
        }

        if (text.compare(i, 6, "&quot;") == 0)
        {
            output.push_back('"');
            i += 6;
            continue;
        }

        if (text.compare(i, 6, "&apos;") == 0)
        {
            output.push_back('\'');
            i += 6;
            continue;
        }

        output.push_back(text[i]);
        ++i;
    }

    return output;
}

class POLYCONF_XML_READER final
{
private:
    const std::string& m_input;
    std::size_t m_pos = 0;
    std::vector<std::string> m_pending_comments;

public:
    //
    //!\brief Construct XML reader
    //
    explicit POLYCONF_XML_READER(const std::string& input)
        : m_input(input)
    {
    }

    //
    //!\brief Parse the full XML document into a root node
    //
    POLYCONF::NODE parse_document()
    {
        POLYCONF::NODE root;

        skip_misc();

        while (!eof())
        {
            std::pair<std::string, POLYCONF::NODE> element = parse_element();

            root.add_child(element.first, std::move(element.second));
            skip_misc();
        }

        apply_pending_as_trailing_comments(root);

        return root;
    }

private:
    //
    //!\brief Check whether end of input is reached
    //
    bool eof() const
    {
        return m_pos >= m_input.size();
    }

    //
    //!\brief Get current character
    //
    char peek() const
    {
        if (eof())
        {
            return '\0';
        }

        return m_input[m_pos];
    }

    //
    //!\brief Check whether input starts with text at current position
    //
    bool starts_with(const std::string& text) const
    {
        if ((m_pos + text.size()) > m_input.size())
        {
            return false;
        }

        return m_input.compare(m_pos, text.size(), text) == 0;
    }

    //
    //!\brief Advance by one character
    //
    void advance()
    {
        if (!eof())
        {
            ++m_pos;
        }
    }

    //
    //!\brief Expect a specific character
    //
    void expect(char ch)
    {
        if (peek() != ch)
        {
            throw std::runtime_error("XML parse error: unexpected character");
        }

        advance();
    }

    //
    //!\brief Skip whitespace
    //
    void skip_whitespace()
    {
        while (!eof() && std::isspace(static_cast<unsigned char>(peek())))
        {
            advance();
        }
    }

    //
    //!\brief Skip XML declaration
    //
    void skip_declaration()
    {
        std::size_t end = m_input.find("?>", m_pos);

        if (end == std::string::npos)
        {
            throw std::runtime_error("XML parse error: unterminated declaration");
        }

        m_pos = end + 2;
    }

    //
    //!\brief Read XML comment text
    //
    std::string read_comment()
    {
        std::size_t start = m_pos + 4;
        std::size_t end = m_input.find("-->", start);

        if (end == std::string::npos)
        {
            throw std::runtime_error("XML parse error: unterminated comment");
        }

        m_pos = end + 3;
        return POLYCONF_xml_trim(m_input.substr(start, end - start));
    }

    //
    //!\brief Attach pending comments before a node
    //
    void apply_pending_as_leading_comments(POLYCONF::NODE& node)
    {
        std::size_t i = 0;

        while (i < m_pending_comments.size())
        {
            node.add_comment_before(std::move(m_pending_comments[i]));
            ++i;
        }

        m_pending_comments.clear();
    }

    //
    //!\brief Attach pending comments after a node
    //
    void apply_pending_as_trailing_comments(POLYCONF::NODE& node)
    {
        std::size_t i = 0;

        while (i < m_pending_comments.size())
        {
            node.add_trailing_comment(std::move(m_pending_comments[i]));
            ++i;
        }

        m_pending_comments.clear();
    }

    //
    //!\brief Skip declarations comments and whitespace
    //
    void skip_misc()
    {
        bool changed = true;

        while (changed)
        {
            changed = false;

            skip_whitespace();

            if (starts_with("<?"))
            {
                skip_declaration();
                changed = true;
                continue;
            }

            if (starts_with("<!--"))
            {
                m_pending_comments.push_back(read_comment());
                changed = true;
                continue;
            }
        }
    }

    //
    //!\brief Parse element name
    //
    std::string parse_name()
    {
        std::size_t start = m_pos;

        while (!eof())
        {
            char ch = peek();

            if (std::isalnum(static_cast<unsigned char>(ch)) || ch == '_' || ch == '-' || ch == ':')
            {
                advance();
                continue;
            }

            break;
        }

        if (m_pos == start)
        {
            throw std::runtime_error("XML parse error: expected name");
        }

        return m_input.substr(start, m_pos - start);
    }

    //
    //!\brief Skip attributes inside an opening tag
    //
    void skip_attributes()
    {
        while (!eof())
        {
            char ch = peek();

            if (ch == '>' || ch == '/')
            {
                return;
            }

            if (ch == '"' || ch == '\'')
            {
                char quote = ch;

                advance();

                while (!eof() && peek() != quote)
                {
                    advance();
                }

                expect(quote);
                continue;
            }

            advance();
        }
    }

    //
    //!\brief Parse one element
    //
    std::pair<std::string, POLYCONF::NODE> parse_element()
    {
        std::string name;
        POLYCONF::NODE node;
        std::string text;

        skip_misc();

        expect('<');
        name = parse_name();
        apply_pending_as_leading_comments(node);

        skip_attributes();

        if (starts_with("/>"))
        {
            m_pos += 2;
            return std::make_pair(name, node);
        }

        expect('>');

        while (!eof())
        {
            if (starts_with("<!--"))
            {
                m_pending_comments.push_back(read_comment());
                continue;
            }

            if (starts_with("</"))
            {
                std::string closing_name;

                m_pos += 2;
                closing_name = parse_name();
                skip_whitespace();
                expect('>');

                if (closing_name != name)
                {
                    throw std::runtime_error("XML parse error: mismatched closing tag");
                }

                apply_pending_as_trailing_comments(node);
                break;
            }

            if (peek() == '<')
            {
                std::pair<std::string, POLYCONF::NODE> child = parse_element();

                node.add_child(child.first, std::move(child.second));
                continue;
            }

            text.push_back(peek());
            advance();
        }

        text = POLYCONF_xml_trim(text);

        if (!text.empty())
        {
            node.set_value(POLYCONF_xml_unescape_text(text));
        }

        return std::make_pair(name, node);
    }
};

//
//!\brief Parse XML input into a configuration object
//
POLYCONF::CONFIG POLYCONF::parse_xml(const std::string& input)
{
    POLYCONF_XML_READER reader(input);

    return POLYCONF::CONFIG(reader.parse_document());
}

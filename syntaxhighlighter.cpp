#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    /*QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" <<"\\busing\\b";
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    commentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);*/
    // Define the list of C++ keywords
    keywords << "alignas" << "alignof" << "and" << "and_eq" << "asm" << "auto" << "bitand"
             << "bitor" << "bool" << "break" << "case" << "catch" << "char" << "char8_t"
             << "char16_t" << "char32_t" << "class" << "compl" << "concept" << "const"
             << "consteval" << "constexpr" << "const_cast" << "continue" << "decltype"
             << "default" << "delete" << "do" << "double" << "dynamic_cast" << "else"
             << "enum" << "explicit" << "export" << "extern" << "false" << "float"
             << "for" << "friend" << "goto" << "if" << "inline" << "int" << "long"
             << "mutable" << "namespace" << "new" << "noexcept" << "not" << "not_eq"
             << "nullptr" << "operator" << "or" << "or_eq" << "private" << "protected"
             << "public" << "register" << "reinterpret_cast" << "requires" << "return"
             << "short" << "signed" << "sizeof" << "static" << "static_assert"
             << "static_cast" << "struct" << "switch" << "template" << "this" << "thread_local"
             << "throw" << "true" << "try" << "typedef" << "typeid" << "typename"
             << "union" << "unsigned" << "using" << "virtual" << "void" << "volatile"
             << "wchar_t" << "while" << "xor" << "xor_eq";
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    /*for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);*/

    lexertk::generator lexer;
    std::string std_text = text.toStdString();
    lexer.process(std_text);

    for (std::size_t i = 0; i < lexer.size(); ++i) {
        lexertk::token t = lexer[i];
        QString token_value = QString::fromStdString(t.value);

        if (t.type == lexertk::token::token_type::e_symbol ) {
            // If it's a keyword, highlight it
            if (keywords.contains(token_value)) {
                setFormat(t.position, t.value.length(), keywordFormat);
            }
        }
    }
}

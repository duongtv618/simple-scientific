#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <math.h>

double do_brace(std::string &);
double do_no_brace(std::string &);
double do_sin(std::string &, int);
double do_add(std::string &);
int isOperand(char);

int isOperand(char c)
{
    if (c == '-' || c == '+' || c == '*' || c == '/')
        return 1;
    return 0;
}

/** 1 + 2 * ( 2 + ( 2+ 3) * 4 )*/
int validate_math(std::string &s)
{
    int brCnt{0};
    int hasOperand{0};
    int hasDigit{0};
    int j;
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '(')
            brCnt++;
        else if (s[i] == ')')
            brCnt--;

        if (isOperand(s[i]))
            hasOperand++;

        if (isdigit(s[i]))
            hasDigit++;

        if (s[i] == '*' || s[i] == '/')
        {
            if (i == 0)
                return -1;
        }

        if (isalpha(s[i]))
        {
            j = i;
            while (isalpha(s[j]))
                j++;

            std::string sub = s.substr(i, j - i);

            if (!((sub.compare("sin") == 0) ||
                  (sub.compare("cos") == 0) ||
                  (sub.compare("tan") == 0) ||
                  (sub.compare("cot") == 0)))
                return -1;

            i = j - 1;
        }

        if (brCnt < 0)
            break;
    }

    if (brCnt != 0)
        return -1;

    if (hasDigit == 0)
        return -1;

    if (isOperand(s.at(s.length() - 1)))
        return -1;

    return 0;
}

double do_add(std::string &s)
{
    int pos, lpos;
    std::string l, r;
    std::cout << "Do add:\t\t" << s << '\n';
    if (s.length() == 0)
        return 0.0;

    pos = s.find_last_of("+-");

    if (s.npos == pos)
        return std::stod(s);

    l = s.substr(0, pos);
    r = s.substr(pos + 1, s.length() - pos - 1);

    if (s.at(pos) == '-')
        return do_add(l) - do_add(r);
    else
        return do_add(l) + do_add(r);
}

double do_sin(std::string &s, int pos)
{
    int brCnt, i, lpos;
    double r;
    std::cout << "Do sin:\t\t" << s << '\n';
    enum
    {
        SIN,
        COS,
        TAN,
        COT
    } type;

    if (s.compare(pos, 3, "sin") == 0)
        type = SIN;
    else if (s.compare(pos, 3, "cos") == 0)
        type = COS;
    else if (s.compare(pos, 3, "tan") == 0)
        type = TAN;
    else if (s.compare(pos, 3, "cot") == 0)
        type = COT;
    else
    {
        std::cout << "Unknow type\n";
        return 0.0;
    }

    if (s.at(pos + 3) == '(') // have brace right after sin
    {
        brCnt = 1;
        i = pos + 4;
        while (brCnt > 0)
        {
            if (s.at(i) == '(')
                brCnt++;
            if (s.at(i) == ')')
                brCnt--;
            i++;
        }
        std::string sub = s.substr(pos + 4, i - pos - 5);
        r = do_brace(sub);
        r = (type == SIN) ? sin(r) : (type == COS) ? cos(r)
                                 : (type == TAN)   ? tan(r)
                                                   : 1.0 / tan(r);
        s.replace(pos, i - pos, std::to_string((r)));
        return do_no_brace(s);
    }
    else
    {
        lpos = s.find_first_of("+-*/", pos + 1);
        if (lpos == s.npos) // sin1 alone
        {
            r = std::stod(s.substr(pos + 3, s.length() - pos - 3));

            r = (type == SIN) ? sin(r) : (type == COS) ? cos(r)
                                     : (type == TAN)   ? tan(r)
                                                       : 1.0 / tan(r);
            s.replace(pos, s.length() - pos, std::to_string((r)));
        }
        else // sin1+-something
        {
            r = std::stod(s.substr(pos + 3, lpos - pos - 3));
            r = (type == SIN) ? sin(r) : (type == COS) ? cos(r)
                                     : (type == TAN)   ? tan(r)
                                                       : 1.0 / tan(r);
            s.replace(pos, lpos - pos, std::to_string((r)));
        }
        return do_no_brace(s);
    }
}

double do_no_brace(std::string &s)
{
    int pos, fpos, lpos, i;
    double l, r, replace;
    int brCnt;
    std::cout << "Do no brace:\t\t" << s << '\n';
    if (s.length() == 0)
        return 0.0;

    pos = s.find_first_of("sct");
    if (pos != s.npos) // have sin1
        return do_sin(s, pos);

    pos = s.find_first_of("*/");

    if (s.npos == pos)
        return do_add(s);

    fpos = s.find_first_of("-+");

    if (fpos < pos) /** has - or + before pos*/
    {
        lpos = s.find_first_of("+-*/", pos + 1); /** find next +-/**/
        // Then replace
        l = std::stod(s.substr(fpos + 1, pos - fpos));
        r = std::stod(s.substr(pos + 1, lpos - pos));
        replace = (s.at(pos) == '*') ? l * r : l / r;

        s.replace(fpos + 1, lpos - fpos - 1, std::to_string(replace));
        return do_no_brace(s);
    }
    else /** has - or + after pos */
    {
        lpos = s.find_first_of("+-*/", pos + 1); /** find next +-/**/
                                                 // Then replace
        l = std::stod(s.substr(1, pos - 1));
        r = std::stod(s.substr(pos + 1, lpos - pos));
        replace = (s.at(pos) == '*') ? l * r : l / r;

        s.replace(fpos + 1, lpos - fpos - 1, std::to_string(replace));
        return do_no_brace(s);
    }
}

//((1 + 2 * (2 + 3)))
// find inner brace and replace with value
double do_brace(std::string &s)
{
    int pos, brCnt, lpos;
    double r, l;
    std::cout << "Do brace:\t\t" << s << '\n';

    pos = s.find_first_of('(');

    if (s.npos == pos)
        return do_no_brace(s);

    // pos = s.find_first_of('s');
    // if (pos != s.npos) // have sin1
    //     return do_sin(s, pos);

    brCnt = 1;
    lpos = pos + 1;
    while (brCnt > 0) // find close bracket for the pair
    {
        if (s.at(lpos) == '(')
            brCnt++;

        if (s.at(lpos) == ')')
            brCnt--;
        lpos++;
    }

    lpos--;
    std::string sub = s.substr(pos + 1, lpos - pos - 1);
    s.replace(pos, lpos - pos + 1, std::to_string(do_brace(sub)));
    return do_brace(s);
}

double do_math(std::string &s)
{
    if (s.length() == 0)
        return 0.0;

    if (validate_math(s) == -1)
    {
        std::cout << "Syntax error\n";
        return 0.0;
    }

    return do_brace(s);
}

int main(void)
{
    std::string line;
    while (true)
    {
        /** get line and remove all space between */
        std::getline(std::cin, line);
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        // std::cout << line << '\n';

        /** q is quit */
        if (line == "q")
            return 0;

        /** call recursive through all string then return value in double*/
        double out = do_math(line);
        std::cout << "\t\t" << out << '\n';
    }

    return 0;
}
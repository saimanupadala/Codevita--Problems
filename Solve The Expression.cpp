#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

static vector<string> toks;   // "num", "or", "and", "not", "(", ")"
static vector<string> vals;   // bit string for "num" tokens
static size_t pos_ = 0;
static string OR, AND, NOT, LP, RP;

static string bop(string a, string b, bool isAnd) {
    size_t n = max(a.size(), b.size());
    a = string(n - a.size(), '0') + a;
    b = string(n - b.size(), '0') + b;
    string r(n, '0');
    for (size_t i = 0; i < n; i++) {
        bool x = a[i] == '1', y = b[i] == '1';
        r[i] = (isAnd ? (x && y) : (x || y)) ? '1' : '0';
    }
    return r;
}

string parseAnd();

static string parsePrimary() {
    if (pos_ >= toks.size()) return "";
    if (toks[pos_] == "(") {
        pos_++;
        string v = parseAnd();
        if (pos_ < toks.size() && toks[pos_] == ")") pos_++;
        return v;
    }
    return vals[pos_++];
}

static string parseUnary() {
    if (pos_ < toks.size() && toks[pos_] == "not") {
        pos_++;
        string v = parseUnary();
        for (size_t i = 0; i < v.size(); i++) v[i] = (v[i] == '1') ? '0' : '1';
        return v;
    }
    return parsePrimary();
}

static string parseOr() {
    string v = parseUnary();
    while (pos_ < toks.size() && toks[pos_] == "or") {
        pos_++;
        v = bop(v, parseUnary(), false);
    }
    return v;
}

string parseAnd() {
    string v = parseOr();
    while (pos_ < toks.size() && toks[pos_] == "and") {
        pos_++;
        v = bop(v, parseOr(), true);
    }
    return v;
}

static vector<string> readSymbols(const string &r0, const string &r1,
                                  const string &r2, int count) {
    const string *rs[3] = { &r0, &r1, &r2 };
    vector<string> res(count);
    for (int i = 0; i < count; i++) {
        string bits;
        for (int r = 0; r < 3; r++)
            for (int j = 0; j < 3; j++) {
                size_t idx = 3 * i + j;
                char c = idx < rs[r]->size() ? (*rs[r])[idx] : ' ';
                bits += (c != ' ') ? '1' : '0';
            }
        res[i] = bits;
    }
    return res;
}

static bool blankLine(const string &s) {
    return s.find_first_not_of(" \t") == string::npos;
}

static bool readLine(string &line) {
    if (!getline(cin, line)) { line.clear(); return false; }
    while (!line.empty() && (line[line.size() - 1] == '\r' ||
                             line[line.size() - 1] == '\n'))
        line.erase(line.size() - 1);
    return true;
}

static bool isOp(const string &s) {
    return s == OR || s == AND || s == NOT || s == LP || s == RP;
}

int main() {
    // Read exactly 9 rows and stop: never wait for end-of-file.
    vector<string> rows(9);
    string line, second;
    readLine(rows[0]);
    readLine(second);
    if (blankLine(second)) {
        // blank separator line after every row
        for (int k = 1; k < 9; k++) {
            readLine(rows[k]);
            if (k < 8) readLine(line);   // consume the separator
        }
    } else {
        rows[1] = second;
        for (int k = 2; k < 9; k++) readLine(rows[k]);
    }

    vector<string> digits = readSymbols(rows[0], rows[1], rows[2], 10);
    vector<string> ops = readSymbols(rows[3], rows[4], rows[5], 5);
    OR = ops[0]; AND = ops[1]; NOT = ops[2]; LP = ops[3]; RP = ops[4];

    size_t w = max(rows[6].size(), max(rows[7].size(), rows[8].size()));
    int cnt = (int)((w + 2) / 3);
    vector<string> expr = readSymbols(rows[6], rows[7], rows[8], cnt);

    map<string, char> digitOf;
    for (int i = 0; i < 10; i++) digitOf[digits[i]] = (char)('0' + i);

    for (size_t i = 0; i < expr.size();) {
        const string &s = expr[i];
        if (s == OR)       { toks.push_back("or");  vals.push_back(""); i++; }
        else if (s == AND) { toks.push_back("and"); vals.push_back(""); i++; }
        else if (s == NOT) { toks.push_back("not"); vals.push_back(""); i++; }
        else if (s == LP)  { toks.push_back("(");   vals.push_back(""); i++; }
        else if (s == RP)  { toks.push_back(")");   vals.push_back(""); i++; }
        else if (digitOf.count(s)) {
            string num;
            while (i < expr.size() && !isOp(expr[i]) && digitOf.count(expr[i])) {
                num += expr[i];
                i++;
            }
            toks.push_back("num");
            vals.push_back(num);
        } else i++;   // blank / unknown cell
    }

    string result = parseAnd();

    if (result.size() % 9)
        result = string(9 - result.size() % 9, '0') + result;

    string out;
    for (size_t k = 0; k < result.size(); k += 9) {
        string chunk = result.substr(k, 9);
        if (digitOf.count(chunk)) out += digitOf[chunk];
        else out += '0';
    }

    size_t nz = out.find_first_not_of('0');
    if (nz == string::npos) cout << 0;
    else cout << out.substr(nz);
    return 0;
}


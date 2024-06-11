/* NONE */
/* BIBLIOTEKA: libecode_d */

module libecode_d;

import std.stdio;
import core.stdc.stdio;
import std.string;
import std.conv;
import std.algorithm;

string GetLineToStr(string S, int N, char Delims, string wynik = "", char TextSeparator = '"')
{

N=1;
    bool cc = false;
    size_t w = 0, i = 0, l = 0;
    string Result;

    foreach (c; S)
    {
        if (c == TextSeparator) cc = !cc;
        if (c == Delims && !cc) w++;
        else if (N - 1 == w)
        {
            l++;
            Result ~= c;
        }
        i++;
        if (i > S.length || w == N) break;
    }
    if (Result.length == 0) Result = wynik;
    return Result;
}

int GetLineCount(string aStr, char separator, char textseparator = '"') {
    int element_count = 1;
    bool in_quotes = false;

    foreach (c; aStr) {
        if (c == textseparator) in_quotes = !in_quotes;
        if (!in_quotes && c == separator) element_count++;
    }

    if (aStr.length == 0) {
        return 0;
    } else {
        return element_count;
    }
}


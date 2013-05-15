#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include "utf8.h"

using namespace std;

bool TestCommon()
{
    return false;
}

ostream& OutputWideChar(ostream& OS, const uint16_t& uc)
{
    OS << static_cast<unsigned char>(uc >> 0);
    OS << static_cast<unsigned char>(uc >> 8);
    return OS;
}

ostream& OutputWideChar(ostream& OS, const uint32_t& uc)
{
    OS << static_cast<unsigned char>(uc >> 0);
    OS << static_cast<unsigned char>(uc >> 8);
    OS << static_cast<unsigned char>(uc >> 16);
    OS << static_cast<unsigned char>(uc >> 24);
    return OS;
}

istream& InputWideChar(istream& IS, uint16_t& uc)
{
    uc = 0;
    char buf[2];
    IS.read(buf, sizeof(buf)/sizeof(buf[0]));
    uc += (static_cast<uint16_t>(static_cast<unsigned char>(buf[0])) << 0);
    uc += (static_cast<uint16_t>(static_cast<unsigned char>(buf[1])) << 8);
    return IS;
}

istream& InputWideChar(istream& IS, uint32_t& uc)
{
    uc = 0;
    char buf[4];
    IS.read(buf, sizeof(buf)/sizeof(buf[0]));
    uc += (static_cast<uint16_t>(static_cast<unsigned char>(buf[0])) << 0);
    uc += (static_cast<uint16_t>(static_cast<unsigned char>(buf[1])) << 8);
    uc += (static_cast<uint16_t>(static_cast<unsigned char>(buf[2])) << 16);
    uc += (static_cast<uint16_t>(static_cast<unsigned char>(buf[3])) << 24);
    return IS;
}

bool TestSplitUTF8()
{
    string src = "你好hello，世界world！";
    vector<string> vRef = 
    {
        "你", "好",
        "h", "e", "l", "l", "o",
        "，", 
        "世", "界", 
        "w", "o", "r", "l", "d", 
        "！",
    };
    //  assert
    if (true)
    {
        string tgt;
        for (size_t i=0; i<vRef.size(); ++i)
        {
            tgt += vRef[i];
        }
        assert(src == tgt);
    }
    //  split
    vector<string> vTgt;
    if (true)
    {
        string::const_iterator it = src.begin();   // string iterator
        string::const_iterator end = src.end();   // string iterator
        do
        {
            char symbol[] = {0, 0, 0, 0, 0, 0, 0, 0};   //  actually 5 zeros should be enough 
            uint32_t code = utf8::next(it, end);     // get 32 bit code of a utf-8 symbol
            if (code == 0)
            {
                continue;
            }
            utf8::append(code, symbol); // initialize array `symbol`
            vTgt.push_back(string(symbol));
        }
        while (it < end);
    }
    //  check
    if (vTgt != vRef)
    {
        cerr << "src : " << src << endl;
        cerr << "vRef: ";
        for (size_t i=0; i<vRef.size(); ++i)
        {
            if (i != 0)
            {
                cerr << "#";
            }
            cerr << vRef[i];
        }
        cerr << endl;
        cerr << "vTgt: ";
        for (size_t i=0; i<vTgt.size(); ++i)
        {
            if (i != 0)
            {
                cerr << "#";
            }
            cerr << vTgt[i];
        }
        cerr << endl;
        return false;
    }
    return true;
}

template <typename TWideChar>
bool TestUTF8ToOther()
{
    size_t line_num = 0;
    string line;
    //  byte order mark (BOM)
    TWideChar bom = 0xff + (0xfe << 8);
    OutputWideChar(cout, bom);
    //  end line
    TWideChar end_line = '\n';
    while (getline(cin, line))
    {
        ++line_num;
        //  check for invalid UTF-8 (for a simple yes/no check, there is also utf8::is_valid function)
        string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
        if (end_it != line.end())
        {
            cerr << "Invalid UTF-8 encoding detected at line " << line_num << endl;
            cerr << "This part is fine: " << string(line.begin(), end_it) << endl;
            return false;
        }
        //  get the line length (at least for the valid part)
        //cerr << "Length of line " << line_num << " is " << utf8::distance(line.begin(), end_it) << endl;
        //  convert it to other UTF
        vector<TWideChar> vWide;
        utf8::utf8to32(line.begin(), end_it, back_inserter(vWide));
        //  output
        for (size_t i=0; i<vWide.size(); ++i)
        {
            OutputWideChar(cout, vWide[i]);
        }
        OutputWideChar(cout, end_line);
    }
    return true;
}

template <typename TWideChar>
bool TestUTFOtherTo8()
{
    size_t line_num = 0;
    string line;
    //  byte order mark (BOM)
    TWideChar bom = 0xff + (0xfe << 8);
    //  end line
    TWideChar end_line = '\n';
    //  ignore BOM
    TWideChar uc;
    InputWideChar(cin, uc);
    if (uc != bom)
    {
        cerr << "The UTF-" << 8*sizeof(TWideChar) << ": Byte Order Mark does not match" << endl;
        cerr << "bom: " << bom << endl;
        cerr << "uc : " << uc  << endl;
        return false;
    }
    vector<TWideChar> vWide;
    while (InputWideChar(cin, uc))
    {
        if (uc == end_line)
        {
            ++line_num;
            // convert to utf-8
            string utf8line; 
            utf8::utf16to8(vWide.begin(), vWide.end(), back_inserter(utf8line));
            cout << utf8line << endl;
            vWide.clear();
        }
        else
        {
            vWide.push_back(uc);
        }
    }
    if (!vWide.empty())
    {
        ++line_num;
        // convert to utf-8
        string utf8line; 
        utf8::utf16to8(vWide.begin(), vWide.end(), back_inserter(utf8line));
        cout << utf8line << endl;
        vWide.clear();

    }
    return true;
}

int main(int argc, char* argv[])
{
    map<string, pair<function<bool(void)>, string> > mTest =
    {
        make_pair("--test-common",   make_pair(TestCommon,                "Test common function.")),
        make_pair("--test-split",    make_pair(TestSplitUTF8,             "Test split UTF-8 string to vector.")),
        make_pair("--test-utf8to16", make_pair(TestUTF8ToOther<uint16_t>, "Test convert UTF-8 to UTF-16.")),
        make_pair("--test-utf16to8", make_pair(TestUTFOtherTo8<uint16_t>, "Test convert UTF-16 to UTF-8.")),
        make_pair("--test-utf8to32", make_pair(TestUTF8ToOther<uint32_t>, "Test convert UTF-8 to UTF-32.")),
        make_pair("--test-utf32to8", make_pair(TestUTFOtherTo8<uint32_t>, "Test convert UTF-32 to UTF-8.")),
    };
    if (argc < 2)
    {
        const size_t max_space = 19;
        cerr << "Usage:" << endl;
        cerr << "    " << argv[0] << " [OPTION]" << endl;
        cerr << "Option:" << endl;
        for (auto mit=mTest.begin(); mit!=mTest.end(); ++mit)
        {
            const string& opt_name = mit->first;
            const string& opt_desc = mit->second.second;
            assert(opt_name.size() < max_space);
            cerr << "    " << opt_name << string(max_space-opt_name.size(), ' ') << opt_desc << endl;
        }
        cerr << "Note:" << endl;
        cerr << "    Only one option could be specified."<<endl;
        exit(1);
    }
    string opt_name = argv[1];
    if (true)
    {
        auto mit = mTest.find(opt_name);
        if (mit == mTest.end())
        {
            cerr << argv[0] << ": unknown test option: " << opt_name << endl;
            exit(1);
        }
        auto fTest = mit->second.first;
        if (!fTest())
        {
            cerr << argv[0] << ": call test function failed: " << opt_name << endl;
            exit(1);
        }
        else
        {
            if (opt_name.find("--test-utf") != 0)
            {
                cerr << "passed " << opt_name << endl;
            }
        }
    }
    return 0;
}


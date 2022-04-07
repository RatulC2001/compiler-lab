#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

struct sym
{
    string s;
    
    sym()
    {
        ;
    }

    sym(string str)
    {
        s = str;
    }

    bool is_terminal()
    {
        return s[0]=='"';
    }
};

ostream & operator << (ostream &out, const sym &s)
{
    string str = s.s;
    if(str[0] == '"')
    {
        str.pop_back();
        str.erase(0, 1);
    } 
    out << str;
    return out;
}

bool operator == (const sym &s1, const sym &s2)
{
    return s1.s == s2.s;
}

struct production
{
    sym lhs;
    vector <sym> rhs;
    int dot_loc = -1;

    production()
    {
        ;
    }

    production(sym l, vector <sym> r)
    {
        lhs = l;
        rhs = r;
    }

    production(sym l, vector <sym> r, int pos)
    {
        lhs = l;
        rhs = r;
        dot_loc = pos;
    }

    int count_rhs() const
    {
        return int(rhs.size());
    }

    bool can_extend()
    {
        if(dot_loc == -1) return false;
        if(dot_loc > count_rhs() - 1) return false;

        return !rhs[dot_loc].is_terminal();
    }
};

ostream & operator << (ostream &out, const production &p)
{
    out << p.lhs << " -> ";
    if (p.dot_loc == -1)
    {
        for(auto s : p.rhs) cout << s << " ";
    }
    else
    {
        for(int i = 0; i < p.count_rhs(); i++)
        {
            if(i == p.dot_loc) cout << " . ";
            cout << p.rhs[i] << " "; 
        }
    }
    return out;
}

bool operator == (const production &p1, const production &p2) 
{
    if(p1.dot_loc != p2.dot_loc) return false;
    if(!(p1.lhs == p2.lhs)) return false;
    if(p1.count_rhs() != p2.count_rhs()) return false;

    for(int i = 0; i < p1.count_rhs(); i++)
    {
        if(!(p1.rhs[i] == p2.rhs[i])) return false;
    }

    return true;
}

vector <production> productions;

bool does_exist(vector <production> &p, production s)
{
    for(auto prod : p) if(prod == s) return true;
    return false;
}

struct item_set
{
    vector <production> prod_list;

    item_set()
    {
        ;
    }

    item_set(vector <production> productions)
    {
        prod_list = productions;
    }

    void get_closure()
    {
        stack <production> st;
        for(auto p : prod_list) st.push(p);

        while(!st.empty())
        {
            production p = st.top();
            st.pop();

            sym start = p.rhs[p.dot_loc];

            if(p.can_extend())
            {
                for(auto prod : productions)
                {
                    if(prod.lhs == start)
                    {
                        prod.dot_loc = 0;

                        if(!does_exist(prod_list, prod))
                        {
                            prod_list.push_back(prod);
                            st.push(prod);
                        }
                    }
                }
            }
        }
    }
};

ostream & operator << (ostream &out, const item_set &is)
{
    for(auto prod : is.prod_list) cout << prod <<endl;
    return out;
}

signed main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int cnt;
    cin >> cnt;

    for(int i = 0; i < cnt; i++)
    {
        string str;
        cin >> str;   // gets lhs
        sym lhs(str);
        cin >> str;   // gets :
        vector <sym> rhs;
        while(true)
        {
            cin >> str;     // get rhs
            if(str == ";")  break;
            rhs.push_back(sym(str));
        }

        productions.push_back(production(lhs, rhs));
    }

    vector <production> plist;
    plist.push_back(production(productions[1].lhs, productions[1].rhs, 4));
    item_set is = item_set(plist);
    is.get_closure();
    cout << is;

    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <stdio.h>

using namespace std;

struct sym
{
    string s;
    int id;
    
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

    vector <sym> follow()
    {
        vector <sym> ret;

        if(s == "statements")
        {
            ret.push_back(sym("\"}\""));
        }
        else if(s == "statement")
        {
            ret.push_back(sym("\";\""));
        }
        else if(s == "dec_st")
        {
            ret.push_back(sym("\";\""));
        }
        else if(s == "math_st")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\",\""));
        }
        else if(s == "io")
        {
            ret.push_back(sym("\";\""));
        }
        else if(s == "if_st")
        {
            ret.push_back(sym("\";\""));
        }        
        else if(s == "d_prod")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\",\""));
        }
        else if(s == "d_nat")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\",\""));
        }
        else if(s == "VALUE")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\",\""));
            ret.push_back(sym("\")\""));
            ret.push_back(sym("\"o_cas\""));
            ret.push_back(sym("\"comp_op\""));
            ret.push_back(sym("\"addop\""));
        }
        else if(s == "inp")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\"i_cas\""));
        }
        else if(s == "opt")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\"o_cas\""));
        }
        else if(s == "comp_st")
        {
            ret.push_back(sym("\")\""));
        }
        else if(s == "CONT")
        {
            ret.push_back(sym("\";\""));
        }
        else if(s == "TERM")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\",\""));
            ret.push_back(sym("\")\""));
            ret.push_back(sym("\"o_cas\""));
            ret.push_back(sym("\"comp_op\""));
            ret.push_back(sym("\"addop\""));
            ret.push_back(sym("\"mulop\""));
        }
        else if(s == "FAC")
        {
            ret.push_back(sym("\";\""));
            ret.push_back(sym("\",\""));
            ret.push_back(sym("\")\""));
            ret.push_back(sym("\"o_cas\""));
            ret.push_back(sym("\"comp_op\""));
            ret.push_back(sym("\"addop\""));
            ret.push_back(sym("\"mulop\""));
        }

        return ret;
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

bool operator < (const sym &s1, const sym &s2)
{
    return s1.s < s2.s;
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

        if(p.dot_loc == p.count_rhs())
        cout << " . ";
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
    static int count;
    int id;
    vector <production> prod_list;

    item_set()
    {
        id = count;
        count ++;
    }

    item_set(vector <production> productions)
    {
        id = count;
        count ++;
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

            if(p.can_extend())
            {
                sym start = p.rhs[p.dot_loc];
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

int item_set::count = 0;

ostream & operator << (ostream &out, const item_set &is)
{
    for(auto prod : is.prod_list) cout << prod <<endl;
    return out;
}

bool operator == (item_set &is1, item_set &is2)
{
    if((int)(is1.prod_list.size()) != (int)(is2.prod_list.size())) return false;

    int n = (int)(is1.prod_list.size());

    for(int i = 0; i < n; i ++)
    {
        if(!(is1.prod_list[i] == is2.prod_list[i])) return false;
    }

    return true;
}

vector <pair<int,sym>> adj[1000];      // contains indices in the item_list

vector <item_set> item_list;

int item_in(item_set is)
{
    for(int i = 0; i < (int)(item_list.size()) ; i ++) if(item_list[i] == is) return i;
    return -1;
}

vector <sym> nextsyms(item_set is)
{
    set <sym> clist;
    for(auto prod : is.prod_list)
    {
        if(prod.dot_loc < prod.count_rhs())
        {
            clist.insert(prod.rhs[prod.dot_loc]);
        }
    }
    vector <sym> rlist;
    for(sym s : clist) rlist.push_back(s);

    return rlist;
}

item_set transition(item_set is, sym s)
{
    item_set inext;
    for(auto prod : is.prod_list)
    {
        if(prod.dot_loc < prod.count_rhs())
        {
            if(prod.rhs[prod.dot_loc] == s)
            {
                prod.dot_loc += 1;
                inext.prod_list.push_back(prod);
            }
        }
    }
    inext.get_closure();
    return inext;
}

void generate(item_set is)
{
    vector <sym> symlist = nextsyms(is);
    int curridx = item_in(is);

    for(sym s : symlist)
    {
        item_set nis = transition(is, s);

        int idx;
        if(item_in(nis) == -1)
        {
            item_list.push_back(nis);
            idx = item_in(nis);
            adj[curridx].push_back({idx, s});
            generate(nis);
        }
        else
        {
            idx = item_in(nis);
            adj[curridx].push_back({idx, s});
        }
    }
}

void printst(stack <int> st)
{
    if (st.empty()) return;
    else
    {
        int val = st.top();
        st.pop();
        printst(st);
        cout << val << " ";
    }
}

void repr(stack <int> st, vector <sym> &symbols, vector <sym> &term_list)
{
    cout << "STACK : ";
    printst(st);
    cout << endl;
    cout << "SYMBOLS : ";
    for(sym s : symbols)
    cout << s << " ";
    cout << endl;
    cout << "INPUT : ";
    for(int i = (int)(term_list.size()) - 1; i >= 0; i --) cout << term_list[i] << " ";
    cout << endl;
    cout << endl;
}

vector <sym> globalsymbols;
vector <int> graph[1000];
int symid = 0;

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
    plist.push_back(production(productions[0].lhs, productions[0].rhs, 0));
    item_set is = item_set(plist);
    is.get_closure();
    item_list.push_back(is);

    generate(is);

    int n = (int)item_list.size();

    for(int i = 0; i < n; i ++) item_list[i].id = i;

    for(item_set item : item_list)
    {
        cout << "I (" << item.id << ")" << endl;
        cout << item << endl;
    }

    int term_cnt;
    cin >> term_cnt;

    vector <sym> term_list;
    for(int i = 0; i < term_cnt; i ++)
    {
        string str;
        cin >> str;
        sym s(str);
        s.id = symid;
        symid ++;
        globalsymbols.push_back(s);
        term_list.push_back(s);
    }

    reverse(term_list.begin(), term_list.end());

    stack <int> st;
    st.push(0);

    vector <sym> symbols;

    repr(st, symbols, term_list);

    for(int itr = 0; itr < 500 ; itr ++)
    {
        if((int)(term_list.size()) == 1 && st.top() + 1 == (int)(item_list.size())) break;

        sym curr = *term_list.rbegin();
        int pos = st.top();

        vector <sym> trans = nextsyms(item_list[pos]);
        bool found = false;
        bool shift_to = false;
        for(int i = 0; i < (int)(trans.size()); i ++) if(trans[i] == curr) found = true;

        for(production prod : item_list[pos].prod_list)
        {
            if(prod.dot_loc != prod.count_rhs())
            {
                if(prod.rhs[prod.dot_loc] == sym("%empty"))
                {
                    shift_to = true;
                }
            }
        }

        if(found)
        {
            // cout << "IN1" << endl;
            int next_item;
            for(int i = 0; i < (int)(adj[pos].size()); i ++) 
            {
                if(adj[pos][i].second == curr)
                {
                    next_item = adj[pos][i].first;
                }
            }

            term_list.pop_back();
            symbols.push_back(curr);
            st.push(next_item);
        }
        else if (shift_to)
        {
            // cout << "IN2" << endl;
            sym addsym("%empty");
            addsym.id = symid;
            symid ++;
            globalsymbols.push_back(addsym);
            term_list.push_back(addsym);
        }
        else
        {
            // cout << "IN3" << endl;
            production req;
            bool no_production = true;
            int end_count = 0;
            for(production prod : item_list[pos].prod_list)
            {
                if(prod.dot_loc == prod.count_rhs())
                {
                    vector <sym> followset = prod.lhs.follow();
                    bool found = false;

                    if(curr == sym("$")) found = true, end_count += 1;

                    for(sym s : followset)
                    {
                        if(s == curr)
                        {
                            found = true;
                        }
                    }

                    if(found)
                    {
                        req = prod;
                        no_production = false;
                    }
                }
            }

            if(end_count > 1) no_production = true;

            if(no_production)
            {
                cout << "ERROR" << endl;
                exit(0);
            }

            sym new_sym(req.lhs.s);
            new_sym.id = symid;
            symid ++;
            globalsymbols.push_back(new_sym);

            vector <int> to_append; 

            for(int i = 0; i < req.count_rhs(); i ++) 
            {
                st.pop();
                sym curr_sym = *symbols.rbegin();
                to_append.push_back(curr_sym.id);
                symbols.pop_back();
            }

            reverse(to_append.begin(), to_append.end());
            for(int idx : to_append) graph[new_sym.id].push_back(idx);

            pos = st.top();

            int next_item;
            for(int i = 0; i < (int)(adj[pos].size()); i ++) 
            {
                if(adj[pos][i].second == req.lhs)
                {
                    next_item = adj[pos][i].first;
                }
            }

            st.push(next_item);
            symbols.push_back(new_sym);
        }

        repr(st, symbols, term_list);
    }
    
    cout << endl;

    FILE *fp;
    fp = fopen("graph.md", "w");

    fprintf(fp, "```mermaid\ngraph TD\n");

    for(int i = 0; i < 999; i ++)
    {
        for (int j : graph[i])
        {
            fprintf(fp, "%d(%s) --> %d(%s);\n", i, globalsymbols[i].s.c_str(), j, globalsymbols[j].s.c_str());
        }
    }

    fprintf(fp, "```\n");

    fclose(fp);

    return 0;
}
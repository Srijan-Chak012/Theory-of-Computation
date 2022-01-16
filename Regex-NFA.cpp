#include <iostream>
#include <vector>
#include <string>
#include <ctype.h>
#include <stdlib.h>
using namespace std;

int priority(char op)
{
    int set_priority = 0;
    if (op == '*') //star
    {
        set_priority = 9;
    }

    else if (op == '+') //union
    {
        set_priority = 1;
    }

    else if (op == '/') //concatenation
    {
        set_priority = 5;
    }

    return set_priority;
}

class NFA
{
public:
    int states;
    vector<vector<vector<int>>> transition;

    NFA(int statenumber)
    {
        states = statenumber;
        transition.assign(states, vector<vector<int>>(27, vector<int>()));
    }
};

// vector<int> read_transition(NFA A, int from, char trans)
// {
//     return (A.transition[from][trans]);
// }

NFA Create(string str)
{
    vector<NFA> stack;

    for (int c = 0; c < str.length(); c++)
    {
        if (isalpha(str[c]) > 0)
        {
            NFA var(2);
            var.transition[0][str[c] - 'a'].push_back(1);
            stack.push_back(var);
        }

        else if (str[c] == '+')
        {
            NFA var1 = stack.back();
            stack.pop_back();
            NFA var2 = stack.back();
            stack.pop_back();

            NFA newstate(2 + var1.states + var2.states);
            //  newstate.states = 2 + var1.states + var2.states;

            newstate.transition[0][26].push_back(1);
            newstate.transition[0][26].push_back(var1.states + 1);


            for (int i = 1; i <= var1.states; i++)
            {
                for (int j = 0; j < 27; j++)
                {
                    vector<int> array = var1.transition[i - 1][j];
                    for (int k = 0; k < array.size(); k++)
                    {
                        newstate.transition[i][j].push_back(array[k] + 1);
                    }
                }
            }

            for (int i = var1.states + 1; i < (newstate.states - 1); i++)
            {
                for (int j = 0; j < 27; j++)
                {
                    vector<int> array = var2.transition[i - 1 - var1.states][j];
                    for (int k = 0; k < array.size(); k++)
                    {
                        newstate.transition[i][j].push_back(array[k] + 1 + var1.states);
                    }
                }
            }
            newstate.transition[var1.states][26].push_back(var1.states + var2.states + 1);
            newstate.transition[var1.states + var2.states][26].push_back(var1.states + var2.states + 1);
            stack.push_back(newstate);
        }
        else if (str[c] == '*')
        {
            NFA var1 = stack.back();
            stack.pop_back();

            NFA newstate(2 + var1.states);
            newstate.transition[0][26].push_back(1);
            newstate.transition[0][26].push_back(var1.states + 1);
            newstate.transition[var1.states][26].push_back(1);
            newstate.transition[var1.states][26].push_back(var1.states + 1);

            for (int i = 1; i <= var1.states; i++)
            {
                for (int j = 0; j < 27; j++)
                {
                    vector<int> array = var1.transition[i - 1][j];
                    for (int k = 0; k < array.size(); k++)
                    {
                        newstate.transition[i][j].push_back(array[k] + 1);
                    }
                }
            }
            stack.push_back(newstate);
        }
        else if (str[c] == '/')
        {
            NFA var2 = stack.back();
            stack.pop_back();
            NFA var1 = stack.back();
            stack.pop_back();

            NFA newstate(var1.states + var2.states - 1);
            //  newstate.states = 2 + var1.states + var2.states;

            for (int i = 0; i < var1.states; i++)
            {
                for (int j = 0; j < 27; j++)
                {
                    vector<int> array = var1.transition[i][j];
                    for (int k = 0; k < array.size(); k++)
                    {
                        newstate.transition[i][j].push_back(array[k]);
                    }
                }
            }
            for (int i = var1.states - 1; i < (newstate.states - 1); i++)
            {
                for (int j = 0; j < 27; j++)
                {
                    vector<int> array = var2.transition[i + 1 - var1.states][j];
                    for (int k = 0; k < array.size(); k++)
                    {
                        newstate.transition[i][j].push_back(array[k] + var1.states - 1);
                    }
                }
            }
            stack.push_back(newstate);
        }
    }
    return stack.back();
}

string convert(string str)
{
    string postfix = "";
    vector<char> stack;
    for (int i = 0; i < str.length(); i++)
    {
        if (isalpha(str[i]) > 0)
        {
            postfix = postfix + str[i];
            continue;
        }

        else if (str[i] == '(')
        {
            stack.push_back(str[i]);
        }

        else if (str[i] == ')')
        {
            while (stack.back() != '(')
            {
                postfix = postfix + stack.back();
                stack.pop_back();
            }
            stack.pop_back();
        }

        else
        {
            if (priority(str[i]) <= priority(stack.back()))
            {
                while (priority(str[i]) <= priority(stack.back()))
                {
                    postfix = postfix + stack.back();
                    stack.pop_back();
                }
            }
            stack.push_back(str[i]);
        }
    }
    cout << postfix << endl;
    return postfix;
}

int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
    string input;
    cin >> input;
    // system("cls");
    int length = input.length();
    for (int i = 0; i < (input.length() - 1); i++)
    {
        if (isalpha(input[i]) > 0 && isalpha(input[i + 1]) > 0)
        {
            input.insert(i + 1, "/");
            // cout<<"A"<<endl;
        }

        else if (input[i] == ')' && isalpha(input[i + 1]) > 0)
        {
            input.insert(i + 1, "/");
            // cout<<"B"<<endl;
        }

        else if (isalpha(input[i]) > 0 && input[i + 1] == '(')
        {
            input.insert(i + 1, "/");
            // cout<<"C"<<endl;
        }

        else if (input[i] == '*' && input[i + 1] == '(')
        {
            input.insert(i + 1, "/");
            // cout<<"D"<<endl;
        }

        else if (input[i] == '*' && isalpha(input[i + 1]) > 0)
        {
            input.insert(i + 1, "/");
            // cout<<"E"<<endl;
        }

        else if (input[i] == ')' && input[i + 1] == '(')
        {
            input.insert(i + 1, "/");
            // cout<<"F"<<endl;
        }
    }

    input = '(' + input + ')';
    // cout << input << endl;
    auto ans = Create(convert(input));
    // int count = 0;
    // cout << "#states\n";
    // for (int i = 0; i < ans.states; i++)
    // {
    //     cout << i << endl;
    // }
    // cout << "#initial\n0\n#accepting\n" << ans.states - 1 << endl;
    // cout << "#alphabet\na\nb\nc\n";
    // cout << "#transitions\n";
    int count = 0;
    for (int i = 0; i < ans.states; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (auto x : ans.transition[i][j])
            {
                count++;
            }
        }
    }

    cout<<ans.states<<" "<<count<<" "<<ans.states-1<<endl;
    for(int i = 0; i<ans.states; i++)
    {
        cout<<i<<" ";
    }
    cout<<endl;

    for (int i = 0; i < ans.states; i++)
    {
        for (int j = 0; j < 27; j++)
        {
            for (auto x : ans.transition[i][j])
            {
                if (j != 26)
                    cout << i << " " << (char)('a' + j) << " " << x << endl;
                else
                    cout << i << " E " << x << endl;
            }
        }
    }

    return 0;
}
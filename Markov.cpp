#include "stdafx.h"
#include "Markov.h"
#include <vector>
using std::string;
using std::map;
using std::vector;

MarkovTextModel::MarkovNode::MarkovNode()
{
    Ch = "";
    Count = 1;
    FollowCount = 0;
}

MarkovTextModel::MarkovNode::MarkovNode(const string& c)
{
    Ch = c;
    Count = 1;
    FollowCount = 0;
}

MarkovTextModel::MarkovNode* MarkovTextModel::MarkovNode::AddChild(const string& c)
{
    FollowCount += 1;
    auto& child = Children.find(c);
    if (child != Children.end())
    {
        child->second.Count += 1;
        return &child->second;
    }
    else
    {
        return &(Children[c] = MarkovNode(c));
    }
}

const string emptystr;
const string& SelectChildChar(const MarkovTextModel::MarkovNode& node)
{
    // Generate a random number in the range 0..(node.Count-1)
    int rnd = rand() % node.FollowCount;

    // Go through the children to select the node
    int cnt = 0;
    for (auto& iter : node.Children)
    {
        cnt += iter.second.Count;
        if (cnt > rnd)
        {
            return iter.first;
        }
    }
    return emptystr;
}

MarkovTextModel::MarkovTextModel(int order)
{
    ModelOrder = order;
}

void MarkovTextModel::AddString(const string& s)
{

    // Construct the string that will be added.
    vector<string> arr;
    for (auto i = 0; i < ModelOrder; ++i)
        arr.push_back(StartChar);
    size_t iter = 0;
    while (true)
    {
        auto next = s.find(' ', iter);
        arr.push_back(s.substr(iter, next - iter));
        if (next == string::npos)
            break;
        iter = next + 1;
    }
    for (auto i = 0; i < ModelOrder; ++i)
        arr.push_back(StopChar);
    // Naive method
    for (int iStart = 0; iStart < arr.size(); iStart++)
    {
        // Get the order 0 node
        MarkovNode* parent = Root.AddChild(arr[iStart]);

        // Now add N-grams starting with this node
        int i = 1;
        while (i <= ModelOrder && i + iStart < arr.size())
        {
            parent = parent->AddChild(arr[iStart + i]);
            i += 1;
        }
    }
}

void MarkovTextModel::Clear()
{
    Root = MarkovNode(StartChar);
}

string MarkovTextModel::Generate()
{
    vector<string> rslt;
    for (int i = 0; i < ModelOrder; i++)
        rslt.push_back(StartChar);
    int iStart = 0;
    string ch;
    do
    {
        MarkovNode node = Root.Children[rslt[iStart]];
        for (int i = 1; i < ModelOrder; i++)
            node = node.Children[rslt[i + iStart]];
        ch = SelectChildChar(node);
        if (ch != StopChar)
            rslt.push_back(ch);
        iStart += 1;
    } while (ch != StopChar);

    string val;
    for (int i = ModelOrder; i < rslt.size(); ++i)
    {
        if (i > ModelOrder)
            val += ' ';
        val += rslt[i];
    }
    return val;
}

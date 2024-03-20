#pragma once
#include <string>
#include <map>

class MarkovTextModel
{
public:
    class MarkovNode
    {
    public:
        std::string Ch;
        int Count;
        int FollowCount;
        std::map<std::string, MarkovNode> Children;

        MarkovNode();
        MarkovNode(const std::string& c);

        MarkovNode* AddChild(const std::string& c);
    };

private:
    const std::string StartChar = "\ufffe";
    const std::string StopChar = "\uffff";

    MarkovNode Root = MarkovNode(StartChar);
    int ModelOrder;

public:
    MarkovTextModel(int order);

    void AddString(const std::string& s);

    void Clear();

    std::string Generate();
};

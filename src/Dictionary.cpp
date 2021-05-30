#include "Dictionary.hpp"
#include "unistd.h"

Dictionary::Dictionary()
{ 
    for (int i = 0; i < 256; i++)
    {
        dict_elem elem;

        elem.addition = i;
        elem.position = i;
        elem.parent = -1;
        additions.push_back(elem);
    }
}

void Dictionary::insertAddition(char addition)
{
    dict_elem elem;
    elem.addition = addition;
    elem.position = additions.size();
    elem.parent = additions[current].position;
    additions[current].children.push_back(elem.position);
    additions.push_back(elem);
}

bool Dictionary::existsAddidion(char addidion)
{
    if (current == -1)
    {
        current = (unsigned char) addidion;
        return true;
    }
    for (int i : additions[current].children)
    {
        if (additions[i].addition == addidion)
        {
            current = i;
            return true;
        }
    }
    insertAddition(addidion);
    return false;
}

int     Dictionary::getLastAddition()
{
    int last = additions[current].position;
    current = -1;
    return last;
}

int     Dictionary::writeChain(int elem, char *buff, int position)
{
    current = elem & 0xffffff;
    int addition = (elem & 0xff000000) >> 24;
    insertAddition(addition);
    while (current != -1) {
        chain_buff.push_back(additions[current].addition);
        current = additions[current].parent;
    }
    std::vector<char>::reverse_iterator iter = chain_buff.rbegin();
    int i = 0;
    for (; i < chain_buff.size(); ++i)
    {
        buff[position + i] = *iter;
        ++iter;
    }
    buff[position + i] = addition;
    ++i;
    chain_buff.clear();
    return i;
}

void    Dictionary::clear()
{
    additions.clear();
    for (int i = 0; i < 256; i++)
    {
        dict_elem elem;
        elem.addition = i;
        elem.position = i;
        elem.parent = -1;
        additions.push_back(elem);
    }
    current = -1;
}
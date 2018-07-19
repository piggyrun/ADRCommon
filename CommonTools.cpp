#include "CommonTools.h"

string GetFullPath(const char* dir, const char* filename)
{
    ostringstream oss;
    oss << dir;
    if (dir[strlen(dir)-1] != '/')
    {
        oss << '/';
    }
    oss << filename;
    return oss.str();
}

md5_long_32 GetMD5(const uint32_t *termidList, uint32_t termlen)
{
    ostringstream oss;
    for (uint32_t i = 0; i < termlen; i++)
    {
        oss << (int32_t)termidList[i] << ",";
    }
    //printf("for md5 : %s\n", oss.str().c_str());
    return getSign32(oss.str().c_str(), -1);
}
void SplitString(const string &src, char delimitor, vector<string> &dest, bool ignoreEmpty)
{
    dest.clear();
    string::size_type currentPos = 0;
    string::size_type findPos = src.find(delimitor);

    while(findPos != string::npos)
    {        
        if (findPos != currentPos)
        {
            dest.push_back(src.substr(currentPos, findPos - currentPos));
        }
        else if (!ignoreEmpty)
        {
            dest.push_back("");
        }
        currentPos = findPos + 1;
        findPos = src.find(delimitor, currentPos);
    }
    
    if (currentPos != src.size())
    {
        dest.push_back(src.substr(currentPos));
    }
    else if (!ignoreEmpty)
    {
        dest.push_back("");
    }
}

void SplitString(const string &src, const string &delimitor, vector<string> &dest, bool ignoreEmpty)
{
    dest.clear();
    string::size_type currentPos = 0;
    string::size_type findPos = src.find(delimitor);

    string::size_type deliSize = delimitor.size();

    while(findPos != string::npos)
    {        
        if (findPos != currentPos)
        {
            dest.push_back(src.substr(currentPos, findPos - currentPos));
        }
        else if (!ignoreEmpty)
        {
            dest.push_back("");
        }
        currentPos = findPos + deliSize;
        findPos = src.find(delimitor, currentPos);
    }
    
    if (currentPos != src.size())
    {
        dest.push_back(src.substr(currentPos));
    }
    else if (!ignoreEmpty)
    {
        dest.push_back("");
    }
}

int SplitString(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size)
{
	char stoken[2];
	strcpy(stoken,split);
	char *p;   
	uint32_t i = 0;   
	p = strtok((char*)str,stoken);   
	if(p == NULL)   
		return -1;   
	while(p != NULL && i < MAX_FIELD_NUM - 1)  //最多读入MAX_FIELD_NUM个域，多余丢弃  
	{   
		if(strlen(p) > MAX_STRING_LENGTH)	    //如果一个域过长，则截断	
			p[MAX_STRING_LENGTH-1] = '\0';
		strcpy(dest[i++], p);   
		p = strtok(NULL, stoken);   
	}   
	if(i > 0)   
	{   
		*size = --i;   
		return 0;   
	}   
	else   
		return -1;    
}

int parse_str(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size)
{
    return SplitString(str, split, dest, size);   
}
int parse_str(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], uint32_t * size)
{
    return SplitString(str, split, dest, (int *)size);   
}
int parse_string(const char * str, const char * split, char dest[MAX_FIELD_NUM][MAX_STRING_LENGTH], int * size)
{
    return SplitString(str, split, dest, size);   
}

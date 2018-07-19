#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <time.h>
#include <ext/hash_map>
#include <openssl/md5.h>

using namespace std;

vector<string> logdirs;
string outputDir;

struct SCtrInfo
{
    SCtrInfo() 
    {
        pv = 0;
        click = 0;
    }
    float pv;
    uint32_t click; 
    float ctr;
};

typedef __gnu_cxx::hash_map<uint64_t, SCtrInfo> t_uint_info_map;

uint64_t get_md5_64(const char * str)
{
	if(str == NULL)
	{
		return 0;
	}
	uint8_t md5buf[16];
	MD5((uint8_t*)str, strlen(str), md5buf);
	return *(uint64_t *)md5buf;
}

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

uint32_t ParseIp(const string &ipstring)
{
    vector<string> addr;
    SplitString(ipstring, '.', addr, false);
    if (addr.size() != 4 )
    {
        return 0;
    }
    return (atoi(addr[0].c_str()) << 24) + (atoi(addr[1].c_str()) << 16) + (atoi(addr[2].c_str()) << 8) + atoi(addr[3].c_str());   
}

void OutputCtrInfoInt(map<uint32_t, SCtrInfo> &detailctr, const string &outputdir, const string &filename)
{
    string outputPath = GetFullPath(outputdir.c_str(), filename.c_str());
    cout << outputPath << endl;
    ofstream ofs(outputPath.c_str());
    if (ofs == NULL)
    {
        return;
    }

    for (map<uint32_t, SCtrInfo>::iterator it = detailctr.begin(); it != detailctr.end(); ++it)
    {
        ofs << it->first << "\t" << int(it->second.pv) << "\t" << it->second.click << "\t" << float(it->second.click) / it->second.pv << endl;
    }

    ofs.close();
}

void OutputCtrInfoStr(map<string, SCtrInfo> &detailctr, const string &outputdir, const string &filename, uint32_t minPv, uint32_t minClick)
{
    SCtrInfo otherctr; 
    otherctr.pv = 0;
    otherctr.click = 0;
    for (map<string, SCtrInfo>::iterator it = detailctr.begin(); it != detailctr.end(); ++it)
    {
        if (it->second.pv < minPv && it->second.click < minClick)
        {
            otherctr.pv += it->second.pv;
            otherctr.click += it->second.click;
        }
    }

    string outputPath = GetFullPath(outputdir.c_str(), filename.c_str());
    cout << outputPath << endl;
    ofstream ofs(outputPath.c_str());
    if (ofs == NULL)
    {
        return;
    }

    if (otherctr.pv != 0)
    {
        ofs << "!!!!!!\t" << int(otherctr.pv) << "\t" << otherctr.click << "\t" << float(otherctr.click) / otherctr.pv << endl;
    }


    for (map<string, SCtrInfo>::iterator it = detailctr.begin(); it != detailctr.end(); ++it)
    {
        if (it->second.pv >= minPv || it->second.click >= minClick)
        {
            ofs << it->first << "\t" << int(it->second.pv) << "\t" << it->second.click << "\t" << float(it->second.click) / it->second.pv << endl;
        }
    }

    ofs.close();
}



void MergeCtrInfo(const string &filename, uint32_t minPv, uint32_t minClick)
{
    map<string, SCtrInfo> key2ctr;

    for (size_t i = 0; i < logdirs.size(); i++)
    {
        string path = GetFullPath(logdirs[i].c_str(), filename.c_str());
        ifstream ifs(path.c_str());
        string line;
        while (getline(ifs, line))
        {
            vector<string> elements;
            SplitString(line, '\t', elements, false);
            if (elements.size() != 4)
            {
                continue;
            }
            key2ctr[elements[0]].pv += atof(elements[1].c_str());
            key2ctr[elements[0]].click += atoi(elements[2].c_str());
        }
        ifs.close();        
    }           
    OutputCtrInfoStr(key2ctr, outputDir, filename, minPv, minClick);        
}

void MergeCtrInfo2(const string &filename, uint32_t minPv, uint32_t minClick) // using md5, cut memory usage
{
	string tmp_file("record_list.tmp"); // 缓存中间文件
	fstream ftmp(tmp_file.c_str(), ios_base::in | ios_base::out | ios_base::trunc);
	if (!ftmp)
	{
		cerr << tmp_file << " failed\n";
		return;
	}
	vector<uint64_t> md5_list;
	t_uint_info_map merge_map;
    for (size_t i = 0; i < logdirs.size(); i++)
    {
        string path = GetFullPath(logdirs[i].c_str(), filename.c_str());
        ifstream ifs(path.c_str());
		if (!ifs)
		{
			cerr << path << " failed\n";
			return;
		}
        string line;
        while (getline(ifs, line))
        {
            vector<string> elements;
            SplitString(line, '\t', elements, false);
            if (elements.size() != 4)
            {
                continue;
            }
			uint64_t key = get_md5_64(elements[0].c_str());
			if (merge_map.find(key) == merge_map.end())
			{
				ftmp << elements[0] << endl;
				md5_list.push_back(key);
			}
            merge_map[key].pv += atof(elements[1].c_str());
            merge_map[key].click += atoi(elements[2].c_str());
        }
        ifs.close();        
    }           

	t_uint_info_map::iterator it;
    SCtrInfo otherctr; 
    otherctr.pv = 0;
    otherctr.click = 0;
    for (it = merge_map.begin(); it != merge_map.end(); ++it)
    {
        if (it->second.pv < minPv && it->second.click < minClick)
        {
            otherctr.pv += it->second.pv;
            otherctr.click += it->second.click;
        }
    }

    string outputPath = GetFullPath(outputDir.c_str(), filename.c_str());
    cout << outputPath << endl;
    ofstream ofs(outputPath.c_str());
    if (ofs == NULL)
    {
        return;
    }

    if (otherctr.pv != 0)
    {
        ofs << "!!!!!!\t" << int(otherctr.pv) << "\t" << otherctr.click << "\t" << float(otherctr.click) / otherctr.pv << endl;
    }

	ftmp.seekg(0, ios::beg);
	ftmp.clear();
	string record_line;
	for (size_t i=0; i<md5_list.size(); i++)
	{
		it = merge_map.find(md5_list[i]);
		if (!(getline(ftmp, record_line) && it != merge_map.end()))
		{
			cerr << "missing record!!! ERROR\n";
			continue;
		}
        if (it->second.pv >= minPv || it->second.click >= minClick)
        {
            ofs << record_line << "\t" << int(it->second.pv) << "\t" << it->second.click << "\t" << float(it->second.click) / it->second.pv << endl;
        }
    }
    ofs.close();
	ftmp.close();
	remove(tmp_file.c_str());
}

int main(int argc,char** argv)
{
    if (argc != 5)
    {
		cerr << "Usage: " << argv[0] << " file_config date_list daily_stat_path out_dir\n";
        return 0;
    }

    string logFileList = argv[1];
    string logDirList = argv[2];
    string logRoot = argv[3];
    outputDir = argv[4];


    ifstream ifs(logDirList.c_str());
    string line;

    while (getline(ifs, line))
    {
        logdirs.push_back(GetFullPath(logRoot.c_str(), line.c_str()));
    }
    ifs.close();

    ifstream ifs2(logFileList.c_str());

    while (getline(ifs2, line))
    {
        istringstream iss(line);
        string path;
        uint32_t minPv, minClick;
        iss >> path >> minPv >> minClick;
        MergeCtrInfo2(path, minPv, minClick);
    }
    ifs2.close();


    
    //string generalInformationFile = argv[3];

    return 0;
}

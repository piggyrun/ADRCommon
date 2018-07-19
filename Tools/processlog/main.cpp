#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <time.h>
#include "Cernet.h"
using namespace std;
using namespace adr;

#define CountFeature(mapname, key, isclick) \
    mapname[key].pv ++; \
    if (isclick == 1) { mapname[key].click ++; }

#define CountFeature2(mapname, key, isclick, weight) \
    mapname[key].pv += weight; \
    if (isclick == 1) { mapname[key].click ++; }


CCernet ct;
set<uint32_t> netb;

const int query_type_index[] = {13, 18, 19, 29, 30, 31, 32, 34, 35, 37, 38, 39, 40};
const size_t qtype_num = sizeof(query_type_index)/sizeof(int);

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

struct SLogInfo
{
    string      query;
    uint32_t    adid;
    uint32_t    accid;
    uint32_t    adtype;
    string      pid;
    uint32_t    adpos;
    uint32_t    ip;
    uint32_t    ipC;
    uint32_t    ipB;
    uint32_t    ipA;

    uint32_t    locale;
    string      suid;
    string      yyid;
    long        pvtime;
    long        clicktime;
    uint32_t    adcount;
    uint32_t    isHint;
    uint32_t    isSuggestion;
    uint32_t    position;

    uint32_t    isClick;

    uint32_t    querylen;
    string      adid_adpos;
    string      accid_adpos;
    string      query_adid;
    string      query_accid;
    string      locale_adtype;

    uint32_t    pvhour;
    string      pvhour_adtype;
    string      accid_pvhour;
    string      accid_locale;
    uint32_t    iptype;

	// web part
	float rank;
	string doc_url;
	string doc_title;
	uint64_t doc_flag;

	string site;
	string site_pos;
	string site_rank;
	vector<string> querytype_site_list;
	string query_site;
	string query_url;
};

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

string GetSite(const string &url)
{
	string site;
	size_t start = url.find("://");
	start += 3;
	if (start >= url.size())
		return site;
	size_t end = url.find("/", start);
	site = url.substr(start, end-start);
	return site;
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
        ofs << it->first << "\t" << it->second.pv << "\t" << it->second.click << "\t" << float(it->second.click) / it->second.pv << endl;
    }

    ofs.close();
}

void OutputCtrInfoStr(map<string, SCtrInfo> &detailctr, const string &outputdir, const string &filename)
{
    string outputPath = GetFullPath(outputdir.c_str(), filename.c_str());
    cout << outputPath << endl;
    ofstream ofs(outputPath.c_str());
    if (ofs == NULL)
    {
        return;
    }

    for (map<string, SCtrInfo>::iterator it = detailctr.begin(); it != detailctr.end(); ++it)
    {
        ofs << it->first << "\t" << it->second.pv << "\t" << it->second.click << "\t" << float(it->second.click) / it->second.pv << endl;
    }

    ofs.close();
}



uint32_t ParseLogLine(const string &line, SLogInfo &loginfo)
{
    vector<string> elements;
    SplitString(line, '', elements, false);

    if (elements.size() < 20)
    {
        return -1;
    }
        
    loginfo.isClick     = atoi(elements[0].c_str());
    loginfo.query       = elements[1];
    //loginfo.adid        = atoi(elements[1].c_str());
    //loginfo.accid       = atoi(elements[2].c_str());
    //loginfo.adtype      = atoi(elements[3].c_str());
    loginfo.pid         = elements[2];
    //loginfo.adpos       = atoi(elements[5].c_str());
    loginfo.ip          = ParseIp(elements[3].c_str());
    loginfo.ipC         = loginfo.ip >> 8;
    loginfo.ipB         = loginfo.ip >> 16;
    loginfo.ipA         = loginfo.ip >> 24;

    //loginfo.locale      = atoi(elements[7].c_str());
    loginfo.suid        = elements[4];
    //loginfo.yyid        = elements[9];

    loginfo.pvtime      = atoi(elements[5].c_str());   
    loginfo.clicktime   = atoi(elements[6].c_str());
    //loginfo.adcount     = atoi(elements[12].c_str());
    //loginfo.isHint      = atoi(elements[13].c_str());
    //loginfo.isSuggestion= atoi(elements[14].c_str());
    loginfo.position    = atoi(elements[7].c_str());

    loginfo.querylen    = loginfo.query.size();

	loginfo.rank = atof(elements[17].c_str()) * 100.0f/2000;
	loginfo.doc_url = elements[8];
	loginfo.site = GetSite(loginfo.doc_url);
	loginfo.doc_title = elements[9];
	loginfo.doc_flag = atol(elements[19].c_str());
    
    ostringstream oss; 
	/*
    oss << loginfo.adid  << '\3' << loginfo.adpos;
    loginfo.adid_adpos = oss.str();

    oss.str("");
    oss << loginfo.accid << '\3' << loginfo.adpos;
    loginfo.accid_adpos = oss.str();

    oss.str("");
    oss << loginfo.query << '\3' << loginfo.adid;
    loginfo.query_adid = oss.str();

    oss.str("");
    oss << loginfo.query << '\3' << loginfo.accid;
    loginfo.query_accid = oss.str();

    oss.str("");
    oss << loginfo.locale << '\3' << loginfo.adtype;
    loginfo.locale_adtype = oss.str();
	*/

    oss.str("");
    oss << loginfo.site << '\3' << loginfo.position;
    loginfo.site_pos = oss.str();

	int rank_level = 0;
	if (loginfo.rank < 15)
		rank_level = 0;
	else if (loginfo.rank > 25)
		rank_level = 2;
	else
		rank_level = 1;
    oss.str("");
    oss << loginfo.site << '\3' << rank_level << '\3' << loginfo.position;
    loginfo.site_rank = oss.str();

	loginfo.querytype_site_list.clear();
	for (size_t i=0; i<qtype_num; i++)
	{
		if (((loginfo.doc_flag>>query_type_index[i])&1) == 1)
		{
			oss.str("");
			oss << query_type_index[i] << '\3' << loginfo.site << '\3' << loginfo.position;
			loginfo.querytype_site_list.push_back(oss.str());
			break; // 只取遇到的第一个类别
		}
	}

    oss.str("");
    oss << loginfo.query << '\3' << loginfo.site << '\3' << loginfo.position;
    loginfo.query_site = oss.str();

    oss.str("");
    oss << loginfo.query << '\3' << loginfo.doc_url << '\3' << loginfo.position;
    loginfo.query_url = oss.str();

    struct tm *pvtime;
    pvtime = localtime(&loginfo.pvtime);
    loginfo.pvhour = pvtime->tm_hour;

	/*
    oss.str("");
    oss << loginfo.pvhour << '\3' << loginfo.adtype;
    loginfo.pvhour_adtype = oss.str();
    
    loginfo.iptype = 0;
    //if (ct.IsCernetIP(loginfo.ip))
    //{
    //    loginfo.iptype = 1;
    //}
    //if (netb.find(loginfo.ip) != netb.end())
    //{
    //    loginfo.iptype = 2;
    //}

    oss.str("");
    oss << loginfo.accid << '\3' << loginfo.locale;
    loginfo.accid_locale = oss.str();

    oss.str("");
    oss << loginfo.accid << '\3' << loginfo.pvhour;
    loginfo.accid_pvhour = oss.str();
	*/

    return 0;
}

void ParseGeneralInformation(const string &inputFileName, const string &outputdir, uint32_t type)
{
    ifstream ifs(inputFileName.c_str());
    string line;

    map<string, SCtrInfo> site2ctr;
    map<string, SCtrInfo> site_rank2ctr;
    //map<string, SCtrInfo> query2ctr;
    //map<uint32_t, SCtrInfo> querylen2ctr;
    map<string, SCtrInfo> querytype_site2ctr;
    map<string, SCtrInfo> query_site2ctr;
    map<string, SCtrInfo> query_url2ctr;

    uint32_t count = 0;
    while (getline(ifs, line))
    {
        count++;
        if (count % 100000 == 0)
        {
            cout << count <<endl;
        }
        SLogInfo loginfo; 
        ParseLogLine(line, loginfo);        

		string key;
        CountFeature(site2ctr, loginfo.site_pos, loginfo.isClick);
        CountFeature(site_rank2ctr, loginfo.site_rank, loginfo.isClick);
        //CountFeature(query2ctr, loginfo.query, loginfo.isClick);
        //CountFeature(querylen2ctr, loginfo.querylen, loginfo.isClick);
		for (size_t ii=0; ii<loginfo.querytype_site_list.size(); ii++)
		{
			CountFeature(querytype_site2ctr, loginfo.querytype_site_list[ii], loginfo.isClick);
		}
        CountFeature(query_site2ctr, loginfo.query_site, loginfo.isClick);
        CountFeature(query_url2ctr, loginfo.query_url, loginfo.isClick);
    }
    ifs.close();

    OutputCtrInfoStr(site2ctr, outputdir, "site");
    OutputCtrInfoStr(site_rank2ctr, outputdir, "site_rel2ctr");
    //OutputCtrInfoStr(query2ctr, outputdir, "query");
    //OutputCtrInfoInt(querylen2ctr, outputdir, "querylen");
    OutputCtrInfoStr(querytype_site2ctr, outputdir, "qtype_site");
    OutputCtrInfoStr(query_site2ctr, outputdir, "query_site");
    OutputCtrInfoStr(query_url2ctr, outputdir, "query_url");
}

void ParseGeneralInformation2(const string &inputFileName, const string &outputdir, uint32_t type)
{
    ifstream ifs(inputFileName.c_str());
    string line;

    map<uint32_t, SCtrInfo> adid2ctr;
    map<uint32_t, SCtrInfo> accid2ctr;
    map<uint32_t, SCtrInfo> adtype2ctr;
    map<string, SCtrInfo> pid2ctr;
    map<string, SCtrInfo> query2ctr;
    map<uint32_t, SCtrInfo> ip2ctr;
    map<uint32_t, SCtrInfo> ipC2ctr;
    map<uint32_t, SCtrInfo> ipB2ctr;
    map<uint32_t, SCtrInfo> ipA2ctr;
    map<uint32_t, SCtrInfo> locale2ctr;
    map<uint32_t, SCtrInfo> time2ctr;
    map<uint32_t, SCtrInfo> hint2ctr;
    map<uint32_t, SCtrInfo> suggestion2ctr;
    map<uint32_t, SCtrInfo> adcount2ctr;
    map<string, SCtrInfo> suid2ctr;
    map<string, SCtrInfo> yyid2ctr;

    map<uint32_t, SCtrInfo> querylen2ctr;
    map<string, SCtrInfo> adid_adpos2ctr;    
    map<string, SCtrInfo> accid_adpos2ctr;
    map<string, SCtrInfo> query_adid2ctr;
    map<string, SCtrInfo> query_accid2ctr;
    map<string, SCtrInfo> locale_adtype2ctr;

    map<uint32_t, SCtrInfo> pvhour2ctr;
    map<string, SCtrInfo> pvhour_adtype2ctr;
    map<uint32_t, SCtrInfo> iptype2ctr;

    map<uint32_t, SCtrInfo> adid_left2ctr;
    map<uint32_t, SCtrInfo> adid_gnd2ctr;
    map<uint32_t, SCtrInfo> accid_left2ctr;
    map<uint32_t, SCtrInfo> accid_gnd2ctr;

    map<string, SCtrInfo> accid_pvhour2ctr;
    map<string, SCtrInfo> accid_locale2ctr;

    uint32_t count = 0;
    while (getline(ifs, line))
    {
        count++;
        if (count % 100000 == 0)
        {
            cout << count <<endl;
        }
        SLogInfo loginfo; 
        ParseLogLine(line, loginfo);        

        if (loginfo.adpos == 1)
        {
            if (loginfo.position == 1)
            {
                CountFeature(query_adid2ctr, loginfo.query_adid, loginfo.isClick);
            }
            else if (loginfo.position == 2)
            {
                CountFeature2(query_adid2ctr, loginfo.query_adid, loginfo.isClick, 0.7);
            }
            else if (loginfo.position == 3)
            {
                CountFeature2(query_adid2ctr, loginfo.query_adid, loginfo.isClick, 0.5);
            }            
        }

        if (loginfo.position != 1)
        {
            continue;
        }
        CountFeature(accid2ctr, loginfo.accid, loginfo.isClick);
        CountFeature(adid2ctr, loginfo.adid, loginfo.isClick);
        CountFeature(adid_adpos2ctr, loginfo.adid_adpos, loginfo.isClick);
        CountFeature(accid_adpos2ctr, loginfo.accid_adpos, loginfo.isClick);       

        if (loginfo.adpos == 0)
        {
            CountFeature(adid_left2ctr, loginfo.adid, loginfo.isClick);
            CountFeature(accid_left2ctr, loginfo.accid, loginfo.isClick);
        }
        else
        {
            CountFeature(adid_gnd2ctr, loginfo.adid, loginfo.isClick);
            CountFeature(accid_gnd2ctr, loginfo.accid, loginfo.isClick);
        }


        if (loginfo.adpos != type)
        {
            continue;
        }




        CountFeature(locale2ctr, loginfo.locale, loginfo.isClick);
        CountFeature(hint2ctr, loginfo.isHint, loginfo.isClick);
        CountFeature(adtype2ctr, loginfo.adtype, loginfo.isClick);
        CountFeature(suggestion2ctr, loginfo.isSuggestion, loginfo.isClick);
        CountFeature(adcount2ctr, loginfo.adcount, loginfo.isClick);
        CountFeature(ip2ctr, loginfo.ip, loginfo.isClick);
        CountFeature(ipC2ctr, loginfo.ipC, loginfo.isClick);
        CountFeature(ipB2ctr, loginfo.ipB, loginfo.isClick);
        CountFeature(ipA2ctr, loginfo.ipA, loginfo.isClick);

        CountFeature(query2ctr, loginfo.query, loginfo.isClick);
        CountFeature(pid2ctr, loginfo.pid, loginfo.isClick);
        CountFeature(suid2ctr, loginfo.suid, loginfo.isClick);
        CountFeature(yyid2ctr, loginfo.yyid, loginfo.isClick);

        CountFeature(querylen2ctr, loginfo.querylen, loginfo.isClick);
        
        CountFeature(query_accid2ctr, loginfo.query_accid, loginfo.isClick);
        CountFeature(locale_adtype2ctr, loginfo.locale_adtype, loginfo.isClick);

        CountFeature(pvhour2ctr, loginfo.pvhour, loginfo.isClick);
        CountFeature(pvhour_adtype2ctr, loginfo.pvhour_adtype, loginfo.isClick);
        CountFeature(iptype2ctr, loginfo.iptype, loginfo.isClick);

        CountFeature(accid_pvhour2ctr, loginfo.accid_pvhour, loginfo.isClick);
        CountFeature(accid_locale2ctr, loginfo.accid_locale, loginfo.isClick);


    }
    ifs.close();

    OutputCtrInfoInt(hint2ctr, outputdir, "hint");
    OutputCtrInfoInt(locale2ctr, outputdir, "locale");
    OutputCtrInfoInt(accid2ctr, outputdir, "accid");
    OutputCtrInfoInt(adid2ctr, outputdir, "adid");
    OutputCtrInfoInt(adtype2ctr, outputdir, "adtype");
    OutputCtrInfoInt(suggestion2ctr, outputdir, "suggestion");
    OutputCtrInfoInt(adcount2ctr, outputdir, "adcount");

    OutputCtrInfoInt(ip2ctr, outputdir, "ip");
    OutputCtrInfoInt(ipC2ctr, outputdir, "ipc");
    OutputCtrInfoInt(ipB2ctr, outputdir, "ipb");
    OutputCtrInfoInt(ipA2ctr, outputdir, "ipa");

    OutputCtrInfoStr(query2ctr, outputdir, "query");
    OutputCtrInfoStr(pid2ctr, outputdir, "pid");
    OutputCtrInfoStr(suid2ctr, outputdir, "suid");
    OutputCtrInfoStr(yyid2ctr, outputdir, "yyid");

    OutputCtrInfoInt(querylen2ctr, outputdir, "querylen");
    OutputCtrInfoStr(adid_adpos2ctr, outputdir, "adid_adpos");
    OutputCtrInfoStr(accid_adpos2ctr, outputdir, "accid_adpos");
    OutputCtrInfoStr(query_adid2ctr, outputdir, "query_adid");
    OutputCtrInfoStr(query_accid2ctr, outputdir, "query_accid");
    OutputCtrInfoStr(locale_adtype2ctr, outputdir, "locale_adtype");

    OutputCtrInfoInt(pvhour2ctr, outputdir, "pvhour");
    OutputCtrInfoStr(pvhour_adtype2ctr, outputdir, "pvhour_adtype");
    OutputCtrInfoInt(iptype2ctr, outputdir, "iptype");

    OutputCtrInfoInt(adid_left2ctr, outputdir, "adid_left");
    OutputCtrInfoInt(adid_gnd2ctr, outputdir, "adid_gnd");
    OutputCtrInfoInt(accid_left2ctr, outputdir, "accid_left");
    OutputCtrInfoInt(accid_gnd2ctr, outputdir, "accid_gnd");

    OutputCtrInfoStr(accid_pvhour2ctr, outputdir, "accid_pvhour");
    OutputCtrInfoStr(accid_locale2ctr, outputdir, "accid_locale");

        
}
void LoadNetb(const string &path)
{
    ifstream ifs(path.c_str());
    string line;
    while (getline(ifs, line))
    {
        istringstream iss(line);
        string ipstr;
        iss >> ipstr;
        uint32_t ip = ParseIp(ipstr);
        netb.insert(ip);
    }
}

int main(int argc,char** argv)
{
    if (argc != 4)
    {
		cerr << "Usage: " << argv[0] << " samples out_dir adtype\n";
        return 0;
    }

    string logFileName = argv[1];
    string outputdir = argv[2];
    uint32_t type = atoi(argv[3]);

    //ct.LoadCernetIP("/search/guoang/regdata/iptable/cernet.ip");
    //LoadNetb("/search/guoang/regdata/iptable/exp_ip.dat");

    ParseGeneralInformation(logFileName, outputdir, type);

    //ct.Release();
    //string generalInformationFile = argv[3];

    return 0;
}

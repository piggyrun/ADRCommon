#include <iostream>
#include <fstream>
#include "Platform/docId/docId.h"

using namespace std;

struct t_site_rank
{
	siteID_t id;
	uint16_t rank;
};

int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " in out\n";
		return 0;
	}
	ifstream in(argv[1], ios::binary);
	ofstream out(argv[2]);
	//ofstream out(argv[2], ios::binary);
	if (!in || !out)
	{
		cerr << "I/O error\n";
		return -1;
	}

	in.seekg (0, ios::end);
	uint32_t length = in.tellg();
	if (length % sizeof(t_site_rank) != 0)
	{
		cerr << "binary size error\n";
		return -1;
	}
	uint32_t site_num = length / sizeof(t_site_rank);
	cout << "site num: " << site_num << endl;

	in.seekg (0, ios::beg);

	t_site_rank site_rank;
	while (in.read((char *)&site_rank, sizeof(t_site_rank)))
	{
		if (site_rank.rank > 1)
 		{
			uint64_t site_id = site_rank.id.id.obs.host_id;
			/*
			char buf[1024];
			sprintf(buf, "%016llx-%016llx\t%llu\t%u\n", site_rank.id.id.value.value_high, site_rank.id.id.value.value_low, site_id, site_rank.rank); 
			out << buf;
			*/
			out << site_id << "\t" << site_rank.rank << endl;
			//out.write((char *)&site_rank, sizeof(t_site_rank));
		}
	}
	in.close();
	out.close();

	return 0;
}

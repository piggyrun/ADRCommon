#ifndef TERM_FEATURE_H
#define TERM_FEATURE_H

#include "BaseFeature.h"

typedef ACE_Hash_Map_Manager_Ex<uint32_t, uint32_t , ACE_Hash< uint32_t >, ACE_Equal_To< uint32_t >, ACE_Null_Mutex> t_uint_uint_map;
typedef t_uint_uint_map t_termid_fnum_map;

class CTermFeature: public CBaseFeature
{
public:    

    CTermFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);

    CTermFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);

    virtual ~CTermFeature() {};

    virtual int GetFeatures(const SQueryInfo &oneCase, vector<IdWeight> &features) const;

    virtual int Initialize(const string &modelPath);

private: 
    virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	void GetTermFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, 
			const map<uint32_t, uint32_t> &kterm_map,
			const map<uint32_t, uint32_t> &tterm_map,
			const map<uint32_t, uint32_t> &qterm_map,
			vector<IdWeight> &feature_vec) const;
    
private:
	int load_termid_map(const char * filename, t_termid_fnum_map &termid_map);
	int load_match_term_map(const char * filename, t_uint_uint_map &match_map);
	int get_match_term_feature(const map<uint32_t, uint32_t> &qterm_map, const map<uint32_t, uint32_t> &term_map, const t_uint_uint_map &match_map, uint32_t &feature_index, string &debug_str) const;
	int get_match_non_term_feature(const map<uint32_t, uint32_t> &qterm_map, const map<uint32_t, uint32_t> &term_map, const t_uint_uint_map &match_map, vector<uint32_t> &feature_index_vec, vector<string> &oss_vec) const;
	/// termid对应的特征下标
	static t_termid_fnum_map key_termid_fnum_map;
	static t_termid_fnum_map title_termid_fnum_map;
	static t_termid_fnum_map query_termid_fnum_map;
	static t_termid_fnum_map query_key_termid_fnum_map;
	static t_termid_fnum_map query_title_termid_fnum_map;
	/// 匹配关系特征下标
	static t_uint_uint_map match_query_title_map;
	static t_uint_uint_map match_non_query_title_map;

};

#endif

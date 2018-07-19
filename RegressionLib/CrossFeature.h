#ifndef CROSS_FEATURE_H_
#define CROSS_FEATURE_H_

#include "BaseFeature.h"
#include "Tree.h"

typedef ACE_Hash_Map_Manager_Ex<uint32_t, uint32_t , ACE_Hash< uint32_t >, ACE_Equal_To< uint32_t >, ACE_Null_Mutex> t_uint_uint_map;
typedef ACE_Hash_Map_Manager_Ex<ACE_CString, uint32_t, ACE_Hash< ACE_CString >, ACE_Equal_To< ACE_CString >, ACE_Null_Mutex> t_string_int_map;

class CCrossFeature: public CBaseFeature
{
	private:
		static t_uint_uint_map match_term_map;
		static t_uint_uint_map query_nonmatch_map;
		static t_uint_uint_map ad_nonmatch_map;
		static Tree feature_tree;

		static uint32_t CATE_FEATURE_BASE;
		static uint32_t CROSS_FEATURE_BASE;
		
		int load_termid_map(const char* filename, t_uint_uint_map& termid_map);
		int load_feature_map(const char* filename, t_string_int_map& feature_map);
		int load_feature_tree(const char* fielname);

	protected:
		virtual int GetFeature(const SFeatureInfo &featureInfo, const SQueryInfo &oneCase, vector<IdWeight> &features) const;

	public:    
		CCrossFeature(bool isOnline, uint32_t implementation = IMPLEMENT_ALL);
		CCrossFeature(bool isOnline, const vector<uint32_t> &useFeatureList, uint32_t implementation = IMPLEMENT_ALL);
		virtual ~CCrossFeature() {};
		virtual int Initialize(const string &modelPath);
};

#endif

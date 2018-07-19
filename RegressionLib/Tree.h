//Tree.h
#ifndef TREE_H_
#define TREE_H_
#include <stdint.h>
#include <iostream>
#include <vector>

const uint32_t DEFAULT_NODE_LENGTH = 200000000;
const uint32_t MAX_CHAR_LENGTH = 1024;

class TermNode
{
	public:
		uint32_t id;
		uint32_t feature;
		bool operator == (const TermNode& node);
		bool operator < (const TermNode& node);
		bool operator > (const TermNode& node);
};

class TreeNode
{
	public:
		uint32_t firstchild;
		uint32_t nextsibing_lastchild;
		uint32_t parent;
		TermNode data;
	public:
		TreeNode ();
		TreeNode (const TermNode& data);
};

class SimpleFeature
{
	public:
		std::vector<uint32_t> feature_vec;
		uint32_t feature_no;

		static bool GreaterSize(const SimpleFeature& a, const SimpleFeature& b)
		{
			return a.feature_vec.size() > b.feature_vec.size();
		}
};

class Tree
{
	protected:
		TreeNode* tree_node_list;
		uint32_t length;
		uint32_t spare_index;

		void initNode(bool withMem = false);
		uint32_t getNewNode();
		virtual int withdrawNode(uint32_t);
	public:
		Tree(uint32_t len=DEFAULT_NODE_LENGTH);
		virtual ~Tree();
		uint32_t insert(uint32_t parent, uint32_t id);
		uint32_t findInParent(uint32_t parent, uint32_t id);
		uint32_t findFromSibing(uint32_t sibing, uint32_t id);
		uint32_t binaryFindFromSibing(uint32_t sibing, uint32_t id);

		int insertFeatureBranch(const std::vector<uint32_t>& feature_list, uint32_t feature_no);
		void generateFeature(const std::vector<uint32_t>& original_feature, std::vector<uint32_t>& feature_list);

		int loadBinary(const char* filename);
		int outputBinary(const char* filename);

		int transferTree();
	private:
		int generateFeature(const std::vector<uint32_t>& original_feature, std::vector<SimpleFeature>& feature_list, uint32_t vec_idx, uint32_t tree_parent_idx, bool hasSpecial);
		bool isSpecial (uint32_t feature_id);
};
#endif

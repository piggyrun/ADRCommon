//Tree.cpp
#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include "Tree.h"
using namespace std;

bool TermNode::operator == (const TermNode& node)
{
	return (this->id == node.id);
}

bool TermNode::operator < (const TermNode& node)
{
	return (this->id < node.id);
}

bool TermNode::operator > (const TermNode& node)
{
	return (this->id > node.id);
}

TreeNode::TreeNode ():firstchild(0),nextsibing_lastchild(0)
{
}

TreeNode::TreeNode (const TermNode& tmp):firstchild(0),nextsibing_lastchild(0),data(tmp)
{
}

Tree::Tree (uint32_t len):length(len)
{
	initNode(true);
}

Tree::~Tree()
{
	delete[] tree_node_list;
}

uint32_t Tree::insert(uint32_t parent, uint32_t id)
{
	uint32_t cur_ptr = tree_node_list[parent].firstchild;
	uint32_t last_ptr = 0;

	uint32_t ret_idx = 0;

	while(cur_ptr != 0 && tree_node_list[cur_ptr].data.id < id)
	{
		last_ptr = cur_ptr;
		cur_ptr = tree_node_list[cur_ptr].nextsibing_lastchild;
	}
	
	if ((cur_ptr != 0) && (tree_node_list[cur_ptr].data.id == id))
	{
		ret_idx = cur_ptr;
	}
	else
	{
		uint32_t new_idx = getNewNode();
		if (new_idx == 0)
		{
			//cerr << "Not Enough nodes!" << endl;
			return 0;
		}
		tree_node_list[new_idx].parent = parent;
		tree_node_list[new_idx].data.id = id;
		tree_node_list[new_idx].data.feature = 0;
		//在中间插入
		if (last_ptr != 0)
		{
			tree_node_list[new_idx].nextsibing_lastchild = tree_node_list[last_ptr].nextsibing_lastchild;
			tree_node_list[last_ptr].nextsibing_lastchild = new_idx;
		}
		else
		{
			tree_node_list[new_idx].nextsibing_lastchild = tree_node_list[parent].firstchild;
			tree_node_list[parent].firstchild = new_idx;
		}
		ret_idx = new_idx;
	}
	return ret_idx;
}

int Tree::insertFeatureBranch(const std::vector<uint32_t>& feature_list, uint32_t feature_no)
{
	uint32_t idx = 0;
	if (feature_list.size() == 0)
	{
		return 0;
	}

	uint32_t cur_idx = insert(0, feature_list[0]);
	idx ++;
	
	while((idx<feature_list.size()) && (cur_idx!=0))
	{
		cur_idx = insert(cur_idx, feature_list[idx]);
		idx ++;
	}

	if (cur_idx == 0)
	{
		cerr << "Insert Branch Error!" << endl;
		return -1;
	}
	else
	{
		tree_node_list[cur_idx].data.feature = feature_no;
	}
	return 0;
}

uint32_t Tree::findInParent(uint32_t parent, uint32_t id)
{
	uint32_t cur_ptr = tree_node_list[parent].firstchild;
	uint32_t last_ptr = 0;
	while (cur_ptr != 0 && tree_node_list[cur_ptr].data.id < id)
	{
		last_ptr = cur_ptr;
		cur_ptr = tree_node_list[cur_ptr].nextsibing_lastchild;
	}
	if (tree_node_list[cur_ptr].data.id == id)
	{
		return cur_ptr;
	}
	else
	{
		return 0;
	}
}


uint32_t Tree::findFromSibing(uint32_t sibing, uint32_t id)
{
	uint32_t cur_ptr = sibing;
	uint32_t last_ptr = 0;
	while (cur_ptr != 0 && tree_node_list[cur_ptr].data.id < id)
	{
		last_ptr = cur_ptr;
		cur_ptr = tree_node_list[cur_ptr].nextsibing_lastchild;
	}
	if (tree_node_list[cur_ptr].data.id == id)
	{
		return cur_ptr;
	}
	else
	{
		return 0;
	}
}

uint32_t Tree::binaryFindFromSibing(uint32_t sibing, uint32_t id)
{
	uint32_t cur_idx = sibing;
	uint32_t last_idx = tree_node_list[tree_node_list[sibing].parent].nextsibing_lastchild;

	if (tree_node_list[last_idx].data.id == id)
	{
		return last_idx;
	}
	else if (tree_node_list[cur_idx].data.id == id)
	{
		return cur_idx;
	}
	else if (tree_node_list[last_idx].data.id < id)
	{
		return 0;
	}
	else if (tree_node_list[cur_idx].data.id > id)
	{
		return 0;
	}
	
	while (cur_idx <= last_idx)
	{
		uint32_t middle = (cur_idx + last_idx) / 2;
		if (tree_node_list[middle].data.id == id)
		{
			return middle;
		}
		else if (tree_node_list[middle].data.id < id)
		{
			cur_idx = middle + 1;
		}
		else
		{
			last_idx = middle - 1;
		}
	}
	return 0;
}

uint32_t Tree::getNewNode()
{
	uint32_t new_index = spare_index;
	if(spare_index != 0)
	{
		spare_index = tree_node_list[spare_index].nextsibing_lastchild;
	}
	return new_index;
}

int Tree::withdrawNode(uint32_t wd_idx)
{
	tree_node_list[wd_idx].firstchild = 0;
	tree_node_list[wd_idx].nextsibing_lastchild = spare_index;
	tree_node_list[wd_idx].parent = 0;
	spare_index = wd_idx;
	return 0;
}

void Tree::initNode(bool withMem)
{
	if (withMem)
	{
		tree_node_list = new TreeNode[length];
	}
	for(uint32_t i=1; i<(length-1); i++)
	{
		tree_node_list[i].nextsibing_lastchild = (i+1);
		tree_node_list[i].firstchild = 0;
		tree_node_list[i].parent = 0;
		tree_node_list[i].data.id = 0;
		tree_node_list[i].data.feature = 0;
	}
	tree_node_list[length-1].nextsibing_lastchild = 0;
	tree_node_list[length-1].firstchild = 0;
	tree_node_list[length-1].data.id = 0;
	tree_node_list[length-1].data.feature= 0;
	tree_node_list[length-1].parent = 0;

	tree_node_list[0].firstchild = 0;
	tree_node_list[0].nextsibing_lastchild = 0;
	tree_node_list[0].data.id = 0;
	tree_node_list[0].data.feature= 0;
	tree_node_list[0].parent = 0;

	spare_index = 1;
}

void Tree::generateFeature(const std::vector<uint32_t>& original_feature, std::vector<uint32_t>& feature_list)
{
	vector<SimpleFeature> sfeature_list;
	generateFeature(original_feature, sfeature_list, 0, 0, false);
	sort (sfeature_list.begin(), sfeature_list.end(), SimpleFeature::GreaterSize);

	map<uint32_t, uint32_t> uniq_map;
	for (uint32_t i=0; i<sfeature_list.size(); i++)
	{
		bool flag = false;
		for (uint32_t j=0; j<sfeature_list[i].feature_vec.size(); j++)
		{
			if (uniq_map.find(sfeature_list[i].feature_vec[j]) == uniq_map.end())
			{
				flag = true;
				uniq_map[sfeature_list[i].feature_vec[j]] ++;
			}
		}
		if (flag)
		{
			feature_list.push_back(sfeature_list[i].feature_no);
		}
	}

}

int Tree::generateFeature(const std::vector<uint32_t>& original_feature, std::vector<SimpleFeature>& feature_list,
	uint32_t vec_idx, uint32_t tree_parent_idx, bool hasSpecial)
{
	if (vec_idx == original_feature.size())
	{
		return 0;
	}
	uint32_t cur_idx = tree_node_list[tree_parent_idx].firstchild;
	if (cur_idx == 0)
	{
		return 0;
	}

	uint32_t cur_flag = 0;
	uint32_t last_idx = 0;
	for (uint32_t i=vec_idx; i<original_feature.size(); i++)
	{
		bool isSpl = isSpecial(original_feature[i]);
		if (hasSpecial && isSpl)
		{
			continue;
		}
		//find feature in all children
		last_idx = cur_idx;
		cur_idx = binaryFindFromSibing(cur_idx, original_feature[i]);	
		if (cur_idx != 0)
		{
			int flag = generateFeature(original_feature, feature_list, i+1, cur_idx, isSpl);
			if (flag == 1)
			{
				cur_flag = 1;
			}
			else if (tree_node_list[cur_idx].data.feature != 0)
			{
				cur_flag = 1;

				SimpleFeature sfeature;
				sfeature.feature_no = tree_node_list[cur_idx].data.feature;

				uint32_t p_idx = cur_idx;
				while(p_idx != 0)
				{
					sfeature.feature_vec.push_back(tree_node_list[p_idx].data.id);
					p_idx = tree_node_list[p_idx].parent;
				}
				reverse(sfeature.feature_vec.begin(), sfeature.feature_vec.end());
				feature_list.push_back(sfeature);
			}
		}
		else if(last_idx != 0)
		{
			cur_idx = last_idx;
		}
	}

	return cur_flag;
}

bool Tree::isSpecial (uint32_t feature_id)
{
	if (feature_id>20000000 && feature_id < 30000000)
	{
		return true;
	}
	return false;
}

int Tree::loadBinary(const char* filename)
{
	ifstream in(filename, ios::binary);
	if (!in)
	{
		//cerr << "Cannot open File: " << filename << endl;
		return -1;
	}
	in.seekg(0, ios::end);
	int length = in.tellg();
	if (length < 0)
	{
		//cerr << "Seek File Error: " << filename << endl;
		return -1;
	}
	in.seekg(0, ios::beg);

	uint32_t tree_length = length / sizeof(TreeNode);
	this->length = tree_length;
	if (tree_node_list != NULL)
	{
		delete [] tree_node_list;
	}
	tree_node_list = new TreeNode[tree_length];	

	in.read((char*)tree_node_list, sizeof(TreeNode)*tree_length);

	spare_index = 0;
	in.close();
	return 0;
}

int Tree::outputBinary(const char* filename)
{
	ofstream out(filename, ios::binary);
	if (!out)
	{
		//cerr << "Cannot open File: " << filename << endl;
		return -1;
	}
	out.write((char*)tree_node_list, sizeof(TreeNode)*spare_index);
	out.close();
	return 0;
}

int Tree::transferTree()
{
	TreeNode* new_tree_node_list = new TreeNode[length];
	memset((char*)(new_tree_node_list), 0, sizeof(TreeNode)*length);

	deque<uint32_t> old_node_queue;
	deque<uint32_t> new_node_queue;
	old_node_queue.push_back(0);
	new_node_queue.push_back(0);

	uint32_t new_spare_idx = 1;
	while(old_node_queue.size() != 0)
	{
		uint32_t old_cur_idx = old_node_queue.front();
		uint32_t new_cur_idx = new_node_queue.front();
		old_node_queue.pop_front();
		new_node_queue.pop_front();

		uint32_t old_child_idx = tree_node_list[old_cur_idx].firstchild;
		uint32_t new_child_idx = new_spare_idx;
		if (old_child_idx != 0)
		{
			new_child_idx = new_spare_idx;
			new_spare_idx ++;
			new_tree_node_list[new_cur_idx].firstchild = new_child_idx;
			new_tree_node_list[new_cur_idx].nextsibing_lastchild = new_child_idx;
			new_tree_node_list[new_child_idx].parent = new_cur_idx;
			new_tree_node_list[new_child_idx].data = tree_node_list[old_child_idx].data;

			old_node_queue.push_back(old_child_idx);
			new_node_queue.push_back(new_child_idx);

			old_child_idx = tree_node_list[old_child_idx].nextsibing_lastchild;
		}

		while(old_child_idx != 0)
		{
			new_child_idx = new_spare_idx;
			new_tree_node_list[new_cur_idx].nextsibing_lastchild = new_child_idx;
			new_tree_node_list[new_child_idx].parent = new_cur_idx;
			new_tree_node_list[new_child_idx].data = tree_node_list[old_child_idx].data;

			old_node_queue.push_back(old_child_idx);
			new_node_queue.push_back(new_child_idx);

			old_child_idx = tree_node_list[old_child_idx].nextsibing_lastchild;
			new_spare_idx ++;
		}
	}
	delete [] tree_node_list;
	tree_node_list = new_tree_node_list;
	return 0;
}

#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
public:
	/**
	 * TODO constructors
	 */
	struct node {
		node* left, * right;
		int npl;
		T* data;
		node() {
			npl = 0;
			left = right = nullptr;
			data = nullptr;
		}
		~node()
		{
			if (data!=nullptr) delete data;
			if (left != nullptr) delete left;
			if (right != nullptr) delete right;//�ݹ�ɾ�����ҽ��
		}
		int getnpl()
		{
			if (left == nullptr || right == nullptr)
			{
				npl = 0;
				return 0;
			}
			int l_npl = left->getnpl(), r_npl = right->getnpl();
			npl = l_npl < r_npl ? 1 + l_npl : 1 + r_npl;
			
			return npl;
		}
		void copy_tree_data(const node* copied)//�����е�������������,��֤���������ǿ�
		{
			npl = copied->npl;
			data = new T(*(copied->data));
			if (copied->left != nullptr)
			{
				left = new node;
				left->copy_tree_data(copied->left);
			}
			if (copied->right != nullptr)
			{
				right = new node;
				right->copy_tree_data(copied->right);
			}
		}
		
		
	};
	void swap(node* &a, node* &b)
	{
		node* tmp;
		tmp = a;
		a = b;
		b = tmp;
	}
	void mergen(node*& merge_tree, node*& merged_tree)
	{
		if (merge_tree == nullptr) {
			merge_tree = merged_tree;
			return;
		}
		if (merged_tree == nullptr)
		{
			return;
		}
		Compare cmp;
		if (cmp(*(merge_tree->data),*(merged_tree->data)))
		{
			swap(merge_tree, merged_tree);
		}//��֤merge_tree�ĸ��ڵ�С
		if (merge_tree->right == nullptr) {
			merge_tree->right = merged_tree;
			merge_tree->getnpl();
			if (merge_tree->left == nullptr || merge_tree->left->npl < merge_tree->right->npl) swap(merge_tree->left, merge_tree->right);
			return;
		}
		mergen(merge_tree->right, merged_tree);
		if (merge_tree->left == nullptr || merge_tree->left->npl < merge_tree->right->npl) swap(merge_tree->left, merge_tree->right);
	}
	node* root;
	int siz;

	priority_queue() {
		siz = 0;
		root = nullptr;
	}
	priority_queue(const priority_queue& other) {
		if (other.siz == 0)
		{
			siz = 0;
			return;
		}
		siz = other.siz;
		root = new node;
		root->copy_tree_data(other.root);
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
		if (root) delete root;
		root = nullptr;
		siz = 0;
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if (root == other.root) return *this;
		if (root) delete root;
		root = nullptr;
		if (other.siz == 0) {
			siz = 0;
			return *this;
		}
		node* newroot = new node;
		newroot->copy_tree_data(other.root);
		root = newroot;
		siz = other.siz;
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if (siz == 0) throw container_is_empty();
		return *(root->data);
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		node* newnode = new node;
		newnode->data = new T(e);
		mergen(root,newnode);
		siz++;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if (siz == 0) throw container_is_empty();
		node* l_tree = root->left, * r_tree = root->right;
		root->left = nullptr; root->right = nullptr;
		delete root;
		mergen(l_tree,r_tree);
		root = l_tree;
		siz--;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return size_t(siz);
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return siz == 0;
	}
	/**
	 * merge two priority_queues with at least O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other) {
		mergen(this->root,other.root);
		siz += other.siz;
		other.root = nullptr;
		other.siz = 0;
		return;
	}
};

}

#endif
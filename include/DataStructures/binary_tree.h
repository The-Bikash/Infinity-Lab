//module;
//#include <stdio.h>
//#include <tuple>
//export module binary_tree;
//
//import memory;
//import utility;
//import io;
//import stack;
//
//export namespace alpha {
//	template<class _Ty, _SizeType _Dim>
//	struct _NODE {
//		_NODE* _Node[_Dim];
//		_Ty _Val;
//	};
//
//	template<class _Ty>
//	class BinaryTree {
//	public:
//		using ArgType = _Ty;
//		using Pointer = _Ty*;
//		using Reference = _Ty&;
//		using ConstReference = conditional_t<is_fundamental<_Ty>::value || is_pointer<_Ty>::value, const _Ty, const _Ty&>;
//		using ConstPointer = const _Ty*;
//		using ValueType = _Ty;
//		using SizeType = _SizeType;
//
//	protected:
//		using _TreeNode = _NODE<_Ty, 2>;
//		_TreeNode* _Root = nullptr;
//		static constexpr const bool _Right = 0;
//		static constexpr const bool _Left = 1;
//		using Allocator = allocator<_TreeNode>;
//		Allocator _Allocator;
//	};
//
//	template<class _Ty, class Comparator = less<_Ty>>
//	class BST : public BinaryTree<_Ty> {
//	public:
//		using ArgType = _Ty;
//		using Pointer = _Ty*;
//		using Reference = _Ty&;
//		using ConstReference = conditional_t<is_fundamental<_Ty>::value || is_pointer<_Ty>::value, const _Ty, const _Ty&>;
//		using ConstPointer = const _Ty*;
//		using ValueType = _Ty;
//		using SizeType = _SizeType;
//		using TreeNode = BinaryTree<_Ty>::_TreeNode;
//		using BinaryTree<_Ty>::_Root;
//		using BinaryTree<_Ty>::_Allocator;
//		using BinaryTree<_Ty>::_Right;
//		using BinaryTree<_Ty>::_Left;
//	public:
//		inline constexpr BST(ConstReference _Val)noexcept {
//			_Root = _Allocator.allocate(1);
//			_Root->_Val = _Val;
//			_Root->_Node[_Right] = _Root->_Node[_Left] = nullptr;
//
//		}
//
//		inline constexpr BST()noexcept {};
//
//		inline constexpr ~BST() {
//		}
//		
//		virtual constexpr void insert(ConstReference _Val)noexcept {
//			TreeNode** _Current = &_Root;
//			while (_Current[0] != nullptr)
//				if (_Val != _Current[0]->_Val)
//					_Current = &(_Current[0]->_Node[_Compare(_Val, _Current[0]->_Val)]);
//				else return;
//
//			_Current[0] = _Allocator.allocate(1);
//			construct(&(_Current[0]->_Val), _Val);
//			_Current[0]->_Node[_Left] = _Current[0]->_Node[_Right] = nullptr;
//		}
//
//		constexpr void inOrderTraversal()noexcept {
//			print("\n\n");
//			inOrderTraversal(_Root);
//		}
//		void print() {
//			printTree(_Root, 0);
//		}
//		
//		
//	protected:
//		Comparator _Compare;
//
//
//		void inOrderTraversal(TreeNode* _Node)noexcept {
//			if (_Node != nullptr) {
//				inOrderTraversal(_Node->_Node[_Left]);
//				print(_Node->_Val, " ");
//				inOrderTraversal(_Node->_Node[_Right]);
//			}
//		}
//
//		void deleteTree(TreeNode* node) {
//			/*if (node == nullptr) {
//				return;
//			}
//			deleteTree(node->_Left);
//			deleteTree(node->_Right);
//			_Allocator.deallocate(node, 1);
//			node = nullptr;*/
//		}
//
//		void printTree(TreeNode* root, int space) {
//			if (root == nullptr) {
//				return;
//			}
//
//			// Increase the space between levels
//			space += 10;
//
//			// Recursively print the right subtree
//			printTree(root->_Node[_Right], space);
//
//			// Print the current node
//			printf("\n");
//			for (int i = 10; i < space; i++) {
//				printf(" ");
//			}
//			printf("%d\n", root->_Val);
//
//			// Recursively print the left subtree
//			printTree(root->_Node[_Left], space);
//		}
//	};
//	template<class _Ty>
//	struct Node { _Ty _Val; int _Height; };
//
//	template<class _Ty, class Comparator = less<_Ty>>
//	class AVLT : public BST<Node<_Ty> , Comparator > {
//	public:
//		using ArgType = _Ty;
//		using Pointer = _Ty*;
//		using Reference = _Ty&;
//		using ConstReference = conditional_t<is_fundamental<_Ty>::value || is_pointer<_Ty>::value, const _Ty, const _Ty&>;
//		using ConstPointer = const _Ty*;
//		using ValueType = _Ty;
//		using SizeType = _SizeType;
//		using TreeNode = BinaryTree<_Ty>::_TreeNode;
//		using BinaryTree<_Ty>::_Root;
//		using BinaryTree<_Ty>::_Allocator;
//		using BinaryTree<_Ty>::_Right;
//		using BinaryTree<_Ty>::_Left;
//
//#define _UPDATE_HEIGHT(_Current) _Current->_Height = 1 + max(_Current->_Node[_Right]->_Height, _Current->_Node[_Left]->_Height)
//	public:
//		constexpr void insert(ConstReference _Val)noexcept override {
//			static const constexpr auto LL = [](TreeNode* Z)->TreeNode* {
//				//		   z                                      y 
//				//        / \                                   /   \
//				//       y   T4      Right Rotate (z)          x      z
//				//      / \          - - - - - - - - ->      /  \    /  \ 
//				//     x   T3                               T1  T2  T3  T4
//				//    / \
//				//  T1   T2
//
//				TreeNode* Y = Z->_Node[_Left];
//				TreeNode* T3 = Y->_Node[_Right];
//				Y->_Node[_Right] = Z;
//				Z->_Node[_Left] = T3;
//				Z->_Height = max(Z->_Node[_Right]->_Height, Z->_Node[_Left]->_Height) + 1;
//				Y->_Height = max(Y->_Node[_Right]->_Height, Y->_Node[_Left]->_Height) + 1;
//				return Y;
//
//				};
//			static const constexpr auto LR = [](TreeNode* Z)->TreeNode* {
//				// 	    z                               z                           x
//				//     / \                            /   \                        /  \ 
//				//    y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
//				//   / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
//				// 
//				// T1   x                          y    T3                    T1  T2 T3  T4
//				//     / \                        / \
//				//   T2   T3                    T1   T2
//
//				TreeNode* Y = Z->_Node[_Left];
//				TreeNode* X = Y->_Node[_Right];
//				Y->_Node[_Right] = X->_Node[_Left];
//				Z->_Node[_Left] = X->_Node[_Right];
//				X->_Node[_Right] = Z;
//				X->_Node[_Left] = Y;
//				Y->_Height = max(Y->_Node[_Left]->_Height, Y->_Node[_Right]->_Height) + 1;
//				Z->_Height = max(Z->_Node[_Left]->_Height, Z->_Node[_Right]->_Height) + 1;
//				X->_Height = max(Y->_Height, Z->_Height) + 1;
//				return X;
//
//				};
//			static const constexpr auto RR = [](TreeNode* Z)->TreeNode* {
//				// 	 z                               y
//				//  / \                            /   \ 
//				// T1  y     Left Rotate(z)       z      x
//				//    /  \   - - - - - - - ->    / \    / \
//				//   T2   x                     T1  T2 T3  T4
//				//       / \
//				//      T3  T4
//
//
//				TreeNode* Y = Z->_Node[_Right];
//				TreeNode* T2 = Y->_Node[_Left];
//				Y->_Node[_Left] = Z;
//				Z->_Node[_Right] = T2;
//				Z->_Height = max(Z->_Node[_Left]->_Height, Z->_Node[_Right]->_Height) + 1;
//				Y->_Height = max(Y->_Node[_Left]->_Height, Y->_Node[_Right]->_Height) + 1;
//				return Y;
//
//			};
//			static const constexpr auto RL = [](TreeNode* Z)->TreeNode* {
//				// 	  z                            z                            x
//				//   / \                          / \                          /  \ 
//				// T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
//				//     / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
//				//    x   T4                      T2   y                  T1  T2  T3  T4
//				//   / \                              / \
//				// T2   T3                           T3  T4
//
//				TreeNode* Y = Z->_Node[_Right];
//				TreeNode* X = Y->_Node[_Left];
//				Y->_Node[_Left] = X->_Node[_Right];
//				Z->_Node[_Right] = X->_Node[_Left];
//				X->_Node[_Left] = Z;
//				X->_Node[_Right] = Y;
//				Y->_Height = max(Y->_Node[_Left]->_Height, Y->_Node[_Right]->_Height) + 1;
//				Z->_Height = max(Z->_Node[_Left]->_Height, Z->_Node[_Right]->_Height) + 1;
//				X->_Height = max(Y->_Height, Z->_Height) + 1;
//				return X;
//
//				};
//
//			static const constexpr TreeNode** _Stack[65];
//			static const constexpr TreeNode** _Bottom = _Stack - 1;
//			TreeNode** _Top = _Bottom;
//			TreeNode* _NewNode = _ConstructTreeNode(_Val);
//			TreeNode* _Current = _Root;
//
//			while (_Current) {
//				*++_Top = _Current;
//				if (_Compare(_Val, _Current->_Node[_Left])) {
//					if (_Current->_Node[_Left] == nullptr) {
//						_Current->_Node[_Left] = _NewNode;
//						break;
//					}
//					_Current = _Current->_Node[_Left];
//				}
//				else if (_Current->_Val == _Val) return;
//				else {
//					if (_Current->_Node[_Right] == nullptr) {
//						_Current->_Node[_Right] = _NewNode;
//						break;
//					}
//					_Current = _Current->_Node[_Right];
//				}
//			}
//
//			while (*_Top != *_Bottom) {
//				_Current = *_Top--;
//				_UPDATE_HEIGHT((*_current));
//				int _Balance = _Current->_Node[_Left]->_Height - _Current->_Node[_Right]->_Height;
//				if (_Balance > 1) {
//					if (_Compare(_Val, _Current->_Node[_Left]))
//						_Current = LL(_Current);
//					else
//						_Current = LR(_Current);
//				}
//				else if (_Balance < -1) {
//					if (_Compare(_Val, (*_Top)->_Node[_Left]))
//						_Current = RL(_Current);
//					else
//						_Current = RR(_Current);
//				}
//			}
//			_Root = _Current;
//		}
//	protected:
//		inline constexpr TreeNode* _ConstructTreeNode(ConstReference _Val) {
//			TreeNode* _Tmp = _Allocator.allocate(1);
//			construct(&(_Tmp->_Val)), _Val);
//			_Tmp->_Height = 1;
//			_Tmp->_Node[_Left] = _Tmp->_Node[_Right] = nullptr;
//			return _Tmp;
//		}
//
//	};
//}
#include "../include/Memory/xmemory.h"
#include "../include/Core/core.h"
#include "../include/Core/utility.h"


namespace alpha {
    struct Node {
        char* _Ptr = nullptr;
        Node* _Next = nullptr;
    };

    struct NodeList {
        Node* _Head;
        _SizeType _NodeCount;
        _SizeType _Size;

        ~NodeList()noexcept {
            if (_Head != nullptr) {
                auto* _Next = _Head;
                while (true) {
                    if (_Next->_Ptr != nullptr) {
                        operator delete[](static_cast<void*>(_Next->_Ptr), _Size);
                        _Next->_Ptr = nullptr;
                    }
                    _Head = _Next->_Next;
                    operator delete(_Next, sizeof(Node));
                    _Next = _Head;
                    if (_Next == nullptr)
                        break;
                }
            }
        }

        inline constexpr void insert_front(Node* _NewHead)noexcept {
            if constexpr (_debug) {
                if (_NewHead == nullptr)
                    return;
            }
            auto* _NextNode = _Head;
            _Head = _NewHead;
            _Head->_Next = _NextNode;
            ++_NodeCount;
        }
        inline constexpr Node* pop()noexcept {
            if (_Head == nullptr) {
                auto* _NewNode = static_cast<Node*>(operator new(sizeof(Node)));
                _NewNode->_Ptr = static_cast<char*>(operator new[](_Size));
                _NewNode->_Next = nullptr;
                return _NewNode;
            }

            auto* _head = _Head;
            _Head = _Head->_Next;
            _head->_Next = nullptr;
            --_NodeCount;
            return _head;
        }
        inline constexpr Node* pop(char* _Pointer)noexcept {
            if (_Head->_Ptr == _Pointer) {
                auto* _Node = _Head;
                _Head = _Head->_Next;
                _Node->_Next = nullptr;
                --_NodeCount;
                return _Node;
            }

            auto* _NextNode = _Head->_Next;
            auto* _PreviousNode = _Head;
            while (true) {
                if constexpr (_debug) {
                    if (_NextNode == nullptr)
                        __debugbreak();
                }
                if (_NextNode->_Ptr == _Pointer) {
                    _PreviousNode->_Next = _NextNode->_Next;
                    _NextNode->_Next = nullptr;
                    --_NodeCount;
                    return _NextNode;
                }
                _PreviousNode = _NextNode;
                _NextNode = _NextNode->_Next;
            }
        }
    };

    struct MemoryChunk {
        NodeList _AvailableNodes;
        NodeList _AllocatedNodes;
    };

    struct MemoryEngineImpl {
        MemoryEngineImpl() {
            for (char i = 0; i < 64; ++i) {
                _Chunks[i]._AvailableNodes._Head = nullptr;
                _Chunks[i]._AllocatedNodes._Head = nullptr;

                _Chunks[i]._AvailableNodes._NodeCount = 0;
                _Chunks[i]._AllocatedNodes._NodeCount = 0;

                _Chunks[i]._AvailableNodes._Size = 1ULL << i;
                _Chunks[i]._AllocatedNodes._Size = 1ULL << i;
            }
            _CurrentMemoryUseStatus = 0;
            _CurrentPoolSizeStatus = 0;
        }

        constexpr void* _Malloc(char _idx)noexcept {
            _CurrentMemoryUseStatus += (1ULL << _idx);
            _CurrentPoolSizeStatus += (1ULL << _idx);
            auto* _Node = _Chunks[_idx]._AvailableNodes.pop();
            _Chunks[_idx]._AllocatedNodes.insert_front(_Node);
            return static_cast<void*>(_Node->_Ptr);
        }
        constexpr void _Free(void* _Ptr, char _idx)noexcept {
            if constexpr (_debug) {
                if (_Ptr == nullptr)
                    return;
                if (_idx > 63 || _idx < 0)
                    __debugbreak();
            }

            _Chunks[_idx]._AvailableNodes.insert_front(_Chunks[_idx]._AllocatedNodes.pop(static_cast<char*>(_Ptr)));

            if (_Condition())
                _AI_Destructor();
        }
        constexpr void* _Realloc(void* _Ptr, char _idx1, char _idx2)noexcept {
            if (_idx1 == _idx2)
                return _Ptr;
            auto* _NewPtr = _Malloc(_idx2);
            memcpy(_NewPtr, _Ptr, 1ULL << (_idx1 < _idx2 ? _idx1 : _idx2));
            _Free(_Ptr, _idx1);
            return _NewPtr;
        }
        constexpr char _index(_SizeType& Size)noexcept {
            char _idx = 0;
            _SizeType size = 1;
            while (size < Size) {
                size <<= 1;
                ++_idx;
            }
            Size = size;
            return _idx;
        }
        constexpr bool _Condition()noexcept {
            return false;
        }
        constexpr void _AI_Destructor()noexcept {

        }
    private:
        MemoryChunk _Chunks[64];
        _SizeType _CurrentMemoryUseStatus;
        _SizeType _CurrentPoolSizeStatus;
           
    };

    inline MemoryEngineImpl& _MemoryManager() {
        static MemoryEngineImpl Manager;
        return Manager;
    }

    void* allocate(_SizeType& _Size) {
        auto& Manager = _MemoryManager();
        return Manager._Malloc(Manager._index(_Size));
    }
    void* reallocate(void* _Ptr, _SizeType _Size, _SizeType& _NewSize) {
        auto& Manager = _MemoryManager();
        return Manager._Realloc(_Ptr, Manager._index(_Size), Manager._index(_NewSize));
    }
    void destroy(void* _Ptr, _SizeType _Size) {
        auto& Manager = _MemoryManager();
        Manager._Free(_Ptr, Manager._index(_Size));
        return;
    }
}


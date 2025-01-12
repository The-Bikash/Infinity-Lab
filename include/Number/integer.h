// module;

// export module integer;

// import core;
// import utility;

// export namespace alpha {
//     using _SignedType = long long;
//     using _MulType = unsigned long long;
//     using _SizeType = unsigned long long;

//     class UnsignedInt {
//     public:
//         friend inline void _print(const UnsignedInt& _That)noexcept;
//         friend inline void Add(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void Sub(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void Mul(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void div(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void mod(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void mulx(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void addi(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;
//         friend inline void mulm(UnsignedInt& _Answ, const UnsignedInt& _First, const UnsignedInt& _Secnd)noexcept;

//         UnsignedInt()noexcept;
//         UnsignedInt(const _SizeType _Size)noexcept;
//         UnsignedInt(UnsignedInt&& _That)noexcept;
//         UnsignedInt(const UnsignedInt& _That)noexcept;

//         ~UnsignedInt()noexcept;

//         constexpr UnsignedInt& operator=(const _SizeType _Size)noexcept;
//         constexpr UnsignedInt& operator=(UnsignedInt&& _That)noexcept;
//         constexpr UnsignedInt& operator=(const UnsignedInt& _That)noexcept;


//         [[nodiscard]] constexpr bool operator==(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] constexpr bool operator!=(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] constexpr bool operator> (const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] constexpr bool operator< (const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] constexpr bool operator>=(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] constexpr bool operator<=(const UnsignedInt& _That)const noexcept;


//         constexpr UnsignedInt& operator<<=(const _SizeType _Shift)noexcept;
//         constexpr UnsignedInt& operator>>=(const _SizeType _Shift)noexcept;

//         constexpr UnsignedInt& operator|=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator&=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator^=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator+=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator-=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator*=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator/=(const UnsignedInt& _That)noexcept;
//         constexpr UnsignedInt& operator%=(const UnsignedInt& _That)noexcept;


//         [[nodiscard]] UnsignedInt operator<<(const _SizeType that)const noexcept;
//         [[nodiscard]] UnsignedInt operator>>(const _SizeType that)const noexcept;
//         [[nodiscard]] UnsignedInt operator|(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] UnsignedInt operator&(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] UnsignedInt operator^(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] UnsignedInt operator+(const UnsignedInt& _That)const noexcept;
//         [[nodiscard]] UnsignedInt operator-(const UnsignedInt& _That)const noexcept;

//     private:
//         struct UnsignedIntImpl;
//         UnsignedIntImpl* _Impl;
//     };

//     struct unsigned_int128 {

//         unsigned long long _Num[2];

//         constexpr auto& operator+=(const unsigned_int128& _That) noexcept {
//             _Num[0] += _That._Num[0];
//             _Num[1] += _That._Num[1] + (_Num[0] < _That._Num[0]);
//             return *this;
//         }

//         constexpr auto& operator+=(const unsigned long long& _That) noexcept {
//             _Num[0] += _That;
//             _Num[1] += (_Num[0] < _That);
//             return *this;
//         }

//         constexpr auto operator+(const unsigned_int128& _That) const noexcept {
//             unsigned_int128 _Tmp{};
//             _Tmp._Num[0] = _Num[0] + _That._Num[0];
//             _Tmp._Num[1] = _Num[1] + _That._Num[1] + (_Tmp._Num[0] < _Num[0]);
//             return _Tmp;
//         }

//         constexpr auto operator+(const unsigned long long& _That) const noexcept {
//             unsigned_int128 _Tmp{};
//             _Tmp._Num[0] = _Num[0] + _That;
//             _Tmp._Num[1] = _Num[1] + (_Tmp._Num[0] < _That);
//             return _Tmp;
//         }

//         constexpr auto& operator-=(const unsigned_int128& _That) noexcept {
//             if (_That._Num[0] > _Num[0]) {
//                 _Num[1]--;
//             }
//             _Num[0] -= _That._Num[0];
//             _Num[1] -= _That._Num[1];
//             return *this;
//         }

//         constexpr auto operator-(const unsigned_int128& _That) const noexcept {
//             unsigned_int128 _Tmp{};
//             _Tmp._Num[0] = _Num[0] - _That._Num[0];
//             if (_That._Num[0] > _Num[0]) {
//                 _Tmp._Num[1]--;
//             }
//             _Tmp._Num[1] = _Num[1] - _That._Num[1];
//             return _Tmp;
//         }

//         constexpr auto& operator*=(const unsigned_int128& _That) noexcept {
//             unsigned long long a[4];
//             unsigned long long b[4];
//             unsigned long long res[4];
//             a[0] = _Num[0] & 0xFFFFFFFFULL;
//             a[1] = _Num[0] >> 32;
//             a[2] = _Num[1] & 0xFFFFFFFFULL;
//             a[3] = _Num[1] >> 32;
//             b[0] = _That._Num[0] & 0xFFFFFFFFULL;
//             b[1] = _That._Num[0] >> 32;
//             b[2] = _That._Num[1] & 0xFFFFFFFFULL;
//             b[3] = _That._Num[1] >> 32;

//             for (int i = 0; i < 4; ++i) {
//                 for (int j = 0; j < 4; ++j) {
//                     res[i + j] += a[i] * b[j];
//                     res[i + j + 1] += res[i + j] >> 32;
//                     res[i + j] &= 0xFFFFFFFFULL;
//                 }
//             }

//             _Num[0] = (res[0] & 0xFFFFFFFFULL) | (res[1] << 32);
//             _Num[1] = (res[2] & 0xFFFFFFFFULL) | (res[3] << 32);

//             return *this;
//         }

//     };
// }
#pragma once

#include <compare>
#include <utility>
#include <optional>
#include <bit>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <array>
#include <algorithm>
#include <ranges>
#include "integer_type.hpp"

namespace zikken2a3::stew {
	struct Term final {
		u16 expression{0xFFFFu};

		static constexpr auto from_str(const u8 n, const char * str) noexcept -> Term {
			u16 expression{0xFFFFu};
			for(i32 i = 0; i < n; ++i) {
				switch(str[n - 1 - i]) {
					case '0':
						expression &= ~(3 << (2 * i));
						break;
					case '1':
						expression &= ~(3 << (2 * i));
						expression |= (1 << (2 * i));
						break;
					case 'x':
						break;
					default:
						throw std::invalid_argument("Term::make: invalid character");
				}
			}

			return Term{expression};
		}

		static constexpr auto from_u8(const u8 n, const u8 expression) noexcept -> Term {
			char str[9]{};
			for(i32 i = 0; i < n; ++i) {
				str[n - 1 - i] = (expression & (1 << i)) ? '1' : '0';
			}
			return Term::from_str(n, str);
		}

		friend constexpr auto operator<=>(const Term& lhs, const Term& rhs) = default;
		friend constexpr auto operator==(const Term& lhs, const Term& rhs) -> bool = default;

		friend constexpr auto merge(const Term& lhs, const Term& rhs) -> std::optional<Term> {
			if(const u16 res = std::popcount<u16>(lhs.expression ^ rhs.expression); 0 < res && res <= 2) {
				u16 expression = lhs.expression | rhs.expression;
				for(i32 i = 0; i < 8; ++i) {
					if((lhs.get(i) ^ rhs.get(i)) == 1) {
						expression |= (3 << (2 * i));
					}
				}
				return Term{expression};
			}
			else return std::nullopt;
		}

		constexpr auto get(const u8 idx) const noexcept -> u8 {
			if(idx > 7) {
				return 3;
			}
			return (this->expression >> (2 * idx)) & 3;
		}

		constexpr auto accept(const u8 x) const noexcept -> bool {
			for(i32 i = 0; i < 8; ++i) {
				if(this->get(i) == 3) {
					continue;
				}
				if(this->get(i) != ((x >> i) & 1)) {
					return false;
				}
			}
			return true;
		}

		constexpr auto count1() const noexcept -> u8 {
			u8 count{};
			for(i32 i = 0; i < 8; ++i) {
				if(this->get(i) == 1) {
					++count;
				}
			}
			return count;
		}

		constexpr auto as_str(const std::vector<std::string>& map_bit_str) const noexcept -> std::string {
			const u8 n = map_bit_str.size();
			std::string ret{};
			for(i32 i = n; i-- > 0;) {
				const auto res = this->get(i);
				switch(res) {
					case 0:
						ret += std::string("!") + map_bit_str[n - i - 1];
						break;
					case 1:
						ret += map_bit_str[n - i - 1];
						break;
					case 3:
					default:
					;
				}
			}

			return ret;
		}
	};

	// 主項を返す
	inline constexpr auto quine_mccluskey(const u8 n, const std::vector<u8>& trues) -> std::vector<Term> {
		using Count1Comp = decltype([](const Term& l, const Term& r) -> bool {return l.count1() != r.count1() ? l.count1() < r.count1() : l.expression < r.expression;});
		std::array<std::map<Term, bool, Count1Comp>, 2> terms{};
		for(u64 i = 0; i < trues.size(); ++i) {
			terms[0][Term::from_u8(n, trues[i])] = false;
		}
		std::unordered_set<u16> main_terms{};

		u64 i = 0;
		for(; ; ++i) {
			auto& last = terms[i & 1];
			auto& next = terms[!(i & 1)];
			next.clear();

			if(last.empty()) {
				break;
			}

			auto iter1 = last.begin();
			auto iter2 = std::next(iter1);
			while(iter2 != last.end() && iter2->first.count1() == iter1->first.count1()) {
				++iter2;
			}
			while(iter1 != last.end() && iter2 != last.end()) {
				auto iter3 = iter2;
				const u8 last_popcount = iter3->first.count1();
				for(; iter3 != last.end() && iter3->first.count1() == last_popcount; ++iter3) {
					for(auto iter4 = iter1; iter4 != iter2; ++iter4) {
						if(const auto merged = merge(iter3->first, iter4->first); merged) {
							next[*merged] = false;
							iter3->second = true;
							iter4->second = true;
						}
					}
				}

				iter1 = iter2;
				iter2 = iter3;
			}

			for(const auto& [term, is_used] : last) {
				if(!is_used) {
					main_terms.insert(term.expression);
				}
			}

			volatile int dummy = 0;
			dummy = dummy + 1;
		}

		std::vector<Term> result{};
		for(const auto& main_term : main_terms) {
			result.push_back(Term{main_term});
		}
		std::ranges::sort(result, std::greater{}, [](const Term& x) {return x.count1();});

		return result;
	}

	inline constexpr auto is_same(const std::vector<Term>& terms, const std::vector<u8>& trues) -> bool {
		for(const u8 t : trues) {
			bool found = false;
			for(const auto& term : terms) {
				if(term.accept(t)) {
					found = true;
					break;
				}
			}
			if(!found) {
				return false;
			}
		}

		return true;
	}

	inline constexpr auto get_main_terms(const u8 n, const std::vector<u8>& dcs, const std::vector<u8>& trues) -> std::vector<Term> {
		const auto primary_terms = quine_mccluskey(n, dcs);
		const auto m = primary_terms.size();
		std::vector<u8> bits(1 << m);
		for(u8 i = 0; i < (1 << m); ++i) {
			bits[i] = i;
		}
		std::ranges::sort(bits, {}, [](const u8 x) {return std::popcount(x);});
		auto chosed_terms = primary_terms;
		for(const u8 ibit : bits) {
			chosed_terms.clear();
			for(i32 ib = 0; ib < i32(m); ++ib) {
				if((ibit >> ib) & 1) {
					chosed_terms.emplace_back(primary_terms[ib]);
				}
			}
			if(is_same(chosed_terms, trues)) {
				return chosed_terms;
			}
		}

		throw std::runtime_error("get_main_terms: WTF??");
	}
}